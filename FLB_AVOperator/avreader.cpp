#include "avoperator.h"

#include <QDebug>
#include <qlogging.h>


FAVFileReader::FAVFileReader(QObject* p) :QObject(p), m_upAudioBuffer(std::make_unique<FAVFrameBuffer>(1000)),
m_upVideoBuffer(std::make_unique<FAVFrameBuffer>(200))
{

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
		m_info.m_vIndx = video_index;
		m_info.m_avgfRate = av_q2d(m_procs.m_pSrcVideo->r_frame_rate);
		m_info.m_height = m_procs.m_pVDecCtx->height;
		m_info.m_width = m_procs.m_pVDecCtx->width;
		m_info.m_aspect_ratio = m_procs.m_pSrcVideo->codecpar->sample_aspect_ratio;
		m_info.m_pixFmt = m_procs.m_pVDecCtx->pix_fmt;
		qDebug() << "frame rate of video = " << m_info.m_avgfRate;
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
		m_info.m_aIndx = audio_index;
		m_info.m_sampleFmt = m_procs.m_pADecCtx->sample_fmt;
		m_info.m_chLayout = m_procs.m_pADecCtx->ch_layout;
		m_info.m_sampleRate = m_procs.m_pADecCtx->sample_rate;
		m_info.m_frameSize = m_procs.m_pADecCtx->frame_size;
		m_info.m_nChannel = m_info.m_chLayout.nb_channels;
	}

	if (m_info.m_aIndx < 0 && m_info.m_vIndx < 0)
	{
		qCritical() << "No audio and video stream!" << '\n';
		return false;
	}

	m_info.m_isOpen = true;
	return true;
}

const FAVInfo* FAVFileReader::getInfo()const
{
	return &m_info;
}

FrameSPtr FAVFileReader::popAudioFrame()
{
	return m_upAudioBuffer->popFrame();
}

FrameSPtr FAVFileReader::popVideoFrame()
{
	return m_upVideoBuffer->popFrame();
}

void FAVFileReader::stop()
{
	if (!m_stop)
	{
		{
			QMutexLocker locker(&m_mutex);
			m_stop = true;

			m_upAudioBuffer->clear();
			m_upVideoBuffer->clear();

			m_condStop.wait(&m_mutex);
		}


		this->reset();
	}
}

void FAVFileReader::reset()noexcept
{
	QMutexLocker locker(&m_mutex);
	m_procs.reset();
	m_info.reset();
}

void FAVFileReader::waitNotBeyond()
{
	if (m_upAudioBuffer->isBeyond() || m_upVideoBuffer->isBeyond())
	{
		m_upAudioBuffer->waitNotBeyond();
		m_upVideoBuffer->waitNotBeyond();


	}
}

void FAVFileReader::decodePacket(AVCodecContext* dec_ctx, AVPacket* pkt, AVStream* strm)
{
	BufferUPtr* uppBuffer = nullptr;
	if (pkt->stream_index == m_info.m_aIndx)
		uppBuffer = &m_upAudioBuffer;
	else if (pkt->stream_index == m_info.m_vIndx)
		uppBuffer = &m_upVideoBuffer;
	else
		return;

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
		(*uppBuffer)->pushFrame(pf);
		/*if (*uppBuffer == m_upVideoBuffer)
		{
			qDebug() << "插入视频帧的时间戳：" << pf->getSecond() << " pts = " << pf->getPts();
		}*/
		pf = std::make_shared<FFrame>();
	}
}

void FAVFileReader::readFrames()
{
	{
		QMutexLocker locker(&m_mutex);
		if (!m_info.m_isOpen)
		{
			qDebug() << "Try decoding while the av file is not open, exit!";
			return;
		}

		m_stop = false;
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

			if (packet->stream_index == m_info.m_aIndx)
			{
				this->decodePacket(m_procs.m_pADecCtx, packet, m_procs.m_pSrcAudio);
				//m_upAudioBuffer->pushFrame(pf);
			}
			else if (packet->stream_index == m_info.m_vIndx)
			{
				this->decodePacket(m_procs.m_pVDecCtx, packet, m_procs.m_pSrcVideo);
				//m_upVideoBuffer->pushFrame(pf);
			}


		}

		if (m_stop)
		{
			break;
		}
	}
	av_packet_free(&packet);

	QMutexLocker locker(&m_mutex);
	m_condStop.wakeAll();
}

