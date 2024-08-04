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

FFrame::FFrame(const FFrame& f)
{
	m_pFrame = av_frame_clone(f.m_pFrame);
	m_valid = f.m_valid;
}

FFrame::~FFrame()
{
	av_frame_free(&m_pFrame);
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
	if (m_pSwsCtx == nullptr || !spf || spf->getPts() == m_lastPts)
	{
		return;
	}
	spf->swsVideoFrame(m_pSwsCtx, m_objHeight, m_spYuvFrame);
	m_lastPts = spf->getPts();
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
