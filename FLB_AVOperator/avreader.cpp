#include "avoperator.h"
#include "avutils.h"
#include "avreader.h"
#include "audioprocess.h"
#include "videoprocess.h"
#include "streamutils.h"

#include <assert.h>
#include <QDebug>
#include <qlogging.h>
#include <QDateTime>

IAVReader::IAVReader(QObject* p) :QObject(p), m_upAudioBuffer(std::make_unique<FAVFrameBuffer>(10)),
m_upVideoBuffer(std::make_unique<FAVFrameBuffer>(5)), m_spInfo(std::make_shared<FAVInfo>())
{

}

const FAVInfo* IAVReader::getInfo()const
{
	return m_spInfo.get();
}

bool IAVReader::decoding()const
{
	return m_bDecoding;
}

bool IAVReader::seekAble()const
{
	return m_bSeekAble;
}

bool IAVReader::isStopped()const
{
	return m_bStop;
}

void IAVReader::addAudioProcessor(AProcessSPtr sp)
{
	m_spAudioProcessors->addProcessor(sp);
}

void IAVReader::setListProcessor(const std::shared_ptr<AudioListProcessor>& spListProc)
{
	m_spAudioProcessors = spListProc;
}

FrameSPtr IAVReader::popAudioFrame()
{
	if (!m_spAudioProcessors)
		return m_upAudioBuffer->popFrame();

	FrameSPtr res = m_spAudioProcessors->getRestFrame(true);
	if (res)
		return res;

	res = m_upAudioBuffer->popFrame();
	if (res)
		res = m_spAudioProcessors->processFrame(res);
	else if (this->decoding())
		res = m_spAudioProcessors->getRestFrame(true);
	else
	{
		do
		{
			res = m_spAudioProcessors->getRestFrame(false);
		} while (!res && !m_spAudioProcessors->lastNone());
	}

	return res;
	//	return m_upAudioBuffer->popFrame();
}

FrameSPtr IAVReader::popVideoFrame()
{
	auto res = m_upVideoBuffer->popFrame();
	/*if (res)
	{
		qDebug() << "帧弹出时间" << res->getSecond();
	}*/
	//qDebug() << "视频帧缓冲区满比例" << m_upVideoBuffer->fullProp();
	return res;
}

FrameSPtr IAVReader::frontVideoFrame()
{
	return m_upVideoBuffer->frontFrame();
}

void IAVReader::waitNotBeyond()
{
	while (m_upAudioBuffer->isBeyond() || m_upVideoBuffer->isBeyond())
	{
		//若另一个缓冲区空了，先break

		if (m_spInfo->m_vIndx < 0 || !m_upVideoBuffer->isEmpty())
			m_upAudioBuffer->waitNotBeyond();
		else
			break;

		if (m_spInfo->m_aIndx < 0 || !m_upAudioBuffer->isEmpty())
			m_upVideoBuffer->waitNotBeyond();
		else
			break;

	}
}

void IAVReader::decodePacket(AVCodecContext* dec_ctx, AVPacket* pkt, AVStream* strm)
{
	int index = pkt->stream_index;

	FrameSPtr pf = std::make_shared<FFrame>(); //std::make_shared<FFrame>();
	// 发送压缩数据到解码器
	int ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0) {
		qCritical() << "send packet occur error " << ret << '\n';
		return;
	}
	while (1) {
		// 从解码器实例获取还原后的数据帧
		ret = pf->decode(dec_ctx, strm);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			break;
		}
		else if (ret < 0) {
			qCritical() << "decode frame occur error " << ret << '\n';
			break;
		}

		/*if (m_seekSecond >= 0)
		{
			if (pf->getSecond() < m_seekDecodeSecond)
				continue;
			else if (index == m_spInfo->m_aIndx && index >= 0)
				m_seekDecodeSecond = -1;
		}*/
		if (!chkFrameCondValid(pf.get()))
			continue;

		if (index == m_spInfo->m_aIndx)
		{
			qDebug() << "得到音频帧" << pf->getSecond();
			m_upAudioBuffer->pushFrame(pf);
		}
		else if (index == m_spInfo->m_vIndx)
		{
			qDebug() << "得到视频帧" << pf->getSecond();
			m_upVideoBuffer->pushFrame(pf);
		}

		pf = std::make_shared<FFrame>();
	}
}

void IAVReader::stop()
{
	if (!m_bStop)
	{
		{
			QMutexLocker locker(&m_mutex);
			if (!m_bStop)
			{
				m_bStop = true;

				m_upAudioBuffer->clear();
				m_upVideoBuffer->clear();

				m_condStop.wait(&m_mutex);
			}
		}

		this->reset();
		qDebug() << "文件阅读器关闭！";
	}
}

void IAVReader::reset()noexcept
{
	QMutexLocker locker(&m_mutex);
	m_spInfo->reset();

	m_bDecoding = false;
}

FAVFileReader::FAVFileReader(QObject* p) :IAVReader(p), m_upProcs(std::make_unique<FAVProcessors>())
{

}

FAVFileReader::~FAVFileReader()
{
	qDebug() << "Reader析构！";
}

void FAVFileReader::playerSetHandle(FAVPlayer* pPlayer)
{
	connect(this, &FAVFileReader::durationSecondChanged, pPlayer, &FAVPlayer::setDuration);
	connect(this, &FAVFileReader::seekFinished, pPlayer, &FAVPlayer::onSeekFinished);
	connect(pPlayer, &FAVPlayer::seek, this, &FAVFileReader::seekSecond, Qt::DirectConnection);
}

bool FAVFileReader::openPath(const QString& fpath)
{
	this->reset();

	m_upProcs->m_pInFmtCtx = avformat_alloc_context(); // 输入文件的封装器实例
	// 打开音视频文件
	int ret = avformat_open_input(&m_upProcs->m_pInFmtCtx, fpath.toStdString().data(), nullptr, nullptr);
	if (ret < 0) {
		qCritical() << "Can't open file " << fpath << '\n';
		return false;
	}
	qInfo() << "Success open input_file " << fpath << '\n';
	// 查找音视频文件中的流信息
	ret = avformat_find_stream_info(m_upProcs->m_pInFmtCtx, nullptr);
	if (ret < 0) {
		qCritical() << "Can't find stream information" << '\n';
		return false;
	}

	m_upProcs->m_pVDecCtx = nullptr; // 视频解码器的实例
	m_upProcs->m_pSrcVideo = nullptr;
	// 找到视频流的索引
	int video_index = av_find_best_stream(m_upProcs->m_pInFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	if (video_index >= 0) {
		m_upProcs->m_pSrcVideo = m_upProcs->m_pInFmtCtx->streams[video_index];
		qDebug() << "视频帧数：" << m_upProcs->m_pSrcVideo->nb_frames;
		enum AVCodecID video_codec_id = m_upProcs->m_pSrcVideo->codecpar->codec_id;
		// 查找视频解码器
		AVCodec* video_codec = (AVCodec*)avcodec_find_decoder(video_codec_id);
		if (!video_codec) {
			qCritical() << "video_codec not found" << '\n';
			return false;
		}
		m_upProcs->m_pVDecCtx = avcodec_alloc_context3(video_codec); // 分配解码器的实例
		if (!m_upProcs->m_pVDecCtx) {
			qCritical() << "video_decode_ctx is null" << '\n';
			return false;
		}
		// 把视频流中的编解码参数复制给解码器的实例
		avcodec_parameters_to_context(m_upProcs->m_pVDecCtx, m_upProcs->m_pSrcVideo->codecpar);
		ret = avcodec_open2(m_upProcs->m_pVDecCtx, video_codec, nullptr); // 打开解码器的实例
		if (ret < 0) {
			qCritical() << "Can't open video_decode_ctx" << '\n';
			return false;
		}
		m_spInfo->m_vIndx = video_index;
		m_spInfo->m_avgfRate = av_q2d(m_upProcs->m_pSrcVideo->r_frame_rate);
		m_spInfo->m_height = m_upProcs->m_pVDecCtx->height;
		m_spInfo->m_width = m_upProcs->m_pVDecCtx->width;
		m_spInfo->m_aspect_ratio = m_upProcs->m_pSrcVideo->codecpar->sample_aspect_ratio;
		m_spInfo->m_pixFmt = m_upProcs->m_pVDecCtx->pix_fmt;
		m_spInfo->m_timeBaseVideo = m_upProcs->m_pSrcVideo->time_base;
		qDebug() << "frame rate of video = " << m_spInfo->m_avgfRate;

		m_vDuration = m_upProcs->m_pSrcVideo->duration;
		//v_dur = av_q2d(m_upProcs->m_pSrcVideo->time_base) * m_vDuration;
	}
	// 找到音频流的索引
	int audio_index = av_find_best_stream(m_upProcs->m_pInFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	if (audio_index >= 0) {
		m_upProcs->m_pSrcAudio = m_upProcs->m_pInFmtCtx->streams[audio_index];
		qDebug() << "音频帧数：" << m_upProcs->m_pSrcAudio->nb_frames;
		enum AVCodecID audio_codec_id = m_upProcs->m_pSrcAudio->codecpar->codec_id;
		// 查找音频解码器
		AVCodec* audio_codec = (AVCodec*)avcodec_find_decoder(audio_codec_id);
		if (!audio_codec) {
			qCritical() << "audio_codec not found" << '\n';
			return false;
		}
		m_upProcs->m_pADecCtx = avcodec_alloc_context3(audio_codec); // 分配解码器的实例
		if (!m_upProcs->m_pADecCtx) {
			qCritical() << "audio_decode_ctx is null" << '\n';
			return false;
		}
		// 把音频流中的编解码参数复制给解码器的实例
		avcodec_parameters_to_context(m_upProcs->m_pADecCtx, m_upProcs->m_pSrcAudio->codecpar);
		ret = avcodec_open2(m_upProcs->m_pADecCtx, audio_codec, nullptr); // 打开解码器的实例
		if (ret < 0) {
			qCritical() << "Can't open audio_decode_ctx" << '\n';
			return false;
		}
		m_spInfo->m_aIndx = audio_index;
		m_spInfo->m_sampleFmt = m_upProcs->m_pADecCtx->sample_fmt;
		m_spInfo->m_chLayout = m_upProcs->m_pADecCtx->ch_layout;
		m_spInfo->m_sampleRate = m_upProcs->m_pADecCtx->sample_rate;
		m_spInfo->m_frameSize = m_upProcs->m_pADecCtx->frame_size;
		m_spInfo->m_nChannel = m_spInfo->m_chLayout.nb_channels;
		m_spInfo->m_timeBaseAudio = m_upProcs->m_pSrcAudio->time_base;

		if (m_spInfo->m_frameSize <= 0)
			m_spInfo->m_frameSize = m_upProcs->m_pSrcAudio->codecpar->frame_size;

		m_aDuration = m_upProcs->m_pSrcAudio->duration;
		//a_dur = av_q2d(m_upProcs->m_pSrcAudio->time_base) * m_aDuration;
	}

	if (m_spInfo->m_aIndx < 0 && m_spInfo->m_vIndx < 0)
	{
		qCritical() << "No audio and video stream!" << '\n';
		return false;
	}

	QFile file(fpath);
	m_bSeekAble = file.exists() || (m_aDuration != AV_NOPTS_VALUE) || (m_vDuration != AV_NOPTS_VALUE);

	m_spInfo->m_avPath = fpath;
	m_spInfo->m_isOpen = true;
	this->getPreciousDurationSecond();
	return true;
}

void FAVFileReader::reset()noexcept
{
	QMutexLocker locker(&m_mutex);
	m_upProcs->reset();
	m_spInfo->reset();

	m_bDecoding = false;
}

void FAVFileReader::seekSecond(double sec)
{
	if (sec < 0 || !this->seekAble())
		return;

	{
		QMutexLocker locker(&m_mutex);

		m_seekSecond = sec;
		m_upAudioBuffer->clear();
		m_upVideoBuffer->clear();
	}
}

double FAVFileReader::getPreciousDurationSecond()
{
	//目前预设该函数被调用时，并未处于解码状态
	if ((m_spInfo->m_aIndx < 0 && m_spInfo->m_vIndx < 0) || !m_spInfo->m_isOpen || m_bDecoding)
		return -1;

	if (!this->seekAble())
	{
		return 0;
	}

	auto pts2sec = [](AVRational tb, int64_t d)->double {return d == AV_NOPTS_VALUE ? 0 : av_q2d(tb) * d; };
	double a_dur = m_spInfo->m_aIndx >= 0 ? pts2sec(m_upProcs->m_pSrcAudio->time_base, m_aDuration) : 0;
	double v_dur = m_spInfo->m_vIndx >= 0 ? pts2sec(m_upProcs->m_pSrcVideo->time_base, m_vDuration) : 0;

	this->seekSecond(std::max(a_dur, v_dur));
	if (!this->checkAndSeek())
	{
		//无法跳到最后，那么可能真实较短
		this->seekSecond(0);
		this->checkAndSeek();
	}

	AVPacket* packet = av_packet_alloc(); // 分配一个数据包
	m_aDuration = 0;
	m_vDuration = 0;
	while (av_read_frame(m_upProcs->m_pInFmtCtx, packet) >= 0)
	{
		if (packet->stream_index == m_spInfo->m_aIndx)
		{
			m_aDuration = std::max(m_aDuration, packet->pts + packet->duration);
			if (m_spInfo->m_frameSize <= 0)
			{
				this->decodePacket(m_upProcs->m_pADecCtx, packet, m_upProcs->m_pSrcAudio);
				if (!m_upAudioBuffer->isEmpty())
				{
					auto spf = m_upAudioBuffer->popFrame();
					m_spInfo->m_frameSize = spf->getAVFrame()->nb_samples;
				}
			}
		}
		else
		{
			m_vDuration = std::max(m_vDuration, packet->pts + packet->duration);
		}
		av_packet_unref(packet);
	}
	av_packet_free(&packet);
	a_dur = m_spInfo->m_aIndx >= 0 ? pts2sec(m_upProcs->m_pSrcAudio->time_base, m_aDuration) : 0;
	v_dur = m_spInfo->m_vIndx >= 0 ? pts2sec(m_upProcs->m_pSrcVideo->time_base, m_vDuration) : 0;
	emit durationSecondChanged(std::max(a_dur, v_dur));

	qDebug() << a_dur << v_dur;

	this->seekSecond(0);
	this->checkAndSeek();
	m_upAudioBuffer->clear();
	//m_seekSecond = -1;
	return std::max(a_dur, v_dur);
}

bool FAVFileReader::checkAndSeek()
{
	if (!this->seekAble())
		return false;

	if (m_seekSecond >= 0)
	{
		QMutexLocker locker(&m_mutex);

		if (av_seek_frame(m_upProcs->m_pInFmtCtx, -1, m_seekSecond * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD) < 0)
		{
			m_seekSecond = -1;
			emit seekFinished();
			return false;
		}

		if (m_spInfo->m_aIndx >= 0)
			avcodec_flush_buffers(m_upProcs->m_pADecCtx);
		if (m_spInfo->m_vIndx >= 0)
			avcodec_flush_buffers(m_upProcs->m_pVDecCtx);

		m_seekDecodeSecond = m_seekSecond;
		m_seekSecond = -1;
		m_bDecoding = true;

		emit seekFinished();
		return true;
	}
	return false;
}

bool FAVFileReader::chkFrameCondValid(const FFrame* pf)
{
	if (m_seekDecodeSecond >= 0)
	{
		if (pf->getSecond() < m_seekDecodeSecond)
			return false;
		else if (pf->isAudio() && m_spInfo->m_aIndx >= 0)
			m_seekDecodeSecond = -1;
	}
	return true;
}

void FAVFileReader::readFrames()
{
	{
		QMutexLocker locker(&m_mutex);
		if (!m_spInfo->m_isOpen)
		{
			qDebug() << "Try decoding while the av file is not open, exit!";
			emit decInited();
			return;
		}

		m_bStop = false;
		m_bDecoding = true;
		emit decInited();
	}

	qDebug() << "解码开始！";
	AVPacket* packet = av_packet_alloc(); // 分配一个数据包
	while (1)
	{
		while (av_read_frame(m_upProcs->m_pInFmtCtx, packet) >= 0 && !m_bStop)
		{
			this->waitNotBeyond();

			if (m_bStop)
				break;

			if (this->checkAndSeek())
				continue;

			if (packet->stream_index == m_spInfo->m_aIndx)
			{
				this->decodePacket(m_upProcs->m_pADecCtx, packet, m_upProcs->m_pSrcAudio);
				//m_upAudioBuffer->pushFrame(pf);
			}
			else if (packet->stream_index == m_spInfo->m_vIndx)
			{
				this->decodePacket(m_upProcs->m_pVDecCtx, packet, m_upProcs->m_pSrcVideo);
				//m_upVideoBuffer->pushFrame(pf);
			}
			av_packet_unref(packet);
		}

		if (m_bStop)
		{
			m_bDecoding = false;
			break;
		}
		else if (checkAndSeek())
		{
			continue;
		}
		else
		{
			m_bDecoding = false;
			emit decodeEnd();
		}
	}
	av_packet_free(&packet);

	QMutexLocker locker(&m_mutex);
	m_condStop.wakeAll();
}

FAVStreamReader::FAVStreamReader(const std::shared_ptr<StreamManager>& spStreamManager)
	:m_spStreamManager(spStreamManager)
{
	assert(m_spStreamManager);
	m_spInfo = spStreamManager->getInfo();
	m_bSeekAble = true;

	connect(m_spStreamManager.get(), &StreamManager::endChanged,
		this, &FAVStreamReader::durationSecondChanged, Qt::DirectConnection);
	connect(m_spStreamManager.get(), &StreamManager::abandoned, this, &IAVReader::stop);
}

void FAVStreamReader::readFrames()
{
	{
		QMutexLocker locker(&m_mutex);
		if (!m_spInfo->m_isOpen)
		{
			qDebug() << "Try decoding while the av file is not open, exit!";
			m_bDecoding = false;
			emit decInited();
			return;
		}

		m_bStop = false;
		m_bDecoding = true;
		emit decInited();
	}

	double preDur = m_spStreamManager->getSecEnd(), curDur;
	while (!m_bStop)
	{
		curDur = m_spStreamManager->getSecEnd();
		if (preDur != curDur)
		{
			preDur = curDur;
			m_bSeekAble = curDur > 0;
			emit durationSecondChanged(curDur);
		}

		this->waitNotBeyond(); //while (m_upAudioBuffer->isBeyond());
		m_spStreamManager->waitStream();
		if (m_bStop)
			break;

		FrameSPtr spf;
		if (m_spInfo->m_aIndx >= 0
			&& (m_spInfo->m_vIndx < 0 || m_upAudioBuffer->fullProp() <= m_upVideoBuffer->fullProp()))
		{
			spf = m_spStreamManager->readAudioFrame();
			if (spf)
			{
				m_upAudioBuffer->pushFrame(spf);
				//qDebug() << (int)spf->flag();
				//qDebug() << "音频帧" << spf->getEndSecond();
			}
			else if (m_spStreamManager->isEnd())
			{
				m_bDecoding = false;
				continue;
			}
		}
		else if (m_spInfo->m_vIndx >= 0)
		{
			//auto ms = QDateTime::currentMSecsSinceEpoch();
			spf = m_spStreamManager->readVideoFrame();
			if (spf)
			{
				//auto ms2 = QDateTime::currentMSecsSinceEpoch();
				//qDebug() << "视频读帧耗时" << (ms2 - ms) << "毫秒";
				m_upVideoBuffer->pushFrame(spf);
				//qDebug() << "视频帧" << spf->getEndSecond();
			}
			else if (m_spStreamManager->isEnd())
			{
				m_bDecoding = false;
				continue;
			}
		}
		//qDebug() << m_upAudioBuffer->fullProp() << m_upVideoBuffer->fullProp();
	}

	QMutexLocker locker(&m_mutex);
	m_bDecoding = false;
	m_condStop.wakeAll();
}

void FAVStreamReader::playerSetHandle(FAVPlayer* pPlayer)
{
	connect(this, &FAVStreamReader::durationSecondChanged, pPlayer, &FAVPlayer::setDuration, Qt::DirectConnection);
	connect(this, &FAVStreamReader::seekFinished, pPlayer, &FAVPlayer::onSeekFinished);
	connect(pPlayer, &FAVPlayer::seek, this, &FAVStreamReader::seekSecond, Qt::DirectConnection);

	if (m_spStreamManager->getSecEnd() > 0)
		pPlayer->setDuration(m_spStreamManager->getSecEnd());
}

void FAVStreamReader::seekSecond(double sec)
{
	QMutexLocker locker(&m_mutex);
	if (!m_spStreamManager->seekSecond(sec))
		return;

	m_upAudioBuffer->clear();
	m_upVideoBuffer->clear();
	m_upVideoBuffer->pushFrame(m_spStreamManager->lastFrame());
	emit seekFinished();
}
