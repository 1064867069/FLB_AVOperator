#include "interfaces.h"
#include "avoperator.h"

ITimeObserver::ITimeObserver(FAVPlayer* pObj, QObject* par) : QObject(par)
{
	connect(pObj, &FAVPlayer::secondChanged, this, &ITimeObserver::onSecondChanged);
	connect(pObj, &FAVPlayer::durationChanged, this, &ITimeObserver::onDurationChanged);
}

IAudioFrameProcessor::IAudioFrameProcessor(QObject* p) : QObject(p)
{

}

FrameSPtr IAudioFrameProcessor::getRestFrame(bool mustCmplete)
{
	return nullptr;
}

AVFrame* IAudioFrameProcessor::getAVFrame(FFrame* pf)
{
	return pf != nullptr ? pf->m_pFrame : nullptr;
}

void IAudioFrameProcessor::setFrameValid(FFrame* pf)
{
	if (pf != nullptr)
		pf->m_valid = true;
}

FrameSPtr IAudioFrameProcessor::createNewFrameBySample(const FAVInfo* pInfo, int nb_samples)
{
	if (pInfo->m_aIndx < 0)
		return FrameSPtr();

	nb_samples = nb_samples <= 0 ? pInfo->m_frameSize : nb_samples;

	FrameSPtr spf = std::make_shared<FFrame>();
	AVFrame* paf = this->getAVFrame(spf.get());

	paf->ch_layout = pInfo->m_chLayout;
	paf->nb_samples = nb_samples;
	paf->sample_rate = pInfo->m_sampleRate;
	paf->format = pInfo->m_sampleFmt;
	paf->time_base = pInfo->m_timeBaseAudio;

	return av_frame_get_buffer(paf, 0) < 0 ? FrameSPtr() : spf;
}

IVideoFrameProcessor::IVideoFrameProcessor(QObject* p) : QObject(p)
{

}


void IVideoFrameProcessor::setFrameValid(FFrame* pf)
{
	if (pf != nullptr)
		pf->m_valid = true;
}

AVFrame* IVideoFrameProcessor::getAVFrame(FFrame* pf)
{
	return pf != nullptr ? pf->m_pFrame : nullptr;
}