#include "avfilewriter.h"
#include "avreader.h"
#include "avutils.h"
#include "audioprocess.h"

#include <assert.h>
#include <QDebug>
#include <QFile>

AVFileWriter::AVFileWriter(const ReaderSPtr& spReader, QObject* p) : QObject(nullptr), m_spReader(spReader)
{
	assert(m_spReader);
	m_spReader->moveToThread(&m_threadRead);
	this->moveToThread(&m_threadWrite);

	connect(m_spReader.get(), &IAVReader::decInited, this, &AVFileWriter::work, Qt::QueuedConnection);
	connect(this, &AVFileWriter::workBegin, m_spReader.get(), &IAVReader::readFrames, Qt::QueuedConnection);
	connect(p, &QObject::destroyed, this, &QObject::deleteLater);

	m_threadRead.start();
	m_threadWrite.start();
}

AVFileWriter::~AVFileWriter()
{
	this->stop();
	m_threadRead.wait();
	m_threadWrite.wait();
	m_threadRead.quit();
	m_threadWrite.quit();
}

bool AVFileWriter::writeFile(const QString& path, const QString& sizeStr)
{
	this->stop();
	m_pathOutput = path;
	if (!this->openOutputFile(sizeStr))
		return false;

	QMutexLocker locker(&m_mutex);
	m_state.store(WriterState::Working, std::memory_order_release);
	emit workBegin();
	return true;
}

WriterState AVFileWriter::getState()const
{
	return m_state.load(std::memory_order_acquire);
}

void AVFileWriter::stop()
{
	if (m_state.load(std::memory_order_acquire) != WriterState::Stop)
	{
		QMutexLocker locker(&m_mutex);
		if (m_state.load(std::memory_order_relaxed) != WriterState::Stop)
		{
			m_state.store(WriterState::Stop, std::memory_order_release);
			m_condWaitStopPause.wait(&m_mutex);
		}
	}

	m_spReader->stop();
	this->reset();
}

void AVFileWriter::pause(bool pause)
{
	if (pause && m_state.load(std::memory_order_acquire) == WriterState::Working)
	{
		QMutexLocker locker(&m_mutex);
		if (m_state.load(std::memory_order_relaxed) == WriterState::Working)
		{
			m_state.store(WriterState::Paused, std::memory_order_release);
		}
	}
	else if (!pause && m_state == WriterState::Paused)
	{
		QMutexLocker locker(&m_mutex);
		if (m_state.load(std::memory_order_relaxed) == WriterState::Paused)
			m_state.store(WriterState::Working, std::memory_order_release);
	}
}

namespace
{
	std::string toLowerCase(const std::string& str) {
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	bool isAudioFile(const std::string& back) {
		std::string extension = toLowerCase(back);

		return (extension == "mp3" || extension == "wav" || extension == "flac" ||
			extension == "aac" || extension == "ogg" || extension == "m4a" ||
			extension == "wma");
	}

	constexpr int g_defVideoIndex = 0;
	constexpr int g_defAudioIndex = 1;
}

bool AVFileWriter::openOutputFile(const QString& sizeStr)
{
	int idot = m_pathOutput.lastIndexOf(".");
	if (idot < 0)
		return false;

	QMutexLocker locker(&m_mutex);
	QString bckQs = m_pathOutput.right(m_pathOutput.size() - idot - 1);
	AVCodecID audioCodecId = audio::getAudioCodecID(bckQs);
	int cnt = 0;
	do
	{
		m_pathOutputTemp = m_pathOutput.left(idot) + QString::number(cnt++) + "temp." + bckQs;
	} while (QFile(m_pathOutputTemp).exists());

	std::string back = bckQs.toStdString();
	std::string fpath = m_pathOutputTemp.toStdString();

	// 初始化输出文件
	avformat_alloc_output_context2(&m_pFmtCtx, nullptr, back.c_str(), fpath.c_str());
	if (!m_pFmtCtx) {
		qDebug() << __FUNCTION__ << "Could not create output context\n";
		return false;
	}

	// 打开输出流
	int ret = avio_open(&m_pFmtCtx->pb, fpath.c_str(), AVIO_FLAG_READ_WRITE);
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "Can't open output_file %s.\n", fpath.c_str());
		this->reset();
		return false;
	}

	auto spInfo = m_spReader->getInfo();
	if (spInfo->m_aIndx < 0 && spInfo->m_vIndx < 0)
	{
		this->reset();
		return false;
	}

	if (spInfo->m_vIndx >= 0 && !isAudioFile(back))
	{
		// 打开视频流编码器
		const AVCodec* video_codec = avcodec_find_encoder(AV_CODEC_ID_H265);
		AVRational fr;
		fr.num = spInfo->m_avgfRate;
		fr.den = 1;

		int xi = sizeStr.indexOf("x");
		m_width = sizeStr.left(xi).toInt();
		m_height = sizeStr.right(sizeStr.size() - xi - 1).toInt();
		this->allocSwsCtx(m_width, m_height);

		m_pVideoStream = avformat_new_stream(m_pFmtCtx, video_codec);
		m_pVideoCodecCtx = avcodec_alloc_context3(video_codec);
		//m_pVideoCodecCtx->profile = FF_PROFILE_H265_HIGH;
		m_pVideoCodecCtx->codec_id = video_codec->id;
		m_pVideoCodecCtx->framerate = fr;
		m_pVideoCodecCtx->width = m_width;
		m_pVideoCodecCtx->height = m_height;
		m_pVideoCodecCtx->pix_fmt = spInfo->m_pixFmt;
		m_pVideoCodecCtx->time_base = spInfo->m_timeBaseVideo;
		m_pVideoCodecCtx->gop_size = 12;
		m_pVideoCodecCtx->max_b_frames = 0;
		m_pVideoCodecCtx->bit_rate = 4000000;
		m_pVideoCodecCtx->level = 40;

		// 打开编码器
		if (avcodec_open2(m_pVideoCodecCtx, video_codec, nullptr) < 0)
		{
			this->reset();
			qDebug() << "视频编码器初始化失败！";
			return false;
		}

		qDebug() << m_pVideoCodecCtx->time_base.num << m_pVideoCodecCtx->time_base.den;
		// 设置视频流
		//m_pVideoStream->time_base = m_pVideoCodecCtx->time_base;
		// 将编码器的参数复制到流的 codecpar 字段
		if (avcodec_parameters_from_context(m_pVideoStream->codecpar, m_pVideoCodecCtx) < 0) {
			qDebug() << "Could not copy codec parameters to video stream\n";
			this->reset();
			return false;
		}
		m_pVideoStream->codecpar->codec_tag = 0;
	}


	if (spInfo->m_aIndx >= 0 && audioCodecId != AV_CODEC_ID_NONE)
	{
		// 初始化音频流编码器
		const AVCodec* audio_codec = avcodec_find_encoder(audioCodecId);
		QList<AVSampleFormat> fmts = audio::getSupportedSampleFormats(audio_codec);
		if (fmts.isEmpty())
		{
			qDebug() << "Encoder has no supported sample formats!";
			this->reset();
			return false;
		}

		AVSampleFormat sampleFmt = spInfo->m_sampleFmt;
		if (std::find(fmts.begin(), fmts.end(), sampleFmt) == fmts.end())
			sampleFmt = fmts[0];

		m_spResampleProc = std::make_shared<AudioResampleProcessor>(spInfo->m_sampleRate, spInfo->m_sampleFmt,
			spInfo->m_chLayout, spInfo->m_sampleRate, sampleFmt, spInfo->m_chLayout);

		m_pAudioStream = avformat_new_stream(m_pFmtCtx, audio_codec);
		m_pAudioCodecCtx = avcodec_alloc_context3(audio_codec);
		m_pAudioCodecCtx->codec_id = audio_codec->id;
		m_pAudioCodecCtx->sample_rate = spInfo->m_sampleRate;
		m_pAudioCodecCtx->frame_size = spInfo->m_frameSize;
		m_pAudioCodecCtx->ch_layout = spInfo->m_chLayout;
		m_pAudioCodecCtx->channels = spInfo->m_chLayout.nb_channels;
		m_pAudioCodecCtx->sample_fmt = sampleFmt;
		m_pAudioCodecCtx->bit_rate = 128000;
		m_pAudioCodecCtx->time_base = spInfo->m_timeBaseAudio;

		if (avcodec_open2(m_pAudioCodecCtx, audio_codec, nullptr) < 0)
		{
			this->reset();
			qDebug() << "音频编码器初始化失败！";
			return false;
		}

		// 设置音频流
		m_pAudioStream->time_base = m_pAudioCodecCtx->time_base;
		if (avcodec_parameters_from_context(m_pAudioStream->codecpar, m_pAudioCodecCtx) < 0) {
			qDebug() << "Could not copy codec parameters to audio stream\n";
			this->reset();
			return false;
		}
	}

	if (m_pVideoCodecCtx == nullptr && m_pAudioCodecCtx == nullptr)
	{
		qDebug() << "Could not allocate context of encoding context!";
		this->reset();
		return false;
	}

	ret = avformat_write_header(m_pFmtCtx, NULL); // 写文件头
	if (ret < 0) {
		av_log(NULL, AV_LOG_ERROR, "write file_header occur error %d.\n", ret);
		this->reset();
		return false;
	}

	//this->reopenVCodecForVStream();
	return true;
}

void AVFileWriter::allocSwsCtx(int width, int height)
{
	this->freeSwsCtx();

	auto info = m_spReader->getInfo();
	if (width == info->m_width && height == info->m_height)
		return;

	struct SwsContext* swsCtx = sws_getContext(info->m_width, info->m_height, info->m_pixFmt,
		width, height, info->m_pixFmt,
		SWS_BILINEAR, nullptr, nullptr, nullptr
	);

	//qDebug() << m_srcWidth << m_srcHeight << (int)m_srcPixFormat << " 重采样参数";
	//qDebug() << m_width << m_height << (int)m_pixelFormat << "目标参数";
	if (!swsCtx) {
		qDebug() << "Error: Cannot initialize the conversion context.\n";
		return;
	}

	m_pSwsCtx = swsCtx;
}

void AVFileWriter::freeSwsCtx()
{
	if (!m_pSwsCtx)
		sws_freeContext(m_pSwsCtx);
	m_pSwsCtx = nullptr;
}

void AVFileWriter::work()
{
	if (m_pAudioCodecCtx == nullptr && m_pVideoCodecCtx == nullptr)
	{
		QMutexLocker locker(&m_mutex);
		m_state.store(WriterState::Stop, std::memory_order_release);
		return;
	}

	emit seek(0);
	if (m_pAudioCodecCtx && m_pVideoCodecCtx)
		this->workAudioVideoLoop();
	else if (!m_pVideoCodecCtx)
		this->workAudioLoop();
	else
		this->workVideoLoop();

	qDebug() << __FUNCTION__ << "编码循环结束！";
	QMutexLocker locker(&m_mutex);
	if (m_state.load(std::memory_order_acquire) != WriterState::Stop)
	{
		av_write_trailer(m_pFmtCtx);
		/*QFile file(m_pathOutput);
		if (file.exists())
			file.remove();

		file.setFileName(m_pathOutputTemp);
		file.rename(m_pathOutput);
		qDebug() << m_pathOutput << "转为" << m_pathOutputTemp;*/
		this->reset(m_pathOutput);
		m_state.store(WriterState::Stop, std::memory_order_release);
		emit finished();
	}
	else
	{
		QFile file(m_pathOutputTemp);
		if (file.exists())
			file.remove();

		m_condWaitStopPause.notify_all();
	}
}

void AVFileWriter::workAudioVideoLoop()
{
	FrameSPtr spAudioFrame, spVideoFrame;

	int ret;
	double sec = 0;
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = nullptr;
	packet.size = 0;
	int64_t dts_audio = -1, dts_video = -1;
	int64_t last_video_pts = -1;
	AVRational video_time_base = m_spReader->getInfo()->m_timeBaseVideo;
	while (m_state.load(std::memory_order_acquire) != WriterState::Stop)
	{
		if (m_state.load(std::memory_order_relaxed) == WriterState::Paused)
		{
			QThread::msleep(5);
			continue;
		}

		if (m_pAudioCodecCtx && !spAudioFrame)
			spAudioFrame = m_spReader->popAudioFrame();

		if (m_pVideoCodecCtx && !spVideoFrame)
			spVideoFrame = m_spReader->popVideoFrame();

		AVCodecContext* pEncCtx = nullptr;
		if (spAudioFrame && (!spVideoFrame || spVideoFrame->getSecond() > spAudioFrame->getSecond()))
		{
			spAudioFrame = m_spResampleProc->processFrame(spAudioFrame);
			sec = std::max(sec, spAudioFrame->getEndSecond());
			ret = spAudioFrame->send2Encoder(m_pAudioCodecCtx);
			if (ret >= 0)
			{
				spAudioFrame.reset();
				pEncCtx = m_pAudioCodecCtx;
			}
		}
		else if (spVideoFrame)
		{
			//qDebug() << "视频帧时间" << spVideoFrame->getSecond();
			//spVideoFrame->outPutImage(0, 70);

			if (m_pSwsCtx)
			{
				auto pf = spVideoFrame->getAVFrame();
				auto spf = std::make_shared<FFrame>((AVPixelFormat)pf->format, m_height, m_width);
				spVideoFrame->swsVideoFrame(m_pSwsCtx, pf->height, spf);
				spVideoFrame = spf;
			}

			if (spVideoFrame->getPts() <= last_video_pts)
				spVideoFrame->setPts(last_video_pts + 1);
			last_video_pts = spVideoFrame->getPts();

			sec = std::max(sec, spVideoFrame->getEndSecond());
			ret = spVideoFrame->send2Encoder(m_pVideoCodecCtx);
			if (ret >= 0)
			{
				spVideoFrame.reset();
				pEncCtx = m_pVideoCodecCtx;
			}
		}

		if (pEncCtx)
		{
			ret = avcodec_receive_packet(pEncCtx, &packet);
			if (pEncCtx == m_pAudioCodecCtx)
				packet.stream_index = m_pAudioStream->index;
			else if (pEncCtx == m_pVideoCodecCtx)
			{
				packet.stream_index = m_pVideoStream->index;
				packet.pts = av_rescale_q_rnd(packet.pts, video_time_base, m_pVideoStream->time_base,
					(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
				packet.dts = av_rescale_q_rnd(packet.dts, video_time_base, m_pVideoStream->time_base,
					(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
				packet.duration = av_rescale_q_rnd(1, video_time_base, m_pVideoStream->time_base,
					(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
				packet.time_base = m_pVideoStream->time_base;
			}
			if (ret >= 0) {
				//qDebug() << packet.duration << packet.stream_index;

				av_interleaved_write_frame(m_pFmtCtx, &packet);
				av_packet_unref(&packet);

				//qDebug() << (pEncCtx == m_pVideoCodecCtx ? "写入视频帧" : "写入音频帧");
			}
			else if (ret == AVERROR(EINVAL))
			{
				qDebug() << "编码器状态错误！";
			}
			emit curSecChanged(sec);
		}
		else if (!m_spReader->decoding())
		{
			break;
		}
	}

	avcodec_send_frame(m_pAudioCodecCtx, nullptr); // 发送结束信号
	while (avcodec_receive_packet(m_pAudioCodecCtx, &packet) == 0) {
		packet.stream_index = m_pAudioStream->index;
		av_interleaved_write_frame(m_pFmtCtx, &packet);
		av_packet_unref(&packet);
	}


	avcodec_send_frame(m_pVideoCodecCtx, nullptr); // 发送结束信号
	while (avcodec_receive_packet(m_pVideoCodecCtx, &packet) == 0) {
		packet.stream_index = m_pVideoStream->index;
		packet.pts = av_rescale_q_rnd(packet.pts, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.dts = av_rescale_q_rnd(packet.dts, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.duration = av_rescale_q_rnd(1, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.time_base = m_pVideoStream->time_base;

		av_interleaved_write_frame(m_pFmtCtx, &packet);
		av_packet_unref(&packet);
	}
}

void AVFileWriter::reopenVCodecForVStream()
{
	if (!m_pVideoStream || !m_pVideoStream)
		return;

	AVRational tbc = m_pVideoCodecCtx->time_base;
	AVRational tbs = m_pVideoStream->time_base;
	if (tbc.den == tbs.den && tbc.num == tbc.num)
		return;

	// 打开视频流编码器
	const AVCodec* video_codec = avcodec_find_encoder(AV_CODEC_ID_H264);

	AVCodecContext* newCtx = avcodec_alloc_context3(video_codec);
	newCtx->profile = m_pVideoCodecCtx->profile;
	newCtx->codec_id = m_pVideoCodecCtx->codec_id;
	newCtx->framerate = m_pVideoCodecCtx->framerate;
	newCtx->width = m_pVideoCodecCtx->width;
	newCtx->height = m_pVideoCodecCtx->height;
	newCtx->pix_fmt = m_pVideoCodecCtx->pix_fmt;
	newCtx->gop_size = m_pVideoCodecCtx->gop_size;
	newCtx->max_b_frames = m_pVideoCodecCtx->max_b_frames;
	newCtx->bit_rate = m_pVideoCodecCtx->bit_rate;
	newCtx->level = m_pVideoCodecCtx->level;
	newCtx->time_base = m_pVideoStream->time_base;

	// 打开编码器
	if (avcodec_open2(newCtx, video_codec, nullptr) < 0)
	{
		this->reset();
		qDebug() << "新视频编码器初始化失败！";
		return;
	}

	avcodec_free_context(&m_pVideoCodecCtx);
	m_pVideoCodecCtx = newCtx;
}

void AVFileWriter::workAudioLoop()
{
	if (!m_pAudioCodecCtx)
		return;

	FrameSPtr spAudioFrame;

	int ret;
	double sec = 0;
	AVPacket packet;
	av_init_packet(&packet);
	packet.data = nullptr;
	packet.size = 0;
	while (m_state.load(std::memory_order_acquire) != WriterState::Stop)
	{
		if (m_state.load(std::memory_order_relaxed) == WriterState::Paused)
		{
			QThread::msleep(5);
			continue;
		}

		spAudioFrame = m_spReader->popAudioFrame();
		m_spReader->popVideoFrame();
		if (spAudioFrame)
		{
			spAudioFrame = m_spResampleProc->processFrame(spAudioFrame);
			sec = std::max(sec, spAudioFrame->getEndSecond());
			ret = spAudioFrame->send2Encoder(m_pAudioCodecCtx);
			if (ret >= 0)
			{
				ret = avcodec_receive_packet(m_pAudioCodecCtx, &packet);
				if (ret >= 0) {
					// 将编码后的视频包写入输出文件
					packet.stream_index = m_pAudioStream->index;;
					av_interleaved_write_frame(m_pFmtCtx, &packet);
					av_packet_unref(&packet);
				}
			}
			emit curSecChanged(sec);
		}
		else if (!m_spReader->decoding())
		{
			break;
		}
	}

	avcodec_send_frame(m_pAudioCodecCtx, nullptr); // 发送结束信号
	while (avcodec_receive_packet(m_pAudioCodecCtx, &packet) == 0) {
		packet.stream_index = m_pAudioStream->index;
		av_interleaved_write_frame(m_pFmtCtx, &packet);
		av_packet_unref(&packet);
	}
}

void AVFileWriter::workVideoLoop()
{
	if (!m_pVideoCodecCtx)
		return;

	FrameSPtr spVideoFrame;

	int ret;
	int64_t last_video_pts = -1;
	double sec = 0;
	AVPacket packet;
	AVRational video_time_base = m_spReader->getInfo()->m_timeBaseVideo;

	av_init_packet(&packet);
	packet.data = nullptr;
	packet.size = 0;
	while (m_state.load(std::memory_order_acquire) != WriterState::Stop)
	{
		if (m_state.load(std::memory_order_relaxed) == WriterState::Paused)
		{
			QThread::msleep(5);
			continue;
		}

		spVideoFrame = m_spReader->popVideoFrame();
		m_spReader->popAudioFrame();
		if (spVideoFrame)
		{
			if (m_pSwsCtx)
			{
				auto pf = spVideoFrame->getAVFrame();
				auto spf = std::make_shared<FFrame>((AVPixelFormat)pf->format, m_height, m_width);
				spVideoFrame->swsVideoFrame(m_pSwsCtx, pf->height, spf);
				spVideoFrame = spf;
			}

			if (spVideoFrame->getPts() <= last_video_pts)
				spVideoFrame->setPts(last_video_pts + 1);
			last_video_pts = spVideoFrame->getPts();//lame\lib\*.lib

			sec = std::max(sec, spVideoFrame->getEndSecond());
			ret = spVideoFrame->send2Encoder(m_pVideoCodecCtx);
			if (ret >= 0)
			{
				ret = avcodec_receive_packet(m_pVideoCodecCtx, &packet);
				if (ret >= 0) {
					// 将编码后的视频包写入输出文件
					packet.stream_index = m_pVideoStream->index;
					av_interleaved_write_frame(m_pFmtCtx, &packet);
					av_packet_unref(&packet);
				}
			}
			emit curSecChanged(sec);
		}
		else if (!m_spReader->decoding())
		{
			break;
		}
	}

	avcodec_send_frame(m_pVideoCodecCtx, nullptr); // 发送结束信号
	while (avcodec_receive_packet(m_pVideoCodecCtx, &packet) == 0) {
		packet.stream_index = m_pVideoStream->index;
		packet.pts = av_rescale_q_rnd(packet.pts, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.dts = av_rescale_q_rnd(packet.dts, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.duration = av_rescale_q_rnd(1, video_time_base, m_pVideoStream->time_base,
			(AVRounding)(AV_ROUND_INF | AV_ROUND_PASS_MINMAX));
		packet.time_base = m_pVideoStream->time_base;

		av_interleaved_write_frame(m_pFmtCtx, &packet);
		av_packet_unref(&packet);
	}
}


void AVFileWriter::reset(const QString& newPath)
{
	if (m_pAudioCodecCtx != nullptr)
	{
		avcodec_close(m_pAudioCodecCtx); // 关闭音频解码器的实例
		avcodec_free_context(&m_pAudioCodecCtx); // 释放音频解码器的实例
		m_pAudioCodecCtx = nullptr;
	}

	if (m_pVideoCodecCtx != nullptr)
	{
		avcodec_close(m_pVideoCodecCtx); // 关闭音频解码器的实例
		avcodec_free_context(&m_pVideoCodecCtx); // 释放音频解码器的实例
		m_pVideoCodecCtx = nullptr;
	}

	if (m_pFmtCtx != nullptr)
	{
		avio_closep(&m_pFmtCtx->pb);
		avformat_close_input(&m_pFmtCtx);
		m_pFmtCtx = nullptr;
		m_pAudioStream = nullptr;
		m_pVideoStream = nullptr;
	}

	if (m_pathOutputTemp != "")
	{
		if (newPath != "")
		{
			QFile file(newPath);
			if (file.exists())
				file.remove();

			file.setFileName(m_pathOutputTemp);
			if (file.exists() && !file.rename(newPath))
			{
				qDebug() << "重命名失败！保留temp。";
				return;
			}
		}

		QFile file(m_pathOutputTemp);
		if (file.exists())
			file.remove();
	}
}
