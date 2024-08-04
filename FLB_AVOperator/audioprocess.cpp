#include "audioprocess.h"
#include "avoperator.h"
#include "audiomanager.h"
#include <QDebug>

AudioListProcessor::AudioListProcessor(QObject* p) :IAudioFrameProcessor(p)
{

}

void AudioListProcessor::addProcessor(AProcessSPtr sp)
{
	if (sp)
	{
		for (const auto& cur : m_listProcessors)
		{
			if (cur.get() == sp.get())
				return;
		}

		m_listProcessors.append(std::move(sp));
	}
}

void AudioListProcessor::removeProcessor(IAudioFrameProcessor* pap)
{
	for (int i = 0; i < m_listProcessors.size(); ++i)
	{
		if (m_listProcessors[i].get() == pap)
		{
			m_listProcessors.removeAt(i);
			break;
		}
	}
}

bool AudioListProcessor::lastNone()const
{
	return m_lastNone;
}

FrameSPtr AudioListProcessor::processFrame(FrameSPtr pf)
{
	bool inputNotNull = static_cast<bool>(pf);

	m_lastNone = true;
	for (auto& cur : m_listProcessors)
	{
		pf = cur->processFrame(pf);
		if (pf)
		{
			m_lastNone = false;
			inputNotNull = true; //这样使之前遇见空帧被跳过，之后遇见空帧再终止
		}
		else if (inputNotNull)
			break;
	}

	return pf;
}

FrameSPtr AudioListProcessor::getRestFrame(bool mustCmplete)
{
	m_lastNone = true;
	FrameSPtr spf;
	for (auto& cur : m_listProcessors)
	{
		if (spf)
			spf = cur->processFrame(spf);
		else
			spf = cur->getRestFrame(mustCmplete);

		if (spf)
		{
			m_lastNone = false;
		}
	}

	return spf;
}

VolumnAdjustProcessor::VolumnAdjustProcessor(QObject* p) :IAudioFrameProcessor(p)
{

}

FrameSPtr VolumnAdjustProcessor::processFrame(FrameSPtr pf)
{
	auto paf = this->getAVFrame(pf.get());
	if (!pf || paf->nb_samples <= 0 || m_factor == 1.0)
	{
		return pf;
	}

	FrameSPtr result = std::make_shared<FFrame>(*pf);
	auto volFunc = audio::getVolAdjustFunc(static_cast<AVSampleFormat>(paf->format));

	paf = this->getAVFrame(result.get());
	volFunc(paf, m_factor);

	return result;
}

void VolumnAdjustProcessor::onVolChanged(int v)
{
	m_factor = static_cast<double>(v) / 100;
	m_factor = std::max(0.0, m_factor);
	//m_factor = std::min(200.0, m_factor);
}


AudioSpeedProcessor::AudioSpeedProcessor(QObject* p) :IAudioFrameProcessor(p)
{
	this->reset();
}

float AudioSpeedProcessor::setSpeed(float speed, int nb_sample)
{
	if (speed <= 0 || nb_sample <= 0 || speed > nb_sample)
	{
		QMutexLocker locker(&m_mutex);
		this->reset();
		return m_speed;
	}

	QMutexLocker locker(&m_mutex);
	m_dstSample = nb_sample / speed;
	m_speed = speed;

	return this->getSpeed();
}

void AudioSpeedProcessor::setAVInfo(const FAVInfo& info)
{
	this->reset();
	m_info = info;
	if (m_info.m_aIndx >= 0)
	{
		m_upAudioManager = audio::getManagerPtr(m_info.m_sampleFmt);
		if (m_upAudioManager)
			m_func = m_upAudioManager->getSpeedFunc(m_info.m_nChannel, m_info.m_sampleRate);
	}
}

float AudioSpeedProcessor::getSpeed()const
{
	return m_speed;
}

FrameSPtr AudioSpeedProcessor::getRestFrame(bool mustCmplete)
{
	if (m_numInSample >= m_info.m_frameSize * 3 || (m_numInSample > 0 && !mustCmplete))
		this->processInBuffer();

	return mustCmplete ? this->tryPopOutBuffer() : this->forcePopOutBuffer();
}

//FrameSPtr sp1, sp2;

FrameSPtr AudioSpeedProcessor::processFrame(FrameSPtr pf)
{
	if (!pf)
	{
		return this->forcePopOutBuffer();
	}

	auto paf = this->getAVFrame(pf.get());
	QMutexLocker locker(&m_mutex);
	if (paf->nb_samples <= 0 || m_speed == 1.0 || !m_func || !m_upAudioManager)
	{
		return pf;
	}

	//if (!sp1)
	//	sp1 = pf;
	this->inputFrame(paf);
	if (m_numInSample < m_info.m_frameSize * 3)
		return FrameSPtr();

	this->processInBuffer();

	if (m_numOutSample < m_dstSample)
	{
		return FrameSPtr();
	}
	else
	{
		return this->tryPopOutBuffer();
	}

}

void AudioSpeedProcessor::reset()
{
	m_speed = 1.0;
	m_dstSample = 0;
	m_info.m_aIndx = -1;
	m_numInSample = 0;
	m_numOutSample = 0;
	m_func = audio::getSpeedFunc(AV_SAMPLE_FMT_NONE, m_info.m_nChannel, m_info.m_sampleRate);

	m_bufferInSamples.clear();
	m_bufferOutSamples.clear();
	while (!m_queuePts.empty())
		m_queuePts.pop();
	m_lastEndPts = AV_NOPTS_VALUE;
	m_duration = AV_NOPTS_VALUE;
}

void AudioSpeedProcessor::inputFrame(AVFrame* paf)
{
	if (paf == nullptr)
		return;

	int per_size = m_upAudioManager->perSize();
	int base = per_size * paf->ch_layout.nb_channels;
	int size = paf->nb_samples * base, curInEnd = m_numInSample * base;
	m_bufferInSamples.resize(m_numInSample * base + size + 1);

	if (!av_sample_fmt_is_planar(m_upAudioManager->getSampleFmt()))
	{
		std::copy(paf->data[0], paf->data[0] + size, &m_bufferInSamples[curInEnd]);
	}
	else
	{
		int cur = curInEnd;
		for (int i = 0; i < paf->nb_samples; ++i)
		{
			for (int j = 0; j < paf->ch_layout.nb_channels; ++j)
			{
				for (int k = 0; k < per_size; ++k)
				{
					m_bufferInSamples[cur++] = paf->data[j][i * per_size + k];
				}
			}
		}
	}

	m_numInSample += paf->nb_samples;
	m_queuePts.push(paf->pts);
	if (m_duration != AV_NOPTS_VALUE)
		m_duration = paf->pkt_duration;
}

void AudioSpeedProcessor::outputFrame(AVFrame* paf)
{
	if (paf == nullptr)
		return;

	int nb_sample = std::min(paf->nb_samples, m_dstSample);
	int per_size = m_upAudioManager->perSize();
	int base = per_size * paf->ch_layout.nb_channels;

	if (av_sample_fmt_is_planar(static_cast<AVSampleFormat>(paf->format)))
	{
		for (int c = 0; c < paf->ch_layout.nb_channels; ++c)
		{
			for (int i = 0; i < nb_sample; ++i)
			{
				for (int j = 0; j < per_size; ++j)
				{
					paf->data[c][i * per_size + j] = m_bufferOutSamples[i * base + c * per_size + j];
				}
			}
		}
	}
	else
	{
		std::copy(&m_bufferOutSamples[0], &m_bufferOutSamples[nb_sample * base], paf->data[0]);
	}

	std::copy(&m_bufferOutSamples[nb_sample * base], &m_bufferOutSamples[m_numOutSample * base],
		&m_bufferOutSamples[0]);
	m_numOutSample -= nb_sample;
}

void AudioSpeedProcessor::reallocOutBuffer()
{
	int base = m_upAudioManager->perSize() * m_info.m_chLayout.nb_channels;
	m_bufferOutSamples.resize((m_numInSample / m_speed + m_numOutSample) * base + 1);
}

void AudioSpeedProcessor::processInBuffer()
{
	if (m_numInSample <= 0)
		return;

	this->reallocOutBuffer();

	int base = m_upAudioManager->perSize() * m_info.m_chLayout.nb_channels;
	int maxOut = m_bufferOutSamples.size() / base - m_numOutSample;
	auto ioRes = m_func(&m_bufferOutSamples[m_numOutSample * base], maxOut, &m_bufferInSamples[0], m_numInSample, m_speed);

	/*auto ioRes = std::pair<int, int>(m_numInSample, m_numInSample);
	{
		m_bufferOutSamples.resize(m_bufferInSamples.size() + m_bufferOutSamples.size());
		std::copy(m_bufferInSamples.begin(), m_bufferInSamples.end(), &m_bufferOutSamples[m_numOutSample * base]);

	}*/
	if (ioRes.first == 0)
		return;

	std::copy(&m_bufferInSamples[ioRes.first * base], &m_bufferInSamples[m_numInSample * base], &m_bufferInSamples[0]);
	m_numInSample -= ioRes.first;

	m_numOutSample += ioRes.second;
}

FrameSPtr AudioSpeedProcessor::tryPopOutBuffer()
{
	if (m_numOutSample == 0 || m_numOutSample < m_dstSample || m_info.m_aIndx < 0)
		return FrameSPtr();

	FrameSPtr res = this->createNewFrameBySample(&m_info, m_dstSample);
	auto paf = this->getAVFrame(res.get());
	outputFrame(paf);

	paf->pts = m_queuePts.empty() ? m_lastEndPts : m_queuePts.front();
	if (!m_queuePts.empty())
		m_queuePts.pop();
	m_lastEndPts = paf->pts + m_duration;

	IAudioFrameProcessor::setFrameValid(res.get());

	/*if (!sp2)
	{
		sp2 = res;
		auto f1 = this->getAVFrame(sp1.get()), f2 = this->getAVFrame(sp2.get());
		for (int i = 0; i < std::min(f1->nb_samples, f2->nb_samples); ++i)
		{
			for (int j = 0; j < m_info.m_chLayout.nb_channels; ++j)
			{
				QList<uint8_t> cur1, cur2;
				for (int k = 0; k < m_upAudioManager->perSize(); ++k)
				{
					cur1.append(f1->data[j][i * m_upAudioManager->perSize() + k]);
					cur2.append(f2->data[j][i * m_upAudioManager->perSize() + k]);
				}
				qDebug() << i << j;
				qDebug() << cur1;
				qDebug() << cur2;
			}
		}
	}*/
	return res;
}

FrameSPtr AudioSpeedProcessor::forcePopOutBuffer()
{
	if ((m_numOutSample == 0 && m_numInSample == 0) || m_info.m_aIndx < 0)
		return FrameSPtr();

	if (m_numInSample >= m_dstSample)
		return tryPopOutBuffer();

	int base = m_info.m_chLayout.nb_channels * m_upAudioManager->perSize();
	int newInSmp = m_numInSample / m_speed;
	m_bufferOutSamples.resize(m_numOutSample * base);
	for (int i = 0; i < newInSmp * base; ++i) //截断
	{
		m_bufferOutSamples.push_back(m_bufferInSamples[i]);
	}
	m_bufferOutSamples.push_back(0);

	m_numOutSample += newInSmp;
	m_bufferInSamples.clear();
	m_numInSample = 0;


	FrameSPtr res = this->createNewFrameBySample(&m_info, m_numOutSample);
	auto paf = this->getAVFrame(res.get());
	outputFrame(paf);

	paf->pts = m_queuePts.empty() ? m_lastEndPts : m_queuePts.front();
	if (!m_queuePts.empty())
		m_queuePts.pop();
	m_lastEndPts = paf->pts + m_duration;

	IAudioFrameProcessor::setFrameValid(res.get());

	return res;
}
