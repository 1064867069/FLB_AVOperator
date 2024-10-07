#include "interfaces.h"
#include "streamreaders.h"
#include "streamutils.h"
#include "audioprocess.h"
#include "streamwidgets.h"
#include "threadpool.h"

#include <qlogging.h>
#include <qdebug.h>
#include <QDateTime>

void IFAVStream::setStart(double s)
{
	m_timeStart = std::min(s, m_timeEnd);
	emit startChanged(m_timeStart);
}

void IFAVStream::setEnd(double e)
{
	m_timeEnd = std::max(e, m_timeStart);
	emit endChanged(m_timeEnd);
}

QBrush IFAVStream::backgroundBrush()const
{
	return "#ffffff";
}

double IFAVStream::startMin()const
{
	return 0;
}

double IFAVStream::endMax()const
{
	return 1e8;
}

void IFAVStream::moveHorizon(double gap)
{
	gap = std::max(gap, -m_timeStart);
	m_timeStart += gap;
	m_timeEnd += gap;

	emit endChanged(m_timeEnd);
}

void IFAVStream::bindStreamManager(const std::shared_ptr<StreamManager>& spManager)
{
	connect(this, &IFAVStream::needRefresh, spManager.get(), &StreamManager::streamReqRefresh);
	m_wpStreamManager = spManager;
}

void IFAVStream::connectBlock(AVStreamBlock* pBlock)
{

}

bool IFAVStream::valid()const
{
	return m_valid;
}

bool IFAVStream::inTimeRange(double sec)const
{
	return m_timeStart <= sec && m_timeEnd >= sec;
}

double IFAVStream::timeStart()const
{
	return m_timeStart;
}

double IFAVStream::timeEnd()const
{
	return m_timeEnd;
}

IAudioStream::IAudioStream() :
	m_upListProcs(std::make_unique<AudioListProcessor>()), m_spVolProcessor(std::make_shared<VolumnAdjustProcessor>())
{
	m_upListProcs->addProcessor(m_spVolProcessor);
}

void IAudioStream::setOutParam(int newOutRate, AVSampleFormat dstFmt, AVChannelLayout out_channel)
{
	if (m_inSampleRate <= 0 || m_inFormat == AV_SAMPLE_FMT_NONE)
	{
		m_upSwrProcessor.reset();
		return;
	}

	if (!m_upSwrProcessor)
	{
		m_upSwrProcessor = std::make_unique<AudioResampleProcessor>(m_inSampleRate, m_inFormat, m_inChannels,
			newOutRate, dstFmt, out_channel);
	}
	else
	{
		m_upSwrProcessor->resetOutParam(newOutRate, dstFmt, m_upSwrProcessor->getChannelLayoout());
	}
}

float IAudioStream::getVolProp()const
{
	return m_spVolProcessor->factor();
}

IVideoStream::IVideoStream()
{

}

IVideoStream::~IVideoStream()
{
	//delete m_pRectSetWidget;
	m_pRectSetWidget->deleteLater();
}

VideoStreamPtr IVideoStream::getSharedPointer()
{
	return shared_from_this();
}

void IVideoStream::connectBlock(AVStreamBlock* pBlock)
{
	connect(pBlock, &AVStreamBlock::pressed, this, &IVideoStream::onBlockPressed);
}

void IVideoStream::bindStreamManager(const std::shared_ptr<StreamManager>& spManager)
{
	if (!spManager)
		return;

	IFAVStream::bindStreamManager(spManager);
	this->initStreamRectWidget();

	auto pVideoManager = spManager->getVideoManager();
	this->bindVideoPlayer(pVideoManager->getVideoPlayer());

	connect(pVideoManager, &VideoStreamManager::streamEnterSetting, this, &IVideoStream::onStreamEnterSetting);
}

void IVideoStream::onOutParamUpdated()
{

}

FrameSPtr IVideoStream::swsObjFrame(const FrameSPtr& spf)
{
	if (!spf || !spf->isVideo())
		return spf;

	auto pf = spf->getAVFrame();
	struct SwsContext* swsCtx = sws_getContext(pf->width, pf->height, (AVPixelFormat)pf->format,
		m_width, m_height, m_pixelFormat,
		SWS_BILINEAR, nullptr, nullptr, nullptr
	);

	qDebug() << pf->width << pf->height << pf->format << " 重采样参数 基类";
	qDebug() << m_width << m_height << (int)m_pixelFormat << "目标参数";
	if (!swsCtx)
	{
		qDebug() << __FUNCTION__ << "swsCtx allocate failed.";
		return spf;
	}

	auto res = std::make_shared<FFrame>(m_pixelFormat, m_height, m_width, "");
	spf->swsVideoFrame(swsCtx, pf->height, res);

	// 释放 SwsContext
	sws_freeContext(swsCtx);

	return res;
}

void IVideoStream::initStreamRectWidget()
{
	if (m_pRectSetWidget)
		return;

	QRectF rect(0, 0, 1, 1);
	auto spManager = m_wpStreamManager.lock();
	VideoStreamManager* pvm = nullptr;
	if (spManager)
	{
		pvm = spManager->getVideoManager();
		float width = pvm->width(), height = pvm->height();
		rect = QRectF(xPos() / width, yPos() / height, this->width() / width, this->height() / height);
	}

	m_pRectSetWidget = new VideoStreamRectSetWidget(this->shared_from_this(), rect);
	connect(m_pRectSetWidget, &VideoStreamRectSetWidget::rectWillSet, this, &IVideoStream::StreamWillAdjust);
	connect(this, &IVideoStream::StreamWillAdjust, this, &IVideoStream::onWillAdjust);
	connect(m_pRectSetWidget, &VideoStreamRectSetWidget::setWillFinish, this, &IVideoStream::StreamAdjustWillFinished);
	connect(m_pRectSetWidget, &VideoStreamRectSetWidget::rectPropGeoChanged, this, &IVideoStream::onRectPropChanged);
	if (pvm)
	{
		connect(this, &IVideoStream::streamWillAdjustThis, pvm, &VideoStreamManager::onStreamWillSet);
	}
}

void IVideoStream::onWillAdjust()
{
	emit streamWillAdjustThis(this);
}

void IVideoStream::updateVersion()
{
	m_rectVersion = (m_rectVersion + 1) % 10000;
}

void IVideoStream::bindVideoPlayer(VideoOpenGLPlayer* pPlayer)
{
	m_pRectSetWidget->bindVideoPlayer(pPlayer);
}

void IVideoStream::widgetShowStreamFrame(const FrameSPtr& spFrame)
{
	if (spFrame)
	{
		m_pRectSetWidget->showFrame(spFrame);
	}
}

void IVideoStream::setPixelFormat(AVPixelFormat pixFmt)
{
	if (pixFmt != m_pixelFormat)
	{
		m_pixelFormat = pixFmt;
		this->onOutParamUpdated();
	}
}

void IVideoStream::setFrameRate(int fr)
{
	m_frameRate = fr;
}

void IVideoStream::setLevel(int level)
{
	m_level = level;
}

void IVideoStream::setRemoved(bool removed)
{
	m_bRemoved = removed;
	this->updateVersion();
	if (removed)
	{
		m_pRectSetWidget->hide();
	}
}

void IVideoStream::hideRectWidget()
{
	if (m_pRectSetWidget && !m_pRectSetWidget->isHidden())
		m_pRectSetWidget->hide();
}

void IVideoStream::cpyFrom(const IVideoStream* pStream)
{
	if (pStream)
	{
		m_xPos = pStream->m_xPos;
		m_yPos = pStream->m_yPos;
		m_rect = pStream->m_rect;
		m_level = pStream->m_level;
		m_width = pStream->m_width;
		m_height = pStream->m_height;
		m_bRemoved = pStream->m_bRemoved;
		m_frameRate = pStream->m_frameRate;
		m_rectVersion = pStream->m_rectVersion;

		this->onOutParamUpdated();
	}
}

bool IVideoStream::removed()const
{
	return m_bRemoved;
}

int IVideoStream::width()const
{
	return m_width;
}

int IVideoStream::height()const
{
	return m_height;
}

int IVideoStream::level()const
{
	return m_level;
}

int IVideoStream::version()const
{
	return m_rectVersion;
}

int IVideoStream::xPos()const
{
	return m_xPos;
}

int IVideoStream::yPos()const
{
	return m_yPos;
}

void IVideoStream::onRectPropChanged(const QRectF& rectProp)
{
	auto spManager = m_wpStreamManager.lock();
	if (spManager)
	{
		auto pVManager = spManager->getVideoManager();
		m_xPos = rectProp.x() * pVManager->width();
		m_yPos = rectProp.y() * pVManager->height();
		m_width = rectProp.width() * pVManager->width();
		m_height = rectProp.height() * pVManager->height();

		this->onOutParamUpdated();
		this->updateVersion();
	}
}

void IVideoStream::onBlockPressed()
{
	auto spManager = m_wpStreamManager.lock();
	if (spManager && this->inTimeRange(spManager->getSecCurPlay()) && m_pRectSetWidget->isHidden())
	{
		//emit StreamWillAdjust();
		m_pRectSetWidget->show();
		//m_pRectSetWidget->enterSetting();
	}
}

void IVideoStream::onStreamEnterSetting(IVideoStream* pStream)
{
	if (pStream != this)
	{
		this->hideRectWidget();
	}
}

FAVFileBaseStream::FAVFileBaseStream(const QString& fpath, AVMediaType type)
	:m_upFrameBuffer(std::make_unique<FAVFrameBuffer>(3)), m_filePath(fpath), m_pPacket(av_packet_alloc())
{
	this->openFile(fpath, type);
	if (!m_valid)
		return;
	m_bIsInited = true;
	this->seekSecond(0);
	auto spf = m_upFrameBuffer->frontFrame();
	if (spf && spf->getSecond() > 0)
	{
		this->setStart(spf->getSecond());
		m_decWholeStart = m_timeStart;
	}
}

FAVFileBaseStream::~FAVFileBaseStream()
{
	if (m_futureNxtFrame.valid())
		m_futureNxtFrame.wait();

	//av_packet_unref(m_pPacket);
	av_packet_free(&m_pPacket);

	if (m_pDecCtx != nullptr)
	{
		avcodec_close(m_pDecCtx); // 关闭音频解码器的实例
		avcodec_free_context(&m_pDecCtx); // 释放音频解码器的实例
	}

	if (m_pInFmtCtx != nullptr)
	{
		avformat_close_input(&m_pInFmtCtx);
	}
}

void FAVFileBaseStream::setStart(double s)
{
	s = std::min(s, m_timeEnd);

	double gap = std::min((m_timeStart - s), (m_decStart - m_decWholeStart));
	m_decStart -= gap;
	IFAVStream::setStart(m_timeStart - gap);
}

void FAVFileBaseStream::setEnd(double e)
{
	e = std::max(e, m_timeStart);

	double gap = std::min((e - m_timeEnd), (m_decWholeEnd - m_decEnd));
	m_decEnd += gap;
	IFAVStream::setEnd(m_timeEnd + gap);
}

FrameSPtr FAVFileBaseStream::readOneFrame()
{
	if (m_upFrameBuffer->isEmpty())
	{
		while (av_read_frame(m_pInFmtCtx, m_pPacket) >= 0)
		{
			if (m_pPacket->stream_index == m_streamIndex)
			{
				//qDebug() << "音频流！";
				this->decodePacket();

				if (!m_upFrameBuffer->isEmpty())
				{
					if (m_upFrameBuffer->frontFrame()->getEndSecond() < m_decStart)
					{
						m_upFrameBuffer->popFrame();
					}
					else
					{
						av_packet_unref(m_pPacket);
						break;
					}
				}
			}
			av_packet_unref(m_pPacket);
		}
	}
	/*{
		auto spf = m_upFrameBuffer->frontFrame();
		if (spf)
			qDebug() << m_streamIndex << "新帧时间：" << spf->getEndSecond();
	}*/

	FrameSPtr res = m_upFrameBuffer->popFrame();
	if (res)
	{
		m_decLastSec = res->getEndSecond();
		res->setPtsSecond(res->getSecond() - m_decStart + m_timeStart);
	}
	return res;
}

FrameSPtr FAVFileBaseStream::getNxtFrames()
{
	FrameSPtr res;
	if (m_futureNxtFrame.valid())
	{
		res = m_futureNxtFrame.get();
		if (res)
			m_futureNxtFrame = ThreadPool::getInstance().enqueue(&FAVFileBaseStream::readOneFrame, this);
	}

	return res;
}

StreamPtr FAVFileBaseStream::clone()const
{
	return nullptr;
}

double FAVFileBaseStream::startMin()const
{
	return std::max(0.0, m_timeStart - (m_decStart - m_decWholeStart));
}

double FAVFileBaseStream::endMax()const
{
	return m_timeEnd + (m_decWholeEnd - m_decEnd);
}

void FAVFileBaseStream::seekSecond(double seek)
{
	if (m_futureNxtFrame.valid())
		m_futureNxtFrame.wait();

	if (m_decEnd > 0)
	{
		seek = std::max((seek - m_timeStart + m_decStart), m_decStart);
	}

	if (seek >= 0)
	{
		{
			if (av_seek_frame(m_pInFmtCtx, m_streamIndex, (int64_t)(seek) / av_q2d(m_pSrcStream->time_base),
				AVSEEK_FLAG_BACKWARD) < 0 && seek > 0)
			{
				if (m_decEnd == 0)
					this->seekSecond(0);
				return;
			}

			avcodec_flush_buffers(m_pDecCtx);
			m_upFrameBuffer->clear();
		}

		if (m_bIsInited)
		{
			m_decLastSec = seek;
			auto spf = this->readOneFrame();
			while (spf && spf->getEndSecond() < seek)
				spf = this->readOneFrame();
			//qDebug() << "seek帧时间" << spf->getEndSecond();
			if (spf)
				m_upFrameBuffer->pushFrame(spf);
			m_futureNxtFrame = ThreadPool::getInstance().enqueue(&FAVFileBaseStream::readOneFrame, this);
		}
	}
}

double FAVFileBaseStream::secLastDec()const
{
	return m_timeStart + (m_decLastSec - m_decStart);
}

const AVStream* FAVFileBaseStream::avStream()const
{
	return m_pSrcStream;
}

const AVCodecContext* FAVFileBaseStream::avDecCtx()const
{
	return m_pDecCtx;
}

const QString& FAVFileBaseStream::getFilePath()const
{
	return m_filePath;
}

void FAVFileBaseStream::cpyBFSTime(const FAVFileBaseStream& other)
{
	m_decStart = other.m_decStart;
	m_decEnd = other.m_decEnd;
	m_timeStart = other.m_timeStart;
	m_timeEnd = other.m_timeEnd;

}

void FAVFileBaseStream::decodePacket()
{
	FrameSPtr pf = std::make_shared<FFrame>();
	// 发送压缩数据到解码器
	int ret = avcodec_send_packet(m_pDecCtx, m_pPacket);
	if (ret < 0) {
		qCritical() << "send packet occur error " << ret << '\n';
		return;
	}
	while (1) {
		// 从解码器实例获取还原后的数据帧
		ret = pf->decode(m_pDecCtx, m_pSrcStream);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			break;
		}
		else if (ret < 0) {
			qCritical() << "decode frame occur error " << ret << '\n';
			break;
		}

		m_upFrameBuffer->pushFrame(pf);
		pf = std::make_shared<FFrame>();
	}
}

void FAVFileBaseStream::openFile(const QString& fpath, AVMediaType type)
{
	m_pInFmtCtx = avformat_alloc_context(); // 输入文件的封装器实例
	// 打开音视频文件
	int ret = avformat_open_input(&m_pInFmtCtx, fpath.toStdString().data(), nullptr, nullptr);
	if (ret < 0) {
		qCritical() << "Can't open file " << fpath << '\n';
		return;
	}
	qInfo() << "Success open input_file " << fpath << '\n';
	// 查找音视频文件中的流信息
	ret = avformat_find_stream_info(m_pInFmtCtx, nullptr);
	if (ret < 0) {
		qCritical() << "Can't find stream information" << '\n';
		return;
	}

	// 找到音频流的索引
	int index = av_find_best_stream(m_pInFmtCtx, type, -1, -1, nullptr, 0);
	if (index >= 0) {
		m_pSrcStream = m_pInFmtCtx->streams[index];
		m_streamIndex = index;
		qDebug() << "帧数：" << m_pSrcStream->nb_frames;
		enum AVCodecID codec_id = m_pSrcStream->codecpar->codec_id;
		// 查找音频解码器
		AVCodec* codec = (AVCodec*)avcodec_find_decoder(codec_id);
		if (!codec) {
			qCritical() << "audio_codec not found" << '\n';
			return;
		}
		m_pDecCtx = avcodec_alloc_context3(codec); // 分配解码器的实例
		if (!m_pDecCtx) {
			//qCritical() << "audio_decode_ctx is null" << '\n';
			return;
		}
		// 把音频流中的编解码参数复制给解码器的实例
		avcodec_parameters_to_context(m_pDecCtx, m_pSrcStream->codecpar);
		ret = avcodec_open2(m_pDecCtx, codec, nullptr); // 打开解码器的实例
		if (ret < 0) {
			//qCritical() << "Can't open audio_decode_ctx" << '\n';
			return;
		}

		m_valid = true;
		this->getEndTime();
		qDebug() << "成功打开文件！";
	}
}

void FAVFileBaseStream::getEndTime()
{
	if (m_streamIndex < 0 || !this->valid())
		return;

	auto duration = m_pSrcStream->duration;
	auto pts2sec = [](AVRational tb, int64_t d)->double {return d == AV_NOPTS_VALUE ? 0 : av_q2d(tb) * d; };
	double durSec = pts2sec(m_pSrcStream->time_base, m_pSrcStream->duration);

	this->seekSecond(durSec);

	FrameSPtr spf;
	while (av_read_frame(m_pInFmtCtx, m_pPacket) >= 0)
	{
		if (m_pPacket->stream_index == m_streamIndex)
		{
			duration = std::max(duration, m_pPacket->pts + m_pPacket->duration);
		}
		av_packet_unref(m_pPacket);
	}

	m_decEnd = m_decWholeEnd = pts2sec(m_pSrcStream->time_base, duration);
	m_timeEnd = m_decEnd;
	this->seekSecond(0);
}

AudioFileStream::AudioFileStream(const QString& filePath) :m_fileStream(filePath, AVMEDIA_TYPE_AUDIO)
{
	m_valid = m_fileStream.valid();

	if (m_valid)
	{
		this->setStart(m_fileStream.timeStart());
		this->setEnd(m_fileStream.timeEnd());

		auto pStream = m_fileStream.avStream();
		auto pCtx = m_fileStream.avDecCtx();
		m_inFormat = static_cast<AVSampleFormat>(pStream->codecpar->format);
		m_inChannels = pStream->codecpar->ch_layout;
		m_inSampleRate = pStream->codecpar->sample_rate;

		connect(&m_fileStream, &IFAVStream::startChanged, this, &IFAVStream::startChanged);
		connect(&m_fileStream, &IFAVStream::endChanged, this, &IFAVStream::endChanged);
		//connect(&m_fileStream, &IFAVStream::needRefresh, this, &IFAVStream::needRefresh);
	}
}

FrameSPtr AudioFileStream::getNxtFrames()
{
	auto spf = m_fileStream.getNxtFrames();
	return m_upSwrProcessor->processFrame(spf);
}


StreamPtr AudioFileStream::clone()const
{
	if (!m_valid)
		return nullptr;

	auto spStream = std::make_shared<AudioFileStream>(m_fileStream.getFilePath());
	if (!spStream->valid())
		return nullptr;

	spStream->m_fileStream.cpyBFSTime(m_fileStream);
	spStream->m_timeStart = spStream->m_fileStream.timeStart();
	spStream->m_timeEnd = spStream->m_fileStream.timeEnd();



	return spStream;
}

void AudioFileStream::setStart(double s)
{
	double preStart = m_timeStart;
	m_fileStream.setStart(s);
	m_timeStart = m_fileStream.timeStart();

	auto spManager = m_wpStreamManager.lock();
	if (spManager)
	{
		double curPlaySec = spManager->getSecCurPlay();
		if ((preStart > curPlaySec) ^ (m_timeStart > curPlaySec))
		{
			emit needRefresh();
		}
		else if (curPlaySec < preStart)
		{
			double secLastDec = m_fileStream.secLastDec();
			if (preStart < secLastDec || m_timeStart < secLastDec)
			{
				emit needRefresh();
			}
		}
	}
}

void AudioFileStream::setEnd(double e)
{
	double preEnd = m_timeEnd;
	m_fileStream.setEnd(e);
	m_timeEnd = m_fileStream.timeEnd();

	auto spManager = m_wpStreamManager.lock();
	if (spManager)
	{
		double curPlaySec = spManager->getSecCurPlay();
		if ((preEnd > curPlaySec) ^ (m_timeEnd > curPlaySec))
		{
			emit needRefresh();
		}
		else if (preEnd > m_timeEnd)
		{
			double lastDecSec = m_fileStream.secLastDec();
			if (m_timeEnd <= lastDecSec)
			{
				emit needRefresh();
			}
		}
	}

}

void AudioFileStream::moveHorizon(double gap)
{
	auto spManager = m_wpStreamManager.lock();
	bool preIn = false, curIn = false;
	if (spManager)
		preIn = this->inTimeRange(spManager->getSecCurPlay());

	m_fileStream.moveHorizon(gap);
	m_timeStart = m_fileStream.timeStart();
	m_timeEnd = m_fileStream.timeEnd();

	if (spManager)
		curIn = this->inTimeRange(spManager->getSecCurPlay());

	if (preIn ^ curIn)
		emit needRefresh();
}

void AudioFileStream::seekSecond(double sec)
{
	m_fileStream.seekSecond(sec);
}

double AudioFileStream::startMin()const
{
	return m_fileStream.startMin();
}

double AudioFileStream::endMax()const
{
	return m_fileStream.endMax();
}

QBrush AudioFileStream::backgroundBrush()const
{
	return "#0000ff";
}

void AudioFileStream::bindStreamManager(const std::shared_ptr<StreamManager>& spManager)
{
	m_fileStream.bindStreamManager(spManager);
	IFAVStream::bindStreamManager(spManager);
}

VideoFileStream::VideoFileStream(const QString& filePath) :m_fileStream(filePath, AVMEDIA_TYPE_VIDEO)
{
	m_valid = m_fileStream.valid();
	this->setStart(m_fileStream.timeStart());
	this->setEnd(m_fileStream.timeEnd());

	if (m_valid)
	{
		connect(&m_fileStream, &IFAVStream::startChanged, this, &IFAVStream::startChanged);
		connect(&m_fileStream, &IFAVStream::endChanged, this, &IFAVStream::endChanged);

		auto srcStream = m_fileStream.avStream();
		m_srcWidth = m_width = srcStream->codecpar->width;
		m_srcHeight = m_height = srcStream->codecpar->height;
		m_srcPixFormat = (AVPixelFormat)srcStream->codecpar->format;

	}
}

VideoFileStream::~VideoFileStream()
{
	if (!m_pSwsCtx)
	{
		sws_freeContext(m_pSwsCtx);
		m_pSwsCtx = nullptr;
	}
}

FrameSPtr VideoFileStream::getNxtFrames()
{
	/*if (!this->inTimeRange(m_second))
		return nullptr;*/
	double second = m_second;
	m_second += 1.0f / m_frameRate;

	if (!m_spNxtFrame || m_spNxtFrame == m_spCurFrame)
		m_spNxtFrame = m_fileStream.getNxtFrames();
	while (m_spNxtFrame && m_spNxtFrame->getEndSecond() < second)
		m_spNxtFrame = m_fileStream.getNxtFrames();

	//if (m_spNxtFrame)
	//	qDebug() << m_spNxtFrame->getEndSecond() << second;
	if (m_spCurFrame && m_spCurFrame->getEndSecond() >= second)
		return m_spCurFrame;
	else if (m_spNxtFrame && m_spNxtFrame->getSecond() <= second && m_spNxtFrame->getEndSecond() >= second)
	{
		m_spCurFrame = m_spNxtFrame;
		return m_spNxtFrame;
	}

	return m_spCurFrame;
}

StreamPtr VideoFileStream::clone()const
{
	if (!m_valid)
		return nullptr;

	auto spStream = std::make_shared<VideoFileStream>(m_fileStream.getFilePath());
	if (!spStream->valid())
		return nullptr;

	spStream->cpyFrom(this);
	spStream->m_fileStream.cpyBFSTime(m_fileStream);
	spStream->m_timeStart = spStream->m_fileStream.timeStart();
	spStream->m_timeEnd = spStream->m_fileStream.timeEnd();

	return spStream;
}

void VideoFileStream::setStart(double s)
{
	double preStart = m_timeStart;
	m_fileStream.setStart(s);
	m_timeStart = m_fileStream.timeStart();

	auto spManager = m_wpStreamManager.lock();
	if (spManager)
	{
		double curPlaySec = spManager->getSecCurPlay();
		if ((preStart > curPlaySec) ^ (m_timeStart > curPlaySec))
		{
			emit needRefresh();
		}
		else if (curPlaySec < preStart)
		{
			double secLastDec = m_fileStream.secLastDec();
			if (preStart < secLastDec || m_timeStart < secLastDec)
			{
				emit needRefresh();
			}
		}
		else if (curPlaySec < m_timeStart)
		{
			this->seekSecond(m_timeStart);
		}
	}
}

void VideoFileStream::setEnd(double e)
{
	double preEnd = m_timeEnd;
	m_fileStream.setEnd(e);
	m_timeEnd = m_fileStream.timeEnd();

	auto spManager = m_wpStreamManager.lock();
	if (spManager)
	{
		double curPlaySec = spManager->getSecCurPlay();
		if ((preEnd > curPlaySec) ^ (m_timeEnd > curPlaySec))
		{
			emit needRefresh();
		}
		else if (preEnd > m_timeEnd)
		{
			double lastDecSec = m_fileStream.secLastDec();
			if (m_timeEnd <= lastDecSec)
			{
				emit needRefresh();
			}
		}
	}

}

void VideoFileStream::moveHorizon(double gap)
{
	auto spManager = m_wpStreamManager.lock();
	bool preIn = false, curIn = false;
	if (spManager)
		preIn = this->inTimeRange(spManager->getSecCurPlay());

	m_fileStream.moveHorizon(gap);
	m_timeStart = m_fileStream.timeStart();
	m_timeEnd = m_fileStream.timeEnd();

	if (spManager)
		curIn = this->inTimeRange(spManager->getSecCurPlay());

	if (preIn ^ curIn)
		emit needRefresh();
	else if (spManager && m_timeStart >= spManager->getSecCurPlay())
		this->seekSecond(m_timeStart);
}

void VideoFileStream::seekSecond(double sec)
{
	m_second = std::max(sec, m_timeStart);
	m_second = std::min(m_second, m_timeEnd);
	m_fileStream.seekSecond(m_second);
	m_spCurFrame.reset();
	m_spNxtFrame.reset();
}

double VideoFileStream::startMin()const
{
	return m_fileStream.startMin();
}

double VideoFileStream::endMax()const
{
	return m_fileStream.endMax();
}

QBrush VideoFileStream::backgroundBrush()const
{
	return "#ff0000";
}

void VideoFileStream::bindStreamManager(const std::shared_ptr<StreamManager>& spManager)
{
	m_fileStream.bindStreamManager(spManager);
	IVideoStream::bindStreamManager(spManager);
}

void VideoFileStream::onOutParamUpdated()
{
	struct SwsContext* swsCtx = sws_getContext(m_srcWidth, m_srcHeight, m_srcPixFormat,
		m_width, m_height, m_pixelFormat,
		SWS_BILINEAR, nullptr, nullptr, nullptr
	);

	qDebug() << m_srcWidth << m_srcHeight << (int)m_srcPixFormat << " 重采样参数";
	qDebug() << m_width << m_height << (int)m_pixelFormat << "目标参数";
	if (!swsCtx) {
		qDebug() << "Error: Cannot initialize the conversion context.\n";
		return;
	}

	if (!m_pSwsCtx)
		sws_freeContext(m_pSwsCtx);
	m_pSwsCtx = swsCtx;
	//qDebug() << "swsContext赋值！";
}

#include <QThread>
FrameSPtr VideoFileStream::swsObjFrame(const FrameSPtr& spf)
{
	if (!spf || !spf->isVideo())
		return spf;

	if (!m_mutexSws.tryLock())
	{
		return m_pSwsCtx ? IVideoStream::swsObjFrame(spf) : spf;
	}

	auto res = spf;
	if (m_pSwsCtx)
	{
		res = std::make_shared<FFrame>(m_pixelFormat, m_height, m_width, "");
		spf->swsVideoFrame(m_pSwsCtx, m_srcHeight, res);
	}

	m_mutexSws.unlock();
	return res;
}