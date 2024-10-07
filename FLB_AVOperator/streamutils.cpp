#include "streamutils.h"
#include "streamreaders.h"
#include "avutils.h"
#include "audiomanager.h"
#include "avoperator.h"
#include "videoopenglplayer.h"
#include "streamwidgets.h"

#include <queue>
#include <QDebug>
#include <QDateTime>

AudioStreamManager::AudioStreamManager(const std::shared_ptr<FAVInfo>& spInfo) :m_spInfo(spInfo), m_pEndStream(nullptr)
{
	AVChannelLayout layout;
	av_channel_layout_default(&layout, 2);

	this->setNumSample(1024);
	this->setChannelLayout(layout);
	this->setSampleRate(44100);
	this->setFormat(AV_SAMPLE_FMT_S16);
	this->resetFramePad();
}

bool AudioStreamManager::addStream(const StreamPtr& spStream)
{
	if (!spStream->valid())
		return false;

	auto spAudioStream = std::dynamic_pointer_cast<IAudioStream>(spStream);
	if (!spAudioStream || m_streams.contains(spAudioStream))
		return false;

	bool preEmpty = m_streams.isEmpty();
	spAudioStream->setOutParam(m_spInfo->m_sampleRate, m_spInfo->m_sampleFmt, m_spInfo->m_chLayout);
	spAudioStream->seekSecond(m_second);
	connect(spAudioStream.get(), &IFAVStream::endChanged, this, &AudioStreamManager::onStreamEndChanged);
	m_streams.insert(spAudioStream, {});
	if (preEmpty)
		m_spInfo->m_aIndx = 0;

	if (!m_pEndStream || m_pEndStream->timeEnd() < spAudioStream->timeEnd())
	{
		m_pEndStream = spAudioStream.get();
	}
	return true;
}

void AudioStreamManager::removeStream(const StreamPtr& spStream)
{
	auto spAudioStream = std::dynamic_pointer_cast<IAudioStream>(spStream);
	if (spAudioStream && m_streams.contains(spAudioStream))
	{
		spAudioStream->disconnect(this);
		m_streams.remove(spAudioStream);
		if (m_streams.isEmpty())
			m_spInfo->m_aIndx = -1;

		if (spAudioStream.get() == m_pEndStream)
			this->searchEndStream();
	}
}

void AudioStreamManager::seekSecond(double sec)
{
	m_second = std::max(sec, 0.0);
	m_pts = m_second * m_spInfo->m_sampleRate;
	for (auto itr = m_streams.begin(); itr != m_streams.end(); ++itr)
	{
		itr.key()->seekSecond(sec);
		itr.value().clear();
	}
}

void AudioStreamManager::setNumSample(int nbSample)
{
	m_spInfo->m_frameSize = nbSample;
	if (m_spFramePad)
		this->resetFramePad();
}

void AudioStreamManager::setChannelLayout(AVChannelLayout chLayout)
{
	m_spInfo->m_chLayout = chLayout;
	m_spInfo->m_nChannel = chLayout.nb_channels;
	if (m_spFramePad)
		this->resetFramePad();
}

void AudioStreamManager::setSampleRate(int sr)
{
	m_spInfo->m_sampleRate = sr;

	AVRational tb;
	tb.num = 1;
	tb.den = sr;
	m_spInfo->m_timeBaseAudio = tb;
	if (m_spFramePad)
		this->resetFramePad();
}

void AudioStreamManager::setFormat(AVSampleFormat format)
{
	m_spInfo->m_sampleFmt = format;
	if (m_spFramePad)
		this->resetFramePad();
}

FrameSPtr AudioStreamManager::readFrame()
{
	if (!m_spFramePad || !m_spFramePad->isAudio())
	{
		qDebug() << "Invalid pad frame, the parameter of frame is probably invalid!";
		return nullptr;
	}

	bool stop = false;
	do
	{
		FrameSPtr spf = this->getFrameByBuffer();
		if (spf)
			return spf;

		stop = true;
		for (auto itr = m_streams.begin(); itr != m_streams.end(); ++itr)
		{
			auto spStream = itr.key();
			if (!spStream->inTimeRange(m_second))
				continue;

			spf = itr.key()->getNxtFrames();
			if (!spf)
			{
				continue;
			}

			stop = false;
			//qDebug() << "音频帧原始时间" << spf->getEndSecond();
			spf->copy2AudioBufferPlanar(itr.value());
		}
	} while (!stop);

	auto res = m_spFramePad->deepAFClone();
	res->setPts(m_pts);
	this->timeMovByFrame(res);
	return res;
}

void AudioStreamManager::cpyStreamFrom(const AudioStreamManager& other)
{
	m_streams.clear();
	for (auto& spStream : other.m_streams.keys())
		this->addStream(spStream->clone());

	m_spInfo->m_aIndx = this->hasStream() ? 1 : -1;
}

void AudioStreamManager::searchEndStream()
{
	if (m_streams.isEmpty())
	{
		m_pEndStream = nullptr;
		return;
	}

	IAudioStream* pEndStream = m_streams.begin().key().get();
	for (auto itr = m_streams.begin(); itr != m_streams.end(); ++itr)
	{
		auto spStream = itr.key();
		if (spStream->timeEnd() > pEndStream->timeEnd())
			pEndStream = spStream.get();
	}
}

void AudioStreamManager::resetFramePad()
{
	m_spFramePad = std::make_shared<FFrame>(m_spInfo->m_sampleFmt, m_spInfo->m_chLayout, m_spInfo->m_sampleRate,
		m_spInfo->m_frameSize);
	m_spFramePad->setFlag(FrameFlag::PadFrame);
}

void AudioStreamManager::timeMovByFrame(const FrameSPtr& spf)
{
	if (spf && spf->valid())
	{
		auto pvf = spf->getAVFrame();
		m_pts += pvf->nb_samples;
		m_second = (double)m_pts / m_spInfo->m_sampleRate;
	}
}

FrameSPtr AudioStreamManager::getFrameByBuffer()
{
	QList<AudioStreamPtr> streamsWithData;
	auto amPtr = audio::getManagerPtr(m_spInfo->m_sampleFmt);
	int data_size = amPtr->perSize();
	int newSizePerChannel = m_spInfo->m_frameSize * data_size;
	int channel = 0;
	for (auto itr = m_streams.begin(); itr != m_streams.end(); ++itr)
	{
		const AudioStreamPtr& spAStream = itr.key();
		const auto& buffer = itr.value();
		if (buffer.size() > 0)
		{
			channel = std::max(channel, (int)buffer.size());
			if (buffer[0].size() < newSizePerChannel && spAStream->inTimeRange(m_second))
				return nullptr;
			streamsWithData.push_back(spAStream);
		}
	}

	if (channel < 1 || streamsWithData.size() == 0)
		return nullptr;

	//FrameSPtr res = std::make_shared<FFrame>(m_spInfo->m_sampleFmt, m_spInfo->m_chLayout, m_spInfo->m_sampleRate,
	//	m_spInfo->m_frameSize);
	FrameSPtr res = m_spFramePad->deepAFClone();
	if (!res || !res->valid())
		return nullptr;
	//qDebug() << "获取完毕！";
	for (auto& spStream : streamsWithData)
		//m_streams[spStream].clear();
		res->fromAudioBufferPlanar(m_streams[spStream], 1.0f / streamsWithData.size());

	res->setPts(m_pts);
	res->setFlag(FrameFlag::NormalFrame);
	this->timeMovByFrame(res);
	return res;
}

double AudioStreamManager::getCurSecond()const
{
	return m_second;
}

bool AudioStreamManager::hasStream()const
{
	return m_streams.size() > 0;
}

bool AudioStreamManager::isEnd()const
{
	return !this->hasStream() || m_pEndStream == nullptr || m_second >= m_pEndStream->timeEnd();
}

void AudioStreamManager::onStreamEndChanged(double d)
{
	auto streamSender = dynamic_cast<IAudioStream*>(sender());
	if (streamSender == nullptr)
		return;

	if (streamSender != m_pEndStream)
	{
		if (!m_pEndStream || d > m_pEndStream->timeEnd())
			m_pEndStream = streamSender;
	}
	else
	{
		this->searchEndStream();
	}
}

VideoStreamManager::VideoStreamManager(const std::shared_ptr<FAVInfo>& spInfo) : m_spInfo(spInfo), m_pEndStream(nullptr)
{
	m_spInfo->m_pixFmt = AV_PIX_FMT_YUV420P;
	this->setFrameRate(25);
}

void VideoStreamManager::installVideoPlayer(VideoOpenGLPlayer* pPlayer)
{
	if (pPlayer)
	{
		if (m_pVideoPlayer)
			this->disconnect(m_pVideoPlayer);

		m_pVideoPlayer = pPlayer;
		connect(pPlayer, &VideoOpenGLPlayer::willDestroy, this, &VideoStreamManager::unbindVPlayer);
	}
}

bool VideoStreamManager::addStream(const StreamPtr& spStream)
{
	if (!spStream->valid())
		return false;

	auto spVideoStream = std::dynamic_pointer_cast<IVideoStream>(spStream);
	if (!spVideoStream || m_streams.contains(spVideoStream))
		return false;

	spVideoStream->setFrameRate(m_spInfo->m_avgfRate);
	spVideoStream->setPixelFormat(m_spInfo->m_pixFmt);
	spVideoStream->seekSecond(m_second);
	m_streams.insert(spVideoStream);
	spVideoStream->setLevel(m_levelCnt++);
	spVideoStream->setRemoved(false);

	if (m_streams.size() == 1)
	{
		m_spInfo->m_width = spVideoStream->width();
		m_spInfo->m_height = spVideoStream->height();
		m_spInfo->m_vIndx = 0;
	}

	if (!m_pEndStream || m_pEndStream->timeEnd() < spVideoStream->timeEnd())
	{
		m_pEndStream = spVideoStream.get();
	}

	return true;
}

void VideoStreamManager::removeStream(const StreamPtr& spStream)
{
	auto spVideoStream = std::dynamic_pointer_cast<IVideoStream>(spStream);
	if (spVideoStream && m_streams.contains(spVideoStream))
	{
		m_streams.remove(spVideoStream);
		if (m_streams.isEmpty())
			m_spInfo->m_vIndx = -1;

		if (m_pEndStream == spVideoStream.get())
			this->searchEndStream();

		spVideoStream->setRemoved(true);
		emit streamRemoved(spVideoStream);
	}
}

void VideoStreamManager::seekSecond(double sec)
{
	double fdur = (1.0 / m_spInfo->m_avgfRate);
	int num = sec / fdur;
	sec = num * fdur;
	for (auto& spStream : m_streams)
		spStream->seekSecond(sec);
	m_second = sec;
	m_pts = std::round(m_second * m_spInfo->m_timeBaseVideo.den / m_spInfo->m_timeBaseVideo.num);
}

FrameSPtr VideoStreamManager::readFrame()
{
	std::function<bool(VideoStreamPtr, VideoStreamPtr)> compStream =
		[](VideoStreamPtr spStream1, VideoStreamPtr spStream2)
	{
		if (!spStream1)
			return true;
		if (!spStream2)
			return false;
		return spStream1->level() < spStream2->level();
	};

	double second = m_second;

	//std::priority_queue<VideoStreamPtr, QVector<VideoStreamPtr>, decltype(compStream)> minHeapStream(compStream);
	QList<VideoStreamPtr> srtStreams;
	for (const auto& spStream : m_streams)
	{
		//qDebug() << spStream.get();
		if (spStream && spStream->inTimeRange(second))
		{
			//minHeapStream.push(spStream);
			srtStreams.push_back(spStream);
		}
	}

	auto res = std::make_shared<VideoFrameSuperPosed>(m_spInfo->m_pixFmt, m_spInfo->m_height,
		m_spInfo->m_width);

	for (int i = 0; i < srtStreams.size(); ++i)
	{
		auto& spStream = srtStreams[i];
		//auto msBegin = QDateTime::currentMSecsSinceEpoch();
		auto spf = spStream->getNxtFrames();
		if (!spf)
			continue;
		//auto msEnd = QDateTime::currentMSecsSinceEpoch();
		//qDebug() << "目前耗时" << (msEnd - msBegin);
		res->addStreamFrame(spStream, spf);
	}

	res->redrawWhole();
	res->setTimeBase(1, m_spInfo->m_avgfRate);
	res->setPts(m_pts++);
	res->setDuration(1);
	m_second = res->getSecond();
	return res;
}

void VideoStreamManager::searchEndStream()
{
	if (m_streams.isEmpty())
	{
		m_pEndStream = nullptr;
		return;
	}

	m_pEndStream = m_streams.begin()->get();
	for (auto& spStream : m_streams)
	{
		if (spStream->timeEnd() > m_pEndStream->timeEnd())
			m_pEndStream = spStream.get();
	}
}

void VideoStreamManager::unbindVPlayer()
{
	if (m_pVideoPlayer)
	{
		this->disconnect(m_pVideoPlayer);
		m_pVideoPlayer = nullptr;
	}
}

void VideoStreamManager::setFrameRate(int f_rate)
{
	f_rate = std::max(1, f_rate);

	AVRational tb;
	tb.num = 1;
	tb.den = f_rate;
	m_spInfo->m_timeBaseVideo = tb;
	m_spInfo->m_avgfRate = f_rate;

	this->seekSecond(m_second);
}

double VideoStreamManager::getCurSecond()const
{
	return m_second;
}

VideoOpenGLPlayer* VideoStreamManager::getVideoPlayer()
{
	return m_pVideoPlayer;
}

int VideoStreamManager::width()const
{
	return m_spInfo->m_width;
}

int VideoStreamManager::height()const
{
	return m_spInfo->m_height;
}

bool VideoStreamManager::hasStream()const
{
	return m_streams.size() > 0;
}

bool VideoStreamManager::isEnd()const
{
	return !this->hasStream() || m_pEndStream == nullptr || m_second > m_pEndStream->timeEnd();
}

void VideoStreamManager::cpyStreamFrom(const VideoStreamManager& other)
{
	m_streams.clear();
	for (auto spStream : other.m_streams)
		this->addStream(spStream->clone());

	m_spInfo->m_aIndx = this->hasStream() ? 1 : -1;
}

void VideoStreamManager::onStreamWillSet(IVideoStream* pStream)
{
	if (m_spStreamSetting)
	{
		m_spStreamSetting->hideRectWidget();
		m_spStreamSetting.reset();
	}

	if (pStream)
		m_spStreamSetting = pStream->getSharedPointer();
}

void VideoStreamManager::onStreamWillSetDefault()
{
	this->onStreamWillSet();
}

//void VideoStreamManager::onStreamWillSet()
//{
//	if (m_pWidgetSetting)
//	{
//		m_pWidgetSetting->hide();
//	}
//
//	auto srcWidget = dynamic_cast<VideoStreamRectSetWidget*>(sender());
//	m_pWidgetSetting = srcWidget;
//}

std::shared_ptr<StreamManager> StreamManager::createManager()
{
	StreamManager* spManager = new StreamManager();
	return std::shared_ptr<StreamManager>(spManager);
}

int StreamManager::sizePerSecond()const
{
	return 5;
}

bool StreamManager::addStream(const StreamPtr& spStream)
{
	QMutexLocker lock(&m_mutex);
	if (m_bPause)
	{
		bool hs = hasStream();
		if (m_audioManager.addStream(spStream) || m_videoManager.addStream(spStream))
		{
			if (!hs)
			{
				m_condHasStream.wakeAll();
				m_spInfo->m_isOpen = true;
				emit hasStreamGotton();
			}

			return true;
		}
	}

	return false;
}

void StreamManager::removeStream(const StreamPtr& spStream)
{
	QMutexLocker lock(&m_mutex);
	m_audioManager.removeStream(spStream);
	m_videoManager.removeStream(spStream);

	if (!hasStream())
		m_spInfo->m_isOpen = false;

}

bool StreamManager::seekSecond(double sec)
{
	QMutexLocker lock(&m_mutex);
	if (!m_bPause)
		return false;

	m_videoManager.seekSecond(sec);
	m_audioManager.seekSecond(m_videoManager.getCurSecond());
	m_secCurDec = m_videoManager.getCurSecond();
	m_spLastFrame = m_videoManager.readFrame();
	return true;
}

FrameSPtr StreamManager::readAudioFrame()
{
	QMutexLocker lock(&m_mutex);
	if (!this->isEnd())
		return m_audioManager.readFrame();
	else
		return nullptr;
}

FrameSPtr StreamManager::readVideoFrame()
{
	QMutexLocker lock(&m_mutex); //return nullptr;
	if (m_videoManager.isEnd())
		return nullptr;
	//if (!m_bPause)
	{
		//auto ms = QDateTime::currentMSecsSinceEpoch();
		m_spLastFrame = m_videoManager.readFrame();
		//auto ms2 = QDateTime::currentMSecsSinceEpoch();
		//qDebug() << __FUNCTION__ << "持锁读取" << (ms2 - ms) << "毫秒";
	}
	return m_spLastFrame;
}

FrameSPtr StreamManager::lastFrame()
{
	QMutexLocker lock(&m_mutex);
	if (m_spLastFrame)
		return m_spLastFrame;
	else
		return this->readVideoFrame();
}

void StreamManager::start()
{
	m_bPause = false;
	emit stateChanged();
}

void StreamManager::stop()
{
	m_bPause = true;
	emit stateChanged();
}

void StreamManager::onEndChanged(double e)
{
	m_secEnd = e;
	emit endChanged(e);
}

void StreamManager::onPlayStateChanged(PlayState state)
{
	if (state == PlayState::Play)
		this->start();
	else
		this->stop();
}

void StreamManager::setCurPlaySecond(double sec)
{
	m_secCurPlay = sec;
}

void StreamManager::waitStream(unsigned long ms)
{
	QMutexLocker locker(&m_mutex);
	if (!hasStream())
	{
		m_condHasStream.wait(&m_mutex, ms);
	}
}

std::shared_ptr<StreamManager> StreamManager::clone()const
{
	auto res = StreamManager::createManager();
	if (!res)
		return res;

	auto srcInfo = this->getInfo();
	auto dstInfo = res->getInfo();
	*dstInfo = *srcInfo;

	res->m_audioManager.cpyStreamFrom(m_audioManager);
	res->m_videoManager.cpyStreamFrom(m_videoManager);

	dstInfo->m_width = srcInfo->m_width;
	dstInfo->m_height = srcInfo->m_height;
	res->m_secEnd = m_secEnd;
	return res;
}

VideoStreamManager* StreamManager::getVideoManager()
{
	return &m_videoManager;
}

std::shared_ptr<FAVInfo> StreamManager::getInfo()const
{
	return m_spInfo;
}

StreamManager::StreamManager() :m_spInfo(std::make_shared<FAVInfo>()), m_audioManager(m_spInfo), m_videoManager(m_spInfo)
{
	connect(this, &StreamManager::stateChanged, &m_videoManager, &VideoStreamManager::onStreamWillSetDefault);
	//connect(&m_videoManager, &VideoStreamManager::needRefresh, this, &StreamManager::streamReqRefresh,
	//	Qt::QueuedConnection);
	m_spInfo->m_isOpen = true;
}

bool StreamManager::hasStream()const
{
	return m_audioManager.hasStream() || m_videoManager.hasStream();
}

double StreamManager::getSecEnd()const
{
	return m_secEnd;
}

double StreamManager::getSecCurPlay()const
{
	return m_secCurPlay;
}

bool StreamManager::isEnd()const
{
	return m_audioManager.isEnd() && m_videoManager.isEnd();
}

bool StreamManager::isPaused()const
{
	return m_bPause;
}

QString ResourceFactoryFileAudio::fileFilter()
{
	return "*.mp3 *.wav *.aac *.flac";
}

QString ResourceFactoryFileAudio::title()
{
	return "音频";
}

QList<StreamPtr> ResourceFactoryFileAudio::createStreams(const QString& desc)
{
	auto afs = std::make_shared<AudioFileStream>(desc);
	if (afs && afs->valid())
		return { afs };
	return {};
}

QString ResourceFactoryFileVideo::fileFilter()
{
	return "*.mp4 *.mkv *.flv";
}

QString ResourceFactoryFileVideo::title()
{
	return "视频";
}

QList<StreamPtr> ResourceFactoryFileVideo::createStreams(const QString& desc)
{
	ResourceFactoryFileAudio audioFactry;
	QList<StreamPtr> res = audioFactry.createStreams(desc); //return res;
	res.append(std::make_shared<VideoFileStream>(desc));
	if (!res.back()->valid())
		res.pop_back();

	return res;
}

