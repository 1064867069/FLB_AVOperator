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

AVFrame* IAudioFrameProcessor::getAVFrame(FFrame* pf)
{
	return pf->m_pFrame;
}

void IAudioFrameProcessor::setFrameValid(FFrame* pf)
{
	if (pf != nullptr)
		pf->m_valid = true;
}
