#include "audioprocess.h"
#include "avoperator.h"
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

FrameSPtr AudioListProcessor::processFrame(FrameSPtr pf, FAVInfo* pInfo)
{
	if (!pf || pInfo->m_aIndx < 0)
		return pf;

	for (auto& cur : m_listProcessors)
	{
		pf = cur->processFrame(pf, pInfo);
	}

	return pf;
}

VolumnAdjustProcessor::VolumnAdjustProcessor(QObject* p) :IAudioFrameProcessor(p)
{

}

FrameSPtr VolumnAdjustProcessor::processFrame(FrameSPtr pf, FAVInfo* info)
{
	if (!pf || this->getAVFrame(pf.get())->nb_samples <= 0 || m_factor == 1.0)
		return pf;

	FrameSPtr res = std::make_shared<FFrame>(*pf);
	auto volFunc = audio::getVolAdjustFunc(info->m_sampleFmt);

	auto paf = this->getAVFrame(res.get());
	volFunc(paf, info, m_factor);

	return res;
}

void VolumnAdjustProcessor::onVolChanged(int v)
{
	m_factor = static_cast<double>(v) / 100;
	m_factor = std::max(0.0, m_factor);
	//m_factor = std::min(200.0, m_factor);
}


AudioSpeedProcessor::AudioSpeedProcessor(QObject* p) :IAudioFrameProcessor(p)
{

}

float AudioSpeedProcessor::setSpeed(float speed, int nb_sample)
{
	if (nb_sample > 0 && speed < nb_sample)
	{
		dst_sample = nb_sample / speed;
		m_speed = static_cast<float>(nb_sample) / dst_sample;
	}
	else
	{
		m_speed = speed;
	}

	return this->getSpeed();
}

float AudioSpeedProcessor::getSpeed()const
{
	return m_speed;
}

FrameSPtr AudioSpeedProcessor::processFrame(FrameSPtr pf, FAVInfo* pInfo)
{
	if (!pf || this->getAVFrame(pf.get())->nb_samples <= 0 || m_speed == 1.0)
		return pf;

	FrameSPtr res = std::make_shared<FFrame>();
	auto rsmpleFunc = audio::getSpeedResampleFunc(pInfo->m_sampleFmt);

	auto resf = this->getAVFrame(res.get()), srcf = this->getAVFrame(pf.get());
	resf->format = srcf->format;
	resf->sample_rate = srcf->sample_rate;
	resf->ch_layout = srcf->ch_layout;
	resf->channel_layout = srcf->channel_layout;
	resf->time_base = srcf->time_base;
	resf->nb_samples = dst_sample;
	resf->pts = srcf->pts;

	if (av_frame_get_buffer(resf, 0) < 0)
	{
		qDebug() << "Error: AVFrame could not get buffer.\n";
		av_frame_free(&resf);
		return FrameSPtr();
	}

	int halfSrcSample = srcf->nb_samples / 2;
	int data_size = av_get_bytes_per_sample(static_cast<AVSampleFormat>(resf->format));
	if (!av_sample_fmt_is_planar(pInfo->m_sampleFmt))
	{
		int ori_len = std::min(dst_sample, halfSrcSample) * resf->ch_layout.nb_channels * data_size;
		for (int i = 0; i < ori_len; ++i)
		{
			resf->data[0][i] = srcf->data[0][i];
		}

		if (halfSrcSample < dst_sample)
		{
			rsmpleFunc(&resf->data[0][ori_len], dst_sample - halfSrcSample,
				&srcf->data[0][ori_len], srcf->nb_samples - halfSrcSample, resf->ch_layout.nb_channels);
		}
	}
	else
	{
		for (int i = 0; i < resf->ch_layout.nb_channels; ++i)
		{
			int ori_len = std::min(dst_sample, halfSrcSample) * data_size;
			for (int j = 0; j < ori_len; ++j)
			{
				resf->data[i][j] = srcf->data[i][j];
			}

			if (halfSrcSample < dst_sample)
			{
				rsmpleFunc(&resf->data[i][ori_len], dst_sample - halfSrcSample,
					&srcf->data[i][ori_len], srcf->nb_samples - halfSrcSample, resf->ch_layout.nb_channels);
			}
		}
	}

	IAudioFrameProcessor::setFrameValid(res.get());
	return res;
}
