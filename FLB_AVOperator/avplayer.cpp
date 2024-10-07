#include "avoperator.h"
#include "videoopenglplayer.h"
#include "audioprocess.h"
#include "avreader.h"
#include "playbtmbar.h"
#include "streamutils.h"

#include <QDebug>
#include <QDateTime>

FAVPlayer::FAVPlayer(QObject* p) : QObject(p),
m_spSpeedProc(std::make_shared<AudioSpeedProcessor>()), m_spListProc(std::make_shared<AudioListProcessor>())
{
	m_pVideoPlayer = dynamic_cast<VideoOpenGLPlayer*>(p);
	if (m_pVideoPlayer != nullptr)
	{
		connect(m_pVideoPlayer, &VideoOpenGLPlayer::videoEnd, this, &FAVPlayer::onVideoEnd);
		connect(this, &FAVPlayer::stopped, m_pVideoPlayer, &VideoOpenGLPlayer::onAVStop);
	}

	auto audioPlayer = AudioSDLPlayer::getInstance();
	connect(audioPlayer, &AudioSDLPlayer::audioEnd, this, &FAVPlayer::onAudioEnd);

	/*m_spReader->moveToThread(&m_threadDecode);

	m_spReader->playerSetHandle(this);*/
	m_spListProc->addProcessor(m_spSpeedProc);
	m_threadDecode.start();
}

FAVPlayer::~FAVPlayer()
{
	if (m_state != PlayState::Stop)
		this->stop();
	m_threadDecode.quit();
	m_threadDecode.wait();
}

bool FAVPlayer::openPath(const QString& path, const std::shared_ptr<FAVFileReader>& spReader)
{
	if (!spReader)
		return false;

	this->stop();
	m_spReader = spReader;
	m_spReader->playerSetHandle(this);
	m_spReader->setListProcessor(m_spListProc);
	m_spReader->moveToThread(&m_threadDecode);
	connect(this, &FAVPlayer::started, m_spReader.get(), &IAVReader::readFrames);

	if (!spReader->openPath(path))
	{
		qDebug() << __FUNCTION__ << path << " open failed!";
		return false;
	}

	auto info = m_spReader->getInfo();

	if (info->m_vIndx >= 0)
	{
		if (m_pVideoPlayer != nullptr)
		{
			m_pVideoPlayer->bindReader(m_spReader);
			/*if (!m_pVideoPlayer->initProcessor())
				return false;*/
		}
	}

	m_bEndStop = true;
	return true;
}

bool FAVPlayer::openFile(const QString& filePath)
{
	if (m_spReader && filePath == m_spReader->getInfo()->m_avPath && m_spReader->getInfo()->m_isOpen)
		return true;

	/*if (!QFile::exists(filePath))
		return false;*/

	if (!this->openPath(filePath, std::make_shared<FAVFileReader>()))
		return false;

	auto info = m_spReader->getInfo();

	m_state = PlayState::Pause;
	m_bAudioEnd = info->m_aIndx < 0;
	m_bVideoEnd = info->m_vIndx < 0;

	float speed = m_spSpeedProc->getSpeed();
	m_spSpeedProc->setAVInfo(*m_spReader->getInfo());

	if (m_pVideoPlayer)
	{
		auto btm = m_pVideoPlayer->getPlayBtmWidget();
		btm->onSeekAble(m_spReader->seekAble());
	}

	emit started();
	return true;
}

bool FAVPlayer::bindStreamManager(const std::shared_ptr<StreamManager>& spManager)
{
	auto spReader = std::make_shared<FAVStreamReader>(spManager);
	if (spReader && spManager)
	{
		m_spReader = spReader;
		m_spReader->moveToThread(&m_threadDecode);
		connect(this, &FAVPlayer::started, m_spReader.get(), &IAVReader::readFrames);
		connect(this, &FAVPlayer::secondChanged, spManager.get(), &StreamManager::setCurPlaySecond);
		connect(spManager.get(), &StreamManager::streamReqRefresh, this, &FAVPlayer::reseek);
		m_spReader->playerSetHandle(this);

		auto info = m_spReader->getInfo();
		m_state = PlayState::Pause;
		m_bEndStop = false;

		if (m_pVideoPlayer)
		{
			m_pVideoPlayer->removeBtmWidget();
			m_pVideoPlayer->bindReader(m_spReader);

			auto pvm = spManager->getVideoManager();
			pvm->installVideoPlayer(m_pVideoPlayer);
		}

		emit durationChanged(spManager->getSecEnd());
		emit started();
		return true;
	}
	return false;
}

//bool FAVPlayer::openUrl(const QString& urlPath)
//{
//	if (m_spReader && urlPath == m_spReader->getInfo()->m_avPath && m_spReader->getInfo()->m_isOpen)
//		return true;
//
//	if (!this->openPath(urlPath, std::make_shared<FAVUrlReader>()))
//		return false;
//
//	auto info = m_spReader->getInfo();
//
//	m_state = PlayState::Pause;
//	m_spSpeedProc->setSpeed(1.0f, 0);
//
//	if (m_pVideoPlayer)
//	{
//		auto btm = m_pVideoPlayer->getPlayBtmWidget();
//		btm->onSeekAble(m_spReader->seekAble());
//	}
//
//	emit started();
//	return true;
//}

void FAVPlayer::stop()
{
	m_state = PlayState::Stop;
	auto audioPlayer = AudioSDLPlayer::getInstance();

	audioPlayer->unBindAVPlayer(this);

	emit stopped();
	emit playStateChanged(m_state);

	m_curSecond = -1;
	m_lastCntTime = AV_NOPTS_VALUE;
	m_durationSecond = -1;
	m_bAudioEnd = true;
	m_bVideoEnd = true;

	if (m_spReader)
		m_spReader->stop();
	qDebug() << "播放器关闭！";
}

void FAVPlayer::pause()
{
	if (m_state == PlayState::Play)
	{
		m_state = PlayState::Pause;

		auto audioPlayer = AudioSDLPlayer::getInstance();
		audioPlayer->pause();

		if (m_pVideoPlayer != nullptr)
			m_pVideoPlayer->pause(true);

		this->getCurSecond(); //更新当前播放的时间
		emit playStateChanged(m_state);
	}
}

void FAVPlayer::playCont()
{
	if (m_state == PlayState::Stop)
	{
		if (!m_spReader)
			return;

		if (m_spReader->seekAble())
			emit seekProp(0);

		if (m_spReader->isStopped())
			emit started();
		m_state = PlayState::Pause;
	}

	if (m_state == PlayState::Pause)
	{
		m_state = PlayState::Play;

		if (m_pVideoPlayer != nullptr)
			m_pVideoPlayer->pause(false);
		m_lastCntTime = QDateTime::currentMSecsSinceEpoch();

		auto audioPlayer = AudioSDLPlayer::getInstance();
		audioPlayer->bindAVPlayer(this);
		audioPlayer->cont();
		emit playStateChanged(m_state);
	}
}

PlayState FAVPlayer::state()const
{
	return m_state;
}

double FAVPlayer::getCurSecond()const
{
	if (m_spReader->getInfo()->m_aIndx < 0 && m_state == PlayState::Play)
	{
		qint64 curMs = QDateTime::currentMSecsSinceEpoch();
		qint64 gap = curMs - m_lastCntTime;
		m_lastCntTime = curMs;

		m_curSecond += static_cast<double>(gap) / 1000 * m_spSpeedProc->getSpeed();
		emit secondChanged(m_curSecond);
		//qDebug() << gap;
	}
	return m_curSecond;
}

double FAVPlayer::getDuration()const
{
	return m_durationSecond;
}

bool FAVPlayer::getAudioEnd()const
{
	return m_bAudioEnd;
}

void FAVPlayer::addProcessor(const AProcessSPtr& sp)
{
	m_spListProc->addProcessor(sp);
}

float FAVPlayer::setSpeed(float speed)
{
	return m_spSpeedProc->setSpeed(speed, m_spReader->getInfo()->m_frameSize);
}

void FAVPlayer::reseek()
{
	if (m_durationSecond > 0)
	{
		double prop = m_curSecond / m_durationSecond;
		this->seekProp(prop);
	}
}

void FAVPlayer::seekProp(double p)
{
	if (!m_spReader->getInfo()->m_isOpen || m_state == PlayState::Stop || m_durationSecond <= 0 || !m_spReader->seekAble())
		return;

	p = std::max(0.0, p);
	p = std::min(1.0, p);

	auto sdlPlayer = AudioSDLPlayer::getInstance();
	sdlPlayer->pause();

	m_curSecond = m_durationSecond * p;
	this->setCurSecond(m_curSecond);
	if (m_pVideoPlayer != nullptr)
		m_pVideoPlayer->onSeek();

	emit seek(m_curSecond);

	m_bAudioEnd = false;
	if (m_state == PlayState::Play)
	{
		sdlPlayer->cont();
		if (m_pVideoPlayer != nullptr)
			m_pVideoPlayer->pause(false);
	}
	else if (m_state == PlayState::Pause)
	{
		if (m_pVideoPlayer != nullptr)
			m_pVideoPlayer->onPauseSeek();
	}
	if (m_pVideoPlayer != nullptr)
		m_pVideoPlayer->onSeekFinished();
}

void FAVPlayer::seekBackward()
{
	if (!m_spReader->getInfo()->m_isOpen || m_state == PlayState::Stop || m_durationSecond <= 0)
		return;

	double backs = m_curSecond - 2;
	m_curSecond = std::max(0.0, backs);
	this->seekProp(m_curSecond / m_durationSecond);
}

void FAVPlayer::seekForward()
{
	if (!m_spReader->getInfo()->m_isOpen || m_state == PlayState::Stop || m_durationSecond <= 0)
		return;

	double nxts = m_curSecond + 2;
	m_curSecond = std::min(m_durationSecond, nxts);
	this->seekProp(m_curSecond / m_durationSecond);
}

void FAVPlayer::onSeekFinished()
{

	if (m_state == PlayState::Play)
	{
		auto sdlPlayer = AudioSDLPlayer::getInstance();
		sdlPlayer->cont();
		m_pVideoPlayer->pause(false);
	}
}

void FAVPlayer::setCurSecond(double cs)
{
	if (cs >= 0)
	{
		m_curSecond = cs;
		emit secondChanged(m_curSecond);
	}
}

void FAVPlayer::setDuration(double d)
{
	if (d > 0)
	{
		m_durationSecond = d;
		emit durationChanged(m_durationSecond);
	}
}

void FAVPlayer::onAudioEnd(FAVPlayer* pPlayer)
{
	if (this == pPlayer)
	{
		m_bAudioEnd = true;
		this->check_stop();
	}
}

void FAVPlayer::onVideoEnd()
{
	m_bVideoEnd = true;
	this->check_stop();
}

void FAVPlayer::check_stop()
{
	if (m_bAudioEnd && m_bVideoEnd)
	{
		if (m_bEndStop)
			this->stop();
		else
			this->pause();
	}
}
