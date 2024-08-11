#include "avoperator.h"
#include <QDebug>

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


FFrame::FFrame()
{

	m_pFrame = av_frame_alloc();

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
		}

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

int64_t FFrame::getPts()const
{
	return m_pFrame->pts;
}

double FFrame::getSecond()const
{
	return av_q2d(m_pFrame->time_base) * this->getPts();
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

bool FFrame::valid()const
{
	return m_valid;
}

bool FFrame::isAudio()const
{
	return m_pFrame && m_pFrame->nb_samples > 0;
}

bool FFrame::isVideo()const
{
	return m_pFrame && m_pFrame->width && m_pFrame->height > 0;
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

int FFrame::swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame)
{
	if (!yuvFrame)
		return -1;
	return sws_scale(swsCtx, (uint8_t const* const*)m_pFrame->data, m_pFrame->linesize,
		0, sh, yuvFrame->m_pFrame->data, yuvFrame->m_pFrame->linesize);
}

void FFrame::imageRelate(uint8_t* buffer, AVPixelFormat format, int w, int h)
{
	av_image_fill_arrays(m_pFrame->data, m_pFrame->linesize, buffer,
		format, w, h, 1);
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
	if (m_pSwsCtx == nullptr || !spf || spf.get() == m_spLastFrame.get())
	{
		return;
	}
	spf->swsVideoFrame(m_pSwsCtx, m_objHeight, m_spYuvFrame);
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
