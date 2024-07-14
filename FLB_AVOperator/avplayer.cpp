#include "avoperator.h"
#include "videoopenglplayer.h"

#include <QDebug>
#include <QDateTime>

FAVPlayer::FAVPlayer(QObject* p) : QObject(p), m_spReader(std::make_shared<FAVFileReader>())
{
	m_pVideoPlayer = dynamic_cast<VideoOpenGLPlayer*>(p);
	if (m_pVideoPlayer != nullptr)
	{
		m_pVideoPlayer->bindReader(m_spReader);
	}

	m_spReader->moveToThread(&m_threadDecode);
	connect(this, &FAVPlayer::started, m_spReader.get(), &FAVFileReader::readFrames);
	m_threadDecode.start();
}

FAVPlayer::~FAVPlayer()
{
	this->stop();
	m_threadDecode.quit();
	m_threadDecode.wait();
}

void FAVPlayer::openFile(const QString& filePath)
{
	if (filePath == m_spReader->getInfo()->m_filePath)
		return;

	this->stop();
	if (!m_spReader->openFile(filePath))
	{
		qDebug() << __FUNCTION__ << filePath << " open failed!";
		return;
	}


	//if (m_spReader->getInfo()->m_aIndx >= 0)
	//{
	//	auto audioPlayer = AudioSDLPlayer::getInstance();
	//	//audioPlayer->changeReader(m_spReader);
	//	if (!audioPlayer->audioOpen())
	//	{
	//		qDebug() << __FUNCTION__ << " audio player open failed!";
	//		return;
	//	}
	//}

	if (m_spReader->getInfo()->m_vIndx >= 0)
	{
		if (m_pVideoPlayer != nullptr)
		{
			if (!m_pVideoPlayer->initProcessor())
				return;
		}
	}


	emit started();
	m_state = PlayState::Pause;
}

void FAVPlayer::stop()
{
	m_state = PlayState::Stop;
	auto audioPlayer = AudioSDLPlayer::getInstance();

	audioPlayer->unBindAVPlayer(this);

	if (m_pVideoPlayer != nullptr)
		m_pVideoPlayer->pause(true);

	m_spReader->stop();

}

void FAVPlayer::pause(bool isPause)
{
	if (isPause)
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
	else
	{
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
		m_curSecond += static_cast<double>(gap) / 1000;
	}
	return m_curSecond;
}

void FAVPlayer::setCurSecond(double cs)
{
	if (cs >= 0)
	{
		m_curSecond = cs;
		emit secondChanged(m_curSecond);
	}
}
