#include "interfaces.h"
#include "avoperator.h"

ITimeObserver::ITimeObserver(FAVPlayer* pObj, QObject* par) : QObject(par)
{
	connect(pObj, &FAVPlayer::secondChanged, this, &ITimeObserver::onSecondChanged);
	connect(pObj, &FAVPlayer::durationChanged, this, &ITimeObserver::onDurationChanged);
}
