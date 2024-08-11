#include "avoperator.h"
#include "avutils.h"
#include "audioprocess.h"
#include "videoprocess.h"

#include <QDebug>
#include <qlogging.h>


FAVFileReader::FAVFileReader(QObject* p) :QObject(p), m_upAudioBuffer(std::make_unique<FAVFrameBuffer>(10)),
m_upVideoBuffer(std::make_unique<FAVFrameBuffer>(5)), m_spInfo(std::make_shared<FAVInfo>())
{
	m_upAudioProcessors = std::make_unique<AudioListProcessor>();
}

FAVFileReader::~FAVFileReader()
{
	qDebug() << "Reader析构！";
}

bool FAVFileReader::openFile(const QString& fpath)
{
	this->reset();

	m_procs.m_pInFmtCtx = avformat_alloc_context(); // 输入文件的封装器实例
	// 打开音视频文件
	int ret = avformat_open_input(&m_procs.m_pInFmtCtx, fpath.toStdString().data(), nullptr, nullptr);
	if (ret < 0) {
		qCritical() << "Can't open file " << fpath << '\n';
		return false;
	}
	qInfo() << "Success open input_file " << fpath << '\n';
	// 查找音视频文件中的流信息
	ret = avformat_find_stream_info(m_procs.m_pInFmtCtx, nullptr);
	if (ret < 0) {
		qCritical() << "Can't find stream information" << '\n';
		return false;
	}

	m_procs.m_pVDecCtx = nullptr; // 视频解码器的实例
	m_procs.m_pSrcVideo = nullptr;
	// 找到视频流的索引
	int video_index = av_find_best_stream(m_procs.m_pInFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	if (video_index >= 0) {
		m_procs.m_pSrcVideo = m_procs.m_pInFmtCtx->streams[video_index];
		qDebug() << "视频帧数：" << m_procs.m_pSrcVideo->nb_frames;
		enum AVCodecID video_codec_id = m_procs.m_pSrcVideo->codecpar->codec_id;
		// 查找视频解码器
		AVCodec* video_codec = (AVCodec*)avcodec_find_decoder(video_codec_id);
		if (!video_codec) {
			qCritical() << "video_codec not found" << '\n';
			return false;
		}
		m_procs.m_pVDecCtx = avcodec_alloc_context3(video_codec); // 分配解码器的实例
		if (!m_procs.m_pVDecCtx) {
			qCritical() << "video_decode_ctx is null" << '\n';
			return false;
		}
		// 把视频流中的编解码参数复制给解码器的实例
		avcodec_parameters_to_context(m_procs.m_pVDecCtx, m_procs.m_pSrcVideo->codecpar);
		ret = avcodec_open2(m_procs.m_pVDecCtx, video_codec, nullptr); // 打开解码器的实例
		if (ret < 0) {
			qCritical() << "Can't open video_decode_ctx" << '\n';
			return false;
		}
		m_spInfo->m_vIndx = video_index;
		m_spInfo->m_avgfRate = av_q2d(m_procs.m_pSrcVideo->r_frame_rate);
		m_spInfo->m_height = m_procs.m_pVDecCtx->height;
		m_spInfo->m_width = m_procs.m_pVDecCtx->width;
		m_spInfo->m_aspect_ratio = m_procs.m_pSrcVideo->codecpar->sample_aspect_ratio;
		m_spInfo->m_pixFmt = m_procs.m_pVDecCtx->pix_fmt;
		m_spInfo->m_timeBaseVideo = m_procs.m_pSrcVideo->time_base;
		qDebug() << "frame rate of video = " << m_spInfo->m_avgfRate;

		m_vDuration = m_procs.m_pSrcVideo->duration;
		//v_dur = av_q2d(m_procs.m_pSrcVideo->time_base) * m_vDuration;
	}
	// 找到音频流的索引
	int audio_index = av_find_best_stream(m_procs.m_pInFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	if (audio_index >= 0) {
		m_procs.m_pSrcAudio = m_procs.m_pInFmtCtx->streams[audio_index];
		qDebug() << "音频帧数：" << m_procs.m_pSrcAudio->nb_frames;
		enum AVCodecID audio_codec_id = m_procs.m_pSrcAudio->codecpar->codec_id;
		// 查找音频解码器
		AVCodec* audio_codec = (AVCodec*)avcodec_find_decoder(audio_codec_id);
		if (!audio_codec) {
			qCritical() << "audio_codec not found" << '\n';
			return false;
		}
		m_procs.m_pADecCtx = avcodec_alloc_context3(audio_codec); // 分配解码器的实例
		if (!m_procs.m_pADecCtx) {
			qCritical() << "audio_decode_ctx is null" << '\n';
			return false;
		}
		// 把音频流中的编解码参数复制给解码器的实例
		avcodec_parameters_to_context(m_procs.m_pADecCtx, m_procs.m_pSrcAudio->codecpar);
		ret = avcodec_open2(m_procs.m_pADecCtx, audio_codec, nullptr); // 打开解码器的实例
		if (ret < 0) {
			qCritical() << "Can't open audio_decode_ctx" << '\n';
			return false;
		}
		m_spInfo->m_aIndx = audio_index;
		m_spInfo->m_sampleFmt = m_procs.m_pADecCtx->sample_fmt;
		m_spInfo->m_chLayout = m_procs.m_pADecCtx->ch_layout;
		m_spInfo->m_sampleRate = m_procs.m_pADecCtx->sample_rate;
		m_spInfo->m_frameSize = m_procs.m_pADecCtx->frame_size;
		m_spInfo->m_nChannel = m_spInfo->m_chLayout.nb_channels;
		m_spInfo->m_timeBaseAudio = m_procs.m_pSrcAudio->time_base;

		if (m_spInfo->m_frameSize <= 0)
			m_spInfo->m_frameSize = m_procs.m_pSrcAudio->codecpar->frame_size;

		m_aDuration = m_procs.m_pSrcAudio->duration;
		//a_dur = av_q2d(m_procs.m_pSrcAudio->time_base) * m_aDuration;
	}

	if (m_spInfo->m_aIndx < 0 && m_spInfo->m_vIndx < 0)
	{
		qCritical() << "No audio and video stream!" << '\n';
		return false;
	}

	m_spInfo->m_filePath = fpath;
	m_spInfo->m_isOpen = true;
	this->getPreciousDurationSecond();
	return true;
}

const FAVInfo* FAVFileReader::getInfo()const
{
	return m_spInfo.get();
}

FrameSPtr FAVFileReader::popAudioFrame()
{
	FrameSPtr res = m_upAudioProcessors->getRestFrame(true);
	if (res)
		return res;

	res = m_upAudioBuffer->popFrame();
	if (res)
		res = m_upAudioProcessors->processFrame(res);
	else if (this->decoding())
		res = m_upAudioProcessors->getRestFrame(true);
	else
	{
		do
		{
			res = m_upAudioProcessors->getRestFrame(false);
		} while (!res && !m_upAudioProcessors->lastNone());
	}

	return res;
	//	return m_upAudioBuffer->popFrame();
}

FrameSPtr FAVFileReader::popVideoFrame()
{
	return m_upVideoBuffer->popFrame();
}

void FAVFileReader::seekSecond(double sec)
{
	if (sec < 0)
		return;

	{
		QMutexLocker locker(&m_mutex);

		m_seekSecond = sec;
		m_upAudioBuffer->clear();
		m_upVideoBuffer->clear();
	}
}

void FAVFileReader::stop()
{
	if (!m_stop)
	{
		{
			QMutexLocker locker(&m_mutex);
			if (!m_stop)
			{
				m_stop = true;

				m_upAudioBuffer->clear();
				m_upVideoBuffer->clear();

				m_condStop.wait(&m_mutex);
			}
		}

		this->reset();
		qDebug() << "文件阅读器关闭！";
	}
}

double FAVFileReader::getPreciousDurationSecond()
{
	//目前预设该函数被调用时，并未处于解码状态
	if ((m_spInfo->m_aIndx < 0 && m_spInfo->m_vIndx < 0) || !m_spInfo->m_isOpen || m_decoding)
		return -1;

	auto pts2sec = [](AVRational tb, int64_t d)->double {return av_q2d(tb) * d; };
	double a_dur = m_spInfo->m_aIndx >= 0 ? pts2sec(m_procs.m_pSrcAudio->time_base, m_aDuration) : 0;
	double v_dur = m_spInfo->m_vIndx >= 0 ? pts2sec(m_procs.m_pSrcVideo->time_base, m_vDuration) : 0;

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
	while (av_read_frame(m_procs.m_pInFmtCtx, packet) >= 0)
	{
		if (packet->stream_index == m_spInfo->m_aIndx)
		{
			m_aDuration = std::max(m_aDuration, packet->pts + packet->duration);
			if (m_spInfo->m_frameSize <= 0)
			{
				this->decodePacket(m_procs.m_pADecCtx, packet, m_procs.m_pSrcAudio);
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

	}
	av_packet_free(&packet);
	a_dur = m_spInfo->m_aIndx >= 0 ? pts2sec(m_procs.m_pSrcAudio->time_base, m_aDuration) : 0;
	v_dur = m_spInfo->m_vIndx >= 0 ? pts2sec(m_procs.m_pSrcVideo->time_base, m_vDuration) : 0;
	emit durationSecondChanged(std::max(a_dur, v_dur));

	qDebug() << a_dur << v_dur;

	this->seekSecond(0);
	this->checkAndSeek();
	m_upAudioBuffer->clear();
	//m_seekSecond = -1;
	return std::max(a_dur, v_dur);
}

void FAVFileReader::reset()noexcept
{
	QMutexLocker locker(&m_mutex);
	m_procs.reset();
	m_spInfo->reset();

	m_seekSecond = -1;
	m_aDuration = 0;
	m_vDuration = 0;
	m_decoding = false;
}

bool FAVFileReader::decoding()const
{
	QMutexLocker locker(&m_mutex);
	return m_decoding;
}

void FAVFileReader::addAudioProcessor(AProcessSPtr sp)
{
	m_upAudioProcessors->addProcessor(sp);
}


void FAVFileReader::waitNotBeyond()
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

bool FAVFileReader::checkAndSeek()
{
	if (m_seekSecond >= 0)
	{
		QMutexLocker locker(&m_mutex);

		if (av_seek_frame(m_procs.m_pInFmtCtx, -1, m_seekSecond * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD) < 0)
		{
			m_seekSecond = -1;
			emit seekFinished();
			return false;
		}

		if (m_spInfo->m_aIndx >= 0)
			avcodec_flush_buffers(m_procs.m_pADecCtx);
		if (m_spInfo->m_vIndx >= 0)
			avcodec_flush_buffers(m_procs.m_pVDecCtx);

		m_seekDecodeSecond = m_seekSecond;
		m_seekSecond = -1;
		m_decoding = true;

		emit seekFinished();
		return true;
	}
	return false;
}

void FAVFileReader::decodePacket(AVCodecContext* dec_ctx, AVPacket* pkt, AVStream* strm)
{
	/*BufferUPtr* uppBuffer = nullptr;
	if (pkt->stream_index == m_spInfo->m_aIndx)
		uppBuffer = &m_upAudioBuffer;
	else if (pkt->stream_index == m_spInfo->m_vIndx)
		uppBuffer = &m_upVideoBuffer;
	else
		return;*/
	int index = pkt->stream_index;

	FrameSPtr pf = std::make_shared<FFrame>();
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

		if (m_seekSecond >= 0)
		{
			if (pf->getSecond() < m_seekDecodeSecond)
				continue;
			else if (index == m_spInfo->m_aIndx && index >= 0)
				m_seekDecodeSecond = -1;
		}

		if (index == m_spInfo->m_aIndx)
		{
			m_upAudioBuffer->pushFrame(pf);
		}
		else if (index == m_spInfo->m_vIndx)
		{
			m_upVideoBuffer->pushFrame(pf);
		}

		pf = std::make_shared<FFrame>();
	}
}

void FAVFileReader::readFrames()
{
	{
		QMutexLocker locker(&m_mutex);
		if (!m_spInfo->m_isOpen)
		{
			qDebug() << "Try decoding while the av file is not open, exit!";
			return;
		}

		m_stop = false;
		m_decoding = true;
	}

	qDebug() << "解码开始！";
	AVPacket* packet = av_packet_alloc(); // 分配一个数据包
	while (1)
	{
		while (av_read_frame(m_procs.m_pInFmtCtx, packet) >= 0 && !m_stop)
		{
			this->waitNotBeyond();

			if (m_stop)
				break;

			if (this->checkAndSeek())
				continue;

			if (packet->stream_index == m_spInfo->m_aIndx)
			{
				this->decodePacket(m_procs.m_pADecCtx, packet, m_procs.m_pSrcAudio);
				//m_upAudioBuffer->pushFrame(pf);
			}
			else if (packet->stream_index == m_spInfo->m_vIndx)
			{
				this->decodePacket(m_procs.m_pVDecCtx, packet, m_procs.m_pSrcVideo);
				//m_upVideoBuffer->pushFrame(pf);
			}
			av_packet_unref(packet);
		}

		if (m_stop)
		{
			m_decoding = false;
			break;
		}
		else if (checkAndSeek())
		{
			continue;
		}
		else
		{
			m_decoding = false;
			emit decodeEnd();
		}
	}
	av_packet_free(&packet);

	QMutexLocker locker(&m_mutex);
	m_condStop.wakeAll();
}

