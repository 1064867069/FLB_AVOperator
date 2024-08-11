#include "videoprocess.h"
#include "avutils.h"
#include "avtemplates.h"
#include "videomanager.h"
#include <future>
#include <algorithm>

VideoProcessList::VideoProcessList(QObject* p) : IVideoFrameProcessor(p)
{

}

FrameSPtr VideoProcessList::processFrame(FrameSPtr spf)
{
	if (m_listProcs.size() == 0)
		return spf;

	auto res = spf->deepVFClone();
	if (!res)
		return spf;

	for (auto& proc : m_listProcs)
	{
		res = proc->processFrame(std::move(res));
	}
	return res;
}

void VideoProcessList::addVProcessor(const VProcessSPtr& spProc)
{
	if (!spProc)
		return;

	for (const auto& proc : m_listProcs)
	{
		if (proc.get() == spProc.get())
			return;
	}

	m_listProcs.append(spProc);
}

VideoBrightAdjust::VideoBrightAdjust(QObject* p) : IVideoFrameProcessor(p)
{

}

FrameSPtr VideoBrightAdjust::processFrame(FrameSPtr spf)
{
	if (!spf || !spf->isVideo() || std::abs(m_bright) < 1e-4)
		return spf;


	AVFrame* pvf = this->getAVFrame(spf.get());

	// 获取像素格式描述信息
	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(pvf->format));
	if (!desc || desc->nb_components <= 0) {
		return spf;
	}

	int depth = desc->comp[0].depth;
	float co = static_cast<float>((1 << depth) - 1) / 255;
	int maxV = 235 * co, minV = 16 * co;
	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	if (videoManager == nullptr)
		return spf;

	FrameSPtr res = spf;
	//if (!res)
	//	return spf;
	pvf = this->getAVFrame(res.get());

	auto func = videoManager->AddDecPerFunc(minV, maxV, m_bright);
	auto partProcWork = [&func, videoManager](AVFrame* pvf, int s, int l, int w)->void
	{
		for (int i = 0; i < l; ++i)
		{
			video::seriesProc(&pvf->data[0][(s + i) * pvf->linesize[0]], pvf->width,
				videoManager->perSize(), func);
		}
	};

	if (pvf->height < 100)
	{
		partProcWork(pvf, 0, pvf->height, pvf->width);
		/*for (int i = 0; i < pvf->height; ++i)
		{
			video::seriesProc(&pvf->data[0][i * pvf->linesize[0]], pvf->width,
				videoManager->perSize(), func);
		}*/
	}
	else
	{
		int s = 0, len1 = pvf->height / 4 + 1;
		std::future<void> res1 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		std::future<void> res2 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		std::future<void> res3 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		partProcWork(pvf, s, pvf->height - s, pvf->width);
		res1.get();
		res2.get();
		res3.get();
	}

	return res;
}

void VideoBrightAdjust::setBright(float b)
{
	m_bright = std::max(-1.0f, b);
	m_bright = std::min(1.0f, m_bright);
}

VideoChromAdjust::VideoChromAdjust(QObject* p) : IVideoFrameProcessor(p)
{

}

FrameSPtr VideoChromAdjust::processFrame(FrameSPtr spf)
{
	if (!spf || !spf->isVideo() || std::abs(m_chrom) < 1e-4)
		return spf;


	AVFrame* pvf = this->getAVFrame(spf.get());

	// 获取像素格式描述信息
	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(pvf->format));
	if (!desc || desc->nb_components <= 0) {
		return spf;
	}

	int depth = desc->comp[0].depth;
	int maxV = (1 << depth) - 1, minV = 0;
	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	if (videoManager == nullptr)
		return spf;

	FrameSPtr res = spf;
	/*if (!res)
		return spf;*/

	pvf = this->getAVFrame(res.get());

	auto func = videoManager->AddDecPerFunc(minV, maxV, m_chrom);
	for (int i = 1; i < 3; ++i)
	{
		int height = pvf->height * pvf->linesize[i] / pvf->linesize[0];
		int width = pvf->width * pvf->linesize[i] / pvf->linesize[0];
		for (int j = 0; j < height; ++j)
			video::seriesProc(&pvf->data[i][j * pvf->linesize[i]], width, videoManager->perSize(), func);
	}

	return res;
}

void VideoChromAdjust::setChrom(float b)
{
	m_chrom = std::max(-1.0f, b);
	m_chrom = std::min(1.0f, m_chrom);
}

VideoContrastAdjust::VideoContrastAdjust(QObject* p) :IVideoFrameProcessor(p)
{

}

FrameSPtr VideoContrastAdjust::processFrame(FrameSPtr spf)
{
	if (!spf || !spf->isVideo() || std::abs(m_contrast) < 1e-4)
		return spf;


	AVFrame* pvf = this->getAVFrame(spf.get());

	// 获取像素格式描述信息
	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(pvf->format));
	if (!desc || desc->nb_components <= 0) {
		return spf;
	}

	int depth = desc->comp[0].depth;
	float co = static_cast<float>((1 << depth) - 1) / 255;
	int maxV = 235 * co, minV = 16 * co;
	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	if (videoManager == nullptr)
		return spf;

	FrameSPtr res = spf;

	pvf = this->getAVFrame(res.get());

	int avrVal = (maxV + minV) / 2;
	videoManager->avr2dData(pvf->data[0], &avrVal, pvf->width, pvf->height, pvf->linesize[0], 16);
	auto func = videoManager->midADPerFunc(minV, maxV, avrVal, m_contrast);

	auto partProcWork = [&func, videoManager](AVFrame* pvf, int s, int l, int w)->void
	{
		for (int i = 0; i < l; ++i)
		{
			video::seriesProc(&pvf->data[0][(s + i) * pvf->linesize[0]], pvf->width,
				videoManager->perSize(), func);
		}
	};

	if (pvf->height < 100)
	{
		partProcWork(pvf, 0, pvf->height, pvf->width);
	}
	else
	{
		int s = 0, len1 = pvf->height / 4 + 1;
		std::future<void> future1 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		std::future<void> future2 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		std::future<void> future3 = std::async(std::launch::async, partProcWork, pvf, s, len1, pvf->width);

		s += len1;
		partProcWork(pvf, s, pvf->height - s, pvf->width);
		future1.get();
		future2.get();
		future3.get();
	}

	return res;
}

void VideoContrastAdjust::setContrast(float b)
{
	m_contrast = std::max(-1.0f, b);
	m_contrast = std::min(1.0f, m_contrast);
}
