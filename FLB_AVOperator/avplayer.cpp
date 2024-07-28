#include "avoperator.h"
#include "videoopenglplayer.h"
#include "audioprocess.h"

#include <QDebug>
#include <QDateTime>

FAVPlayer::FAVPlayer(QObject* p) : QObject(p), m_spReader(std::make_shared<FAVFileReader>()),
m_spSpeedProc(std::make_shared<AudioSpeedProcessor>())
{
	m_pVideoPlayer = dynamic_cast<VideoOpenGLPlayer*>(p);
	if (m_pVideoPlayer != nullptr)
	{
		m_pVideoPlayer->bindReader(m_spReader);
		connect(m_pVideoPlayer, &VideoOpenGLPlayer::videoEnd, this, &FAVPlayer::onVideoEnd);
		connect(this, &FAVPlayer::stopped, m_pVideoPlayer, &VideoOpenGLPlayer::onAVStop);
	}

	m_spReader->addProcessor(m_spSpeedProc);

	auto audioPlayer = AudioSDLPlayer::getInstance();
	connect(audioPlayer, &AudioSDLPlayer::audioEnd, this, &FAVPlayer::onAudioEnd);

	m_spReader->moveToThread(&m_threadDecode);
	connect(this, &FAVPlayer::started, m_spReader.get(), &FAVFileReader::readFrames);
	connect(m_spReader.get(), &FAVFileReader::durationSecondChanged, this, &FAVPlayer::setDuration);
	connect(m_spReader.get(), &FAVFileReader::seekFinished, this, &FAVPlayer::onSeekFinished);
	m_threadDecode.start();
}

FAVPlayer::~FAVPlayer()
{
	if (m_state != PlayState::Stop)
		this->stop();
	m_threadDecode.quit();
	m_threadDecode.wait();
}

bool FAVPlayer::openFile(const QString& filePath)
{
	if (filePath == m_spReader->getInfo()->m_filePath && m_spReader->getInfo()->m_isOpen)
		return true;

	this->stop();
	if (!m_spReader->openFile(filePath))
	{
		qDebug() << __FUNCTION__ << filePath << " open failed!";
		return false;
	}

	auto info = m_spReader->getInfo();

	if (info->m_vIndx >= 0)
	{
		if (m_pVideoPlayer != nullptr)
		{
			if (!m_pVideoPlayer->initProcessor())
				return false;
		}
	}

	m_spSpeedProc->setSpeed(m_spSpeedProc->getSpeed(), info->m_aIndx >= 0 ? info->m_frameSize : 0);

	m_state = PlayState::Pause;
	m_audioEnd = info->m_aIndx < 0;
	m_videoEnd = info->m_vIndx < 0;

	emit started();
	return true;
}

void FAVPlayer::stop()
{
	m_state = PlayState::Stop;
	auto audioPlayer = AudioSDLPlayer::getInstance();

	audioPlayer->unBindAVPlayer(this);

	emit stopped();

	m_curSecond = -1;
	m_lastCntTime = AV_NOPTS_VALUE;
	m_durationSecond = -1;
	m_audioEnd = true;
	m_videoEnd = true;

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

	}
}

void FAVPlayer::playCont()
{
	if (m_state == PlayState::Stop)
	{
		this->openFile(m_spReader->getInfo()->m_filePath);
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
		qint64 gap = QDateTime::currentMSecsSinceEpoch() - m_lastCntTime;
		m_curSecond += static_cast<double>(gap) / 1000 * m_spSpeedProc->getSpeed();
	}
	return m_curSecond;
}

double FAVPlayer::getDuration()const
{
	return m_durationSecond;
}

bool FAVPlayer::getAudioEnd()const
{
	return m_audioEnd;
}

void FAVPlayer::addProcessor(const AProcessSPtr& sp)
{
	m_spReader->addProcessor(sp);
}

float FAVPlayer::setSpeed(float speed)
{
	return m_spSpeedProc->setSpeed(speed, m_spReader->getInfo()->m_frameSize);
}

void FAVPlayer::seekProp(double p)
{
	if (!m_spReader->getInfo()->m_isOpen || m_state == PlayState::Stop || m_durationSecond <= 0)
		return;

	p = std::max(0.0, p);
	p = std::min(1.0, p);

	auto sdlPlayer = AudioSDLPlayer::getInstance();
	sdlPlayer->pause();

	if (m_pVideoPlayer != nullptr)
		m_pVideoPlayer->onSeek();

	m_spReader->seekSecond(m_durationSecond * p);

	m_audioEnd = false;
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
		m_audioEnd = true;
		this->check_stop();
	}
}

void FAVPlayer::onVideoEnd()
{
	m_videoEnd = true;
	this->check_stop();
}

void FAVPlayer::check_stop()
{
	if (m_audioEnd && m_videoEnd)
		this->stop();
}
