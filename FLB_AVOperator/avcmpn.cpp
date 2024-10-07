#include "avoperator.h"
#include "audiomanager.h"
#include "videomanager.h"
#include <QDebug>

// 将秒数转换为 time_base 下的 PTS
int64_t seconds_to_pts(double seconds, AVRational time_base) {
	// 将秒数转化为 AVRational {seconds, 1}
	AVRational sec_rational = { static_cast<int>(seconds * 1000000), 1000000 };  // 用微秒来表达

	// 使用 av_rescale_q 将秒数转换为给定时间基下的 PTS
	int64_t pts = av_rescale_q(sec_rational.num, sec_rational, time_base);
	return pts;
}

void FAVInfo::reset()noexcept
{
	m_isOpen = false;
	m_vIndx = -1;
	m_aIndx = -1;
}

bool FAVInfo::equalAudioInfo(const FAVInfo& info)const
{

	return false;
}

FAVProcessors::~FAVProcessors()
{
	this->reset();
}

void FAVProcessors::reset()noexcept
{
	if (m_pADecCtx != nullptr)
	{
		avcodec_close(m_pADecCtx); // 关闭音频解码器的实例
		avcodec_free_context(&m_pADecCtx); // 释放音频解码器的实例
		m_pADecCtx = nullptr;
	}

	if (m_pVDecCtx != nullptr)
	{
		avcodec_close(m_pVDecCtx); // 关闭音频解码器的实例
		avcodec_free_context(&m_pVDecCtx); // 释放音频解码器的实例
		m_pVDecCtx = nullptr;
	}

	if (m_pInFmtCtx != nullptr)
	{
		avformat_close_input(&m_pInFmtCtx);
		m_pInFmtCtx = nullptr;
		m_pSrcAudio = nullptr;
		m_pSrcVideo = nullptr;
	}
}

//std::atomic<int> g_num(0);
FFrame::FFrame()
{
	m_pFrame = av_frame_alloc();
	//++g_num;
}

FFrame::FFrame(AVSampleFormat fmt, AVChannelLayout ch_layout, int sample_rate, int nb_samples) : FFrame()
{
	m_pFrame->format = fmt;
	m_pFrame->ch_layout = ch_layout;
	m_pFrame->channels = ch_layout.nb_channels;
	m_pFrame->nb_samples = nb_samples;
	m_pFrame->sample_rate = sample_rate;
	m_pFrame->time_base = { 1, sample_rate };

	if (av_frame_get_buffer(m_pFrame, 32) < 0 || av_frame_make_writable(m_pFrame) < 0)
		qDebug() << "frame buffer get failed!";
	else
	{
		if (av_sample_fmt_is_planar(fmt))
		{
			for (int c = 0; c < m_pFrame->ch_layout.nb_channels; ++c)
				memset(m_pFrame->data[c], 0, m_pFrame->linesize[c]);
		}
		else
		{
			memset(m_pFrame->data[0], 0, m_pFrame->linesize[0]);
		}
		m_valid = true;
	}

}

//QHash<FFrame*, uint8_t*> mapAlloc;
//QMutex tmpMutex;
FFrame::FFrame(AVPixelFormat fmt, int height, int width, const QString& clr) : FFrame()
{
	if (m_pFrame)
	{
		m_pFrame->format = fmt;
		m_pFrame->height = height;
		m_pFrame->width = width;

		const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(fmt));
		if (!desc || desc->nb_components <= 0) {
			return;
		}

		if (av_image_fill_linesizes(m_pFrame->linesize, fmt, width) < 0)
			return;

		if (!video::VideoMemoryManager::getInstance()->allocateImageMem(m_pFrame->data, m_pFrame->linesize,
			m_pFrame->width, m_pFrame->height, fmt))
			return;
		//qDebug() << this << "分配" << m_pFrame->data[0];

		/*{
			QMutexLocker locker(&tmpMutex);
			if (mapAlloc.contains(this))
				qDebug() << this << "重新分配！";
			mapAlloc[this] = m_pFrame->data[0];
		}*/
		QColor color(clr);
		if (!color.isValid())
			color = Qt::black;

		if (clr != "")
			this->fillColor(clr);

		m_valid = true;
	}
}

//FFrame::FFrame(const FFrame& f) : FFrame()
//{
//	if (av_frame_copy(m_pFrame, f.m_pFrame) >= 0)
//		m_valid = f.m_valid;
//}

FFrame::~FFrame()
{
	if (m_pFrame != nullptr)
	{
		if (this->isVideo())
		{
			video::VideoMemoryManager::getInstance()->freeMem(m_pFrame->data[0]);
			/*{
				QMutexLocker locker(&tmpMutex);
				if (mapAlloc.contains(this) && mapAlloc[this] != m_pFrame->data[0])
				{
					qDebug() << this << "分配了" << mapAlloc[this] << " 想释放" << m_pFrame->data[0];
					qDebug() << "额外信息：" << m_externInfo;
				}
			}*/
			//qDebug() << this << "释放" << m_pFrame->data[0];
		}
		else if (m_externInfo != "")
			qDebug() << m_externInfo;
		/*--g_num;
		qDebug() << "释放帧，剩余" << g_num.load();*/

		av_frame_unref(m_pFrame);
		av_frame_free(&m_pFrame);
	}
}

std::shared_ptr<FFrame> FFrame::deepAFClone()const
{
	if (!this->isAudio())
		return nullptr;

	FrameSPtr res = std::make_shared<FFrame>();
	auto dst_frame = res->m_pFrame;
	auto src_frame = m_pFrame;

	// Set properties from the source frame
	dst_frame->nb_samples = src_frame->nb_samples;
	dst_frame->format = src_frame->format;
	dst_frame->channel_layout = src_frame->channel_layout;
	dst_frame->sample_rate = src_frame->sample_rate;

	// Allocate the necessary buffer for the destination frame
	if (av_frame_get_buffer(dst_frame, 0) < 0)
	{
		return nullptr;
	}

	// Copy frame properties (metadata) from source to destination
	if (av_frame_copy_props(dst_frame, src_frame) < 0)
	{
		return nullptr;
	}

	// Copy frame data from source to destination
	if (av_frame_copy(dst_frame, src_frame) < 0)
	{
		fprintf(stderr, "Could not copy frame data\n");
		return nullptr;
	}

	res->m_valid = m_valid;
	res->m_flag = m_flag;
	return res;
}

std::shared_ptr<FFrame> FFrame::deepVFClone()const
{
	if (!this->isVideo())
		return nullptr;

	FrameSPtr res = std::make_shared<FFrame>();
	AVFrame* dst = res->m_pFrame;

	if (dst == nullptr)
		return nullptr;

	// 复制帧的参数（宽度、高度、格式等）
	if (av_frame_copy_props(dst, m_pFrame) < 0)
		return nullptr;

	//// 分配新的缓冲区来存储帧数据
	//if (av_image_alloc(dst->data, dst->linesize, m_pFrame->width, m_pFrame->height,
	//	static_cast<AVPixelFormat>(m_pFrame->format), 8) < 0)
	//	return nullptr;

	if (!video::VideoMemoryManager::getInstance()->allocateImageMem(dst->data, m_pFrame->linesize, m_pFrame->width,
		m_pFrame->height, static_cast<AVPixelFormat>(m_pFrame->format)))
		return nullptr;

	// 复制数据到新的缓冲区
	dst->height = m_pFrame->height;
	dst->width = m_pFrame->width;
	dst->format = m_pFrame->format;
	memcpy(dst->linesize, m_pFrame->linesize, sizeof(int) * 8);
	av_image_copy(dst->data, dst->linesize, (const uint8_t**)m_pFrame->data, m_pFrame->linesize,
		static_cast<AVPixelFormat>(m_pFrame->format), m_pFrame->width, m_pFrame->height);

	res->m_valid = true;
	res->m_flag = m_flag;
	return res;
}

const AVFrame* FFrame::getAVFrame()const
{
	return m_pFrame;
}

int FFrame::decode(AVCodecContext* dec_ctx, AVStream* strm)
{
	int ret = avcodec_receive_frame(dec_ctx, m_pFrame);
	if (ret == 0)
	{
		this->m_valid = true;
		m_pFrame->time_base = strm == nullptr ? dec_ctx->time_base : strm->time_base;
		//m_pts = m_pFrame->pts;
	}
	else
	{
		m_valid = false;
	}
	return ret;
}

int FFrame::send2Encoder(AVCodecContext* encCtx)
{
	if (!this->valid() || !encCtx)
	{
		return -999;
	}

	int ret = avcodec_send_frame(encCtx, m_pFrame);
	return ret;
}

void FFrame::setPts(int64_t pts)
{
	m_pFrame->pts = pts;
}

void FFrame::setPtsSecond(double sec)
{
	if (m_pFrame && m_valid)
	{
		AVRational tb = m_pFrame->time_base;
		m_pFrame->pts = round(sec * tb.den / tb.num);
	}
}

void FFrame::setTimeBase(int num, int den)
{
	if (m_pFrame && m_valid)
		m_pFrame->time_base = { num, den };
}

void FFrame::setDuration(int d)
{
	if (this->valid())
		m_pFrame->pkt_duration = d;
}

int64_t FFrame::getPts()const
{
	return m_pFrame->pts;
}

double FFrame::getSecond()const
{
	return av_q2d(m_pFrame->time_base) * this->getPts();
}

double FFrame::getDurationSecond()const
{
	if (this->isAudio())
		return static_cast<double>(m_pFrame->nb_samples) / m_pFrame->sample_rate;
	else
		return av_q2d(m_pFrame->time_base) * m_pFrame->pkt_duration;
}

double FFrame::getEndSecond()const
{
	return this->getSecond() + this->getDurationSecond();
}

AVRational FFrame::getTimeBase()const
{
	return m_pFrame->time_base;
}

int FFrame::width()const
{
	return m_pFrame->width;
}

int FFrame::height()const
{
	return m_pFrame->height;
}

void FFrame::copy2AudioBufferPlanar(std::vector<std::vector<uint8_t>>& buffer, double second)
{
	if (!this->isAudio())
		return;

	AVSampleFormat format = (AVSampleFormat)m_pFrame->format;
	int data_size = av_get_bytes_per_sample(format);
	int newSizePerChannel = m_pFrame->nb_samples * data_size;;
	int start = buffer.size() > 0 ? buffer[0].size() : 0;
	int channel = m_pFrame->ch_layout.nb_channels;
	if (channel > buffer.size())
	{
		int ors = buffer.size();
		buffer.resize(channel);
		if (ors > 0)
		{
			for (int i = ors; i < buffer.size(); ++i)
				buffer[i] = buffer[0];
		}
	}

	for (auto& chBuff : buffer)
	{
		chBuff.resize(newSizePerChannel + chBuff.size());
	}
	if (av_sample_fmt_is_planar(format))
	{
		for (int c = 0; c < m_pFrame->ch_layout.nb_channels; ++c)
		{
			memcpy(&buffer[c][start], m_pFrame->data[c], newSizePerChannel);
		}
	}
	else
	{
		int cur = 0, pCur = start;
		for (int i = 0; i < m_pFrame->nb_samples; ++i)
		{
			for (int c = 0; c < channel; ++c)
			{
				memcpy(&buffer[c][pCur], &m_pFrame->data[0][cur], data_size);
				cur += data_size;
			}
			pCur += data_size;
		}
	}
}

void FFrame::fromAudioBufferPlanar(std::vector<std::vector<uint8_t>>& buffer, float factor)
{
	int channel = std::min((int)buffer.size(), m_pFrame->ch_layout.nb_channels);
	if (channel < 1 || !m_valid || buffer[0].size() == 0)
		return;

	/*for (int i = 0; i < buffer.size(); ++i)
		buffer[i].clear();
	return;*/

	auto format = (AVSampleFormat)m_pFrame->format;
	auto amPtr = audio::getManagerPtr(format);
	int data_size = amPtr->perSize(), sizePerChannel = m_pFrame->nb_samples * amPtr->perSize();
	int cpy_size = std::min(m_pFrame->nb_samples, static_cast<int>(buffer[0].size() / data_size));
	if (av_sample_fmt_is_planar(format))
	{
		for (int c = 0; c < channel; ++c)
		{
			amPtr->Add2OutBuffer(m_pFrame->data[c], &buffer[c][0], factor, cpy_size);
		}
	}
	else
	{
		int cur = 0, pCur = 0;
		int restChannel = m_pFrame->ch_layout.nb_channels - channel;
		for (int i = 0; i < cpy_size; ++i)
		{
			for (int c = 0; c < channel; ++c)
			{
				amPtr->Add2OutBuffer(&m_pFrame->data[0][cur], &buffer[c][pCur], factor, 1);
				cur += data_size;
			}
			cur += (restChannel)*data_size;
			pCur += data_size;
		}
	}

	for (auto& buf : buffer)
	{
		buf.erase(buf.begin(), buf.begin() + std::min(static_cast<size_t>(sizePerChannel), buf.size()));
	}
}

void FFrame::drawFrom(FrameSPtr other, int x, int y)
{
	if (!this->isVideo() || !other || !other->isVideo())
		return;

	auto srcFrame = other->m_pFrame;
	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(m_pFrame->format));
	if (m_pFrame->format != srcFrame->format || !desc || desc->nb_components <= 0) {
		return;
	}

	auto videoManager = video::IVideoManager::getManagerByDepth(desc->comp[0].depth);
	if (videoManager == nullptr)
		return;

	videoManager->coverFrame(m_pFrame, srcFrame, x, y);
}

void FFrame::outPutSample()
{
	if (valid())
	{
		QList<uint8_t> res;
		for (int i = 0; i < m_pFrame->nb_samples; ++i)
		{
			res.push_back(m_pFrame->data[0][i]);
		}

		qDebug() << res;
	}
}

void FFrame::outPutImage(int c, int step)
{
	if (valid())
	{
		c = std::min(2, c);
		c = std::max(0, c);

		// 获取像素格式描述信息
		const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(m_pFrame->format));
		if (!desc || desc->nb_components <= 0) {
			return;
		}
		QList<uint8_t> res;
		int mvh = 0, mvw = 0;
		if (c > 0)
		{
			mvh = desc->log2_chroma_h;
			mvw = desc->log2_chroma_w;
		}
		for (int i = 0; i < (m_pFrame->height >> mvh); i += step)
		{
			for (int j = 0; j < (m_pFrame->width >> mvw); j += step)
				res.push_back(m_pFrame->data[c][i * m_pFrame->linesize[c] + j]);
		}
		qDebug() << res;
	}
}

void FFrame::setExternInfo(const QString& info)
{
	m_externInfo = info;
}

void FFrame::setFlag(FrameFlag flag)
{
	m_flag = flag;
}

QImage FFrame::toQImage()const
{
	if (!this->isVideo())
		return QImage();

	AVFrame* frame = m_pFrame;
	// 创建SWS上下文用于格式转换，将YUV转为RGB
	SwsContext* swsCtx = sws_getContext(
		frame->width, frame->height, (AVPixelFormat)frame->format,  // 源图像宽度、高度、格式
		frame->width, frame->height, AV_PIX_FMT_RGB24,              // 目标图像格式为RGB24
		SWS_BILINEAR, nullptr, nullptr, nullptr);

	if (!swsCtx) {
		qDebug() << "Failed to create SwsContext!";
		return QImage();
	}

	// 为目标图像数据分配内存
	uint8_t* rgbData[1]; // 输出的图像数据
	int rgbLineSize[1];  // 输出图像每行的字节数
	int bytesPerLine = frame->linesize[0] * 3;  // RGB24的每行字节数 = 宽度 * 3（R、G、B）
	rgbData[0] = (uint8_t*)malloc(bytesPerLine * frame->height);  // 分配内存
	rgbLineSize[0] = bytesPerLine;

	// 将AVFrame中的YUV数据转换为RGB数据
	sws_scale(swsCtx, frame->data, frame->linesize, 0, frame->height, rgbData, rgbLineSize);

	// 使用转换后的RGB数据创建QImage
	QImage image(rgbData[0], frame->width, frame->height, rgbLineSize[0], QImage::Format_RGB888,
		[](void* pRgbData) {
			free(pRgbData);
		}, rgbData[0]);

	// 释放SwsContext
	sws_freeContext(swsCtx);

	return image;
}

FrameFlag FFrame::flag()const
{
	return m_flag;
}

bool FFrame::valid()const
{
	return m_valid;
}

bool FFrame::isAudio()const
{
	return m_valid && m_pFrame && m_pFrame->nb_samples > 0;
}

bool FFrame::isVideo()const
{
	return m_valid && m_pFrame && m_pFrame->width > 0 && m_pFrame->height > 0;
}

int FFrame::getSample()const
{
	return m_valid ? m_pFrame->nb_samples : 0;
}

int FFrame::swrFrame(SwrContext* swrCtx, unsigned char* outBuf, int max_size)
{
	//KAFrameSPtr spf = std::make_shared<KAudioFrame>(av_frame_alloc());
	if (m_pFrame->nb_samples <= 0)
	{
		return 0;
	}

	int swr_size = swr_convert(swrCtx, // 音频采样器的实例
		&outBuf, max_size, // 输出的数据内容和数据大小
		(const uint8_t**)m_pFrame->data, m_pFrame->nb_samples); // 输入的数据内容和数据大小

	return swr_size;
}

int FFrame::swrFrame(SwrContext* swrCtx, AVFrame* other, int max_size)
{
	if (m_pFrame->nb_samples <= 0)
	{
		return 0;
	}

	int swr_size = swr_convert(swrCtx, // 音频采样器的实例
		other->data, max_size, // 输出的数据内容和数据大小
		(const uint8_t**)m_pFrame->data, m_pFrame->nb_samples); // 输入的数据内容和数据大小

	return swr_size;
}

int FFrame::swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame)
{
	if (!yuvFrame)
		return -1;
	return sws_scale(swsCtx, (uint8_t const* const*)m_pFrame->data, m_pFrame->linesize,
		0, sh, yuvFrame->m_pFrame->data, yuvFrame->m_pFrame->linesize);
}

void FFrame::imageRelate(uint8_t* buffer, AVPixelFormat format, int w, int h)
{
	if (buffer == nullptr || w <= 0 || h <= 0)
		return;

	av_image_fill_arrays(m_pFrame->data, m_pFrame->linesize, buffer,
		format, w, h, 1);
	m_pFrame->format = format;
	m_pFrame->height = h;
	m_pFrame->width = w;
	m_valid = true;
}

AVFrame* FFrame::allocVideoFrame(AVPixelFormat fmt, int height, int width)
{
	AVFrame* pFrame = av_frame_alloc();
	if (!pFrame)
		return pFrame;

	pFrame->format = fmt;
	pFrame->height = height;
	pFrame->width = width;

	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(fmt));
	if (!desc || desc->nb_components <= 0) {
		av_frame_free(&pFrame);
		return nullptr;
	}

	if (av_image_fill_linesizes(pFrame->linesize, fmt, width) < 0)
	{
		av_frame_free(&pFrame);
		return nullptr;
	}

	if (!video::VideoMemoryManager::getInstance()->allocateImageMem(pFrame->data, pFrame->linesize,
		pFrame->width, pFrame->height, fmt))
	{
		av_frame_free(&pFrame);
		return nullptr;
	}
	return pFrame;
}

void FFrame::fillColor(const QColor& color)
{
	if (!this->valid())
		return;

	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(m_pFrame->format));
	if (!desc || desc->nb_components <= 0) {
		return;
	}

	int depth = desc->comp[0].depth;
	float ratio = ((1 << depth) - 1) / 255.0f;
	int red = color.red() * ratio;
	int green = color.green() * ratio;
	int blue = color.blue() * ratio;

	int y, u, v;
	std::tie(y, u, v) = video::getYUV_BT601_DV(red, green, blue, depth);

	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	if (videoManager == nullptr)
		return;

	videoManager->coverYUV(m_pFrame, y, u, v);
}

VideoFrameSuperPosed::VideoFrameSuperPosed(AVPixelFormat fmt, int height, int width, const QString& clr) :
	FFrame(fmt, height, width, ""), m_clrFill(clr)
{

}

std::shared_ptr<FFrame> VideoFrameSuperPosed::deepVFClone()const
{
	if (!this->isVideo())
		return nullptr;

	auto res = std::make_shared<VideoFrameSuperPosed>((AVPixelFormat)m_pFrame->format, m_pFrame->height,
		m_pFrame->width, m_clrFill.name());

	// 复制帧的参数（宽度、高度、格式等）
	if (av_frame_copy_props(res->m_pFrame, m_pFrame) < 0)
		return nullptr;

	for (auto& spStream : m_hashStreamFrame.keys())
		res->addStreamFrame(spStream, m_hashStreamFrame[spStream]);

	res->m_pStreamHide = m_pStreamHide;
	if (m_drawnByHide)
		res->redrawHide();
	else
		res->redrawWhole();

	//qDebug() << res->getSecond();
	return res;
}

int VideoFrameSuperPosed::swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame)
{
	if (m_drawnByHide && !m_pStreamHide)
		this->redrawWhole();
	else if (m_pStreamHide)
		this->redrawHide();
	else if (!this->checkVersion())
		this->redrawWhole();

	return FFrame::swsVideoFrame(swsCtx, sh, std::move(yuvFrame));
}

void VideoFrameSuperPosed::addStreamFrame(const VideoStreamPtr& spStream, const FrameSPtr& spFrame)
{
	if (spStream && spFrame)
	{
		m_hashStreamFrame.insert(spStream, spFrame);
		connect(spStream.get(), &IVideoStream::streamWillAdjustThis,
			this, &VideoFrameSuperPosed::hideStreamFrame, Qt::DirectConnection);
		connect(spStream.get(), &IVideoStream::StreamAdjustWillFinished,
			this, &VideoFrameSuperPosed::cancelHidden, Qt::DirectConnection);
	}
}

void VideoFrameSuperPosed::hideStreamFrame(IVideoStream* pStream)
{
	if (pStream && m_hashStreamFrame.contains(pStream->getSharedPointer()))
	{
		m_pStreamHide = pStream;
	}
}

void VideoFrameSuperPosed::cancelHidden()
{
	if (m_pStreamHide)
	{
		m_pStreamHide = nullptr;
	}
}

void VideoFrameSuperPosed::onStreamRemoved(VideoStreamPtr spStream)
{
	if (m_hashStreamFrame.contains(spStream))
	{
		m_hashStreamFrame.remove(spStream);
		m_hashDrawnVersion.remove(spStream);
	}
}

void VideoFrameSuperPosed::redrawWhole()
{
	//auto msBegin = QDateTime::currentMSecsSinceEpoch();
	this->fillColor(m_clrFill);
	//auto msEnd = QDateTime::currentMSecsSinceEpoch();
	//qDebug() << "色彩填充耗时" << (msEnd - msBegin);

	//msBegin = QDateTime::currentMSecsSinceEpoch();
	auto srtStreams = this->sortStreamLevel();
	//msEnd = QDateTime::currentMSecsSinceEpoch();
	//qDebug() << "排序耗时" << (msEnd - msBegin);
	for (const auto& spStream : srtStreams)
	{
		if (!spStream->removed())
		{
			auto spFrame = m_hashStreamFrame[spStream];
			spFrame = spStream->swsObjFrame(spFrame);
			this->drawFrom(std::move(spFrame), spStream->xPos(), spStream->yPos());
			m_hashDrawnVersion[spStream] = spStream->version();
		}
	}
	m_drawnByHide = false;
}

void VideoFrameSuperPosed::redrawHide()
{
	this->fillColor(m_clrFill);

	auto srtStreams = this->sortStreamLevel();
	VideoStreamPtr spHideStream;
	for (const auto& spStream : srtStreams)
	{
		if (spStream.get() != m_pStreamHide)
		{
			if (!spStream->removed())
			{
				auto spFrame = m_hashStreamFrame[spStream];
				spFrame = spStream->swsObjFrame(spFrame);
				this->drawFrom(std::move(spFrame), spStream->xPos(), spStream->yPos());
				m_hashDrawnVersion[spStream] = spStream->version();
			}
		}
		else
		{
			spHideStream = spStream;
		}
	}

	if (spHideStream)
	{
		m_pStreamHide->widgetShowStreamFrame(m_hashStreamFrame[spHideStream]);
	}
	m_drawnByHide = true;
}

QList<VideoStreamPtr> VideoFrameSuperPosed::sortStreamLevel()
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

	auto srtStreams = m_hashStreamFrame.keys();
	std::sort(srtStreams.begin(), srtStreams.end(), compStream);
	return srtStreams;
}

bool VideoFrameSuperPosed::checkVersion()const
{
	for (auto& spStream : m_hashDrawnVersion.keys())
	{
		if (spStream->version() != m_hashDrawnVersion[spStream])
			return false;
	}
	return true;
}

FrameSafeUtils& FrameSafeUtils::getInstance()
{
	static FrameSafeUtils ls_instance;
	return ls_instance;
}

VideoFrameProcesser::VideoFrameProcesser(const FAVInfo* pInfo)
{
	if (pInfo == nullptr)
		return;

	m_objWidth = pInfo->m_width;
	m_objHeight = pInfo->m_height;

	AVRational aspect_ratio = pInfo->m_aspect_ratio;
	AVRational display_aspect_ratio;
	av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
		m_objWidth * aspect_ratio.num,
		m_objHeight * aspect_ratio.den,
		1024 * 1024);

	if (aspect_ratio.num * aspect_ratio.den != 0 && aspect_ratio.num != aspect_ratio.den != 1)
	{
		m_objWidth = m_objHeight * display_aspect_ratio.num / display_aspect_ratio.den;
	}

	m_pSwsCtx = sws_getContext(pInfo->m_width, pInfo->m_height, pInfo->m_pixFmt,
		m_objWidth, m_objHeight, m_yuvFormat, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	if (m_pSwsCtx == nullptr)
	{
		qDebug() << __FUNCTION__ << " Error: sws_ctx allocation failed!";
		return;
	}

	int yuv_size = av_image_get_buffer_size(m_yuvFormat, m_objWidth, m_objHeight, 1);
	m_pYuvBuffer = (uint8_t*)av_malloc((size_t)yuv_size * sizeof(uint8_t));
	//memset(m_pYuvBuffer, 0, (size_t)yuv_size * sizeof(uint8_t));
	if (m_pYuvBuffer == nullptr)
	{
		sws_freeContext(m_pSwsCtx);
		m_pSwsCtx = nullptr;
		return;
	}

	// 将数据帧与缓冲区关联
	m_spYuvFrame = std::make_shared<FFrame>();
	m_spYuvFrame->imageRelate(m_pYuvBuffer, m_yuvFormat, m_objWidth, m_objHeight);

	m_valid = true;
}

void VideoFrameProcesser::processFrame(FrameSPtr spf)
{
	if (m_pSwsCtx == nullptr || !spf)
	{
		return;
	}
	spf->swsVideoFrame(m_pSwsCtx, m_objHeight, m_spYuvFrame);
	//m_spYuvFrame->outPutImage(0, 120);
	//m_spYuvFrame->outPutImage(1, 120);
	//m_spYuvFrame->outPutImage(2, 120);
	m_spLastFrame = spf;
	m_frameDecoded = true;
}

void VideoFrameProcesser::reset()
{
	m_frameDecoded = false;
}

const uint8_t* VideoFrameProcesser::getYuvBuffer()const
{
	return m_frameDecoded ? m_pYuvBuffer : nullptr;
}

int VideoFrameProcesser::size()const
{
	return m_objHeight * m_objWidth;
}

int VideoFrameProcesser::objHeight()const
{
	return m_objHeight;
}

int VideoFrameProcesser::objWidth()const
{
	return m_objWidth;
}

bool VideoFrameProcesser::valid()const
{
	return m_valid;
}
