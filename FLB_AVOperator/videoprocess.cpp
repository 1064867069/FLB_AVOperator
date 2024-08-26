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
	int maxV = 255 * co, minV = 0;
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

void VideoBrightAdjust::setBright(int b)
{
	m_bright = std::max(-1.0f, static_cast<float>(b) / 100);
	m_bright = std::min(1.0f, m_bright);
}
#include <qthread.h>
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

	int minVal = 16 * ((1 << depth) - 1) / 255;
	int maxV = 240 * ((1 << depth) - 1) / 255, maxY = 235 * ((1 << depth) - 1) / 255;
	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	if (videoManager == nullptr)
		return spf;

	FrameSPtr res = spf;
	/*if (!res)
		return spf;*/

	pvf = this->getAVFrame(res.get());

	/*auto func = videoManager->AddDecPerFunc(minVal, maxV, m_chrom);
	for (int i = 1; i < 3; ++i)
	{
		int height = (pvf->height >> desc->log2_chroma_h);
		int width = (pvf->width >> desc->log2_chroma_w);
		for (int j = 0; j < height; ++j)
			video::seriesProc(&pvf->data[i][j * pvf->linesize[i]], width, videoManager->perSize(), func);
	}*/

	videoManager->chromAdjust(pvf, desc, m_chrom, depth);
	/*auto data = pvf->data;
	int ii, jj, yi, ui, vi, y, y2, u, v, r, g, b, gap1, gap2, * pMin, * pMax, * pMid;
	int yi0 = 0, ui0 = 0, vi0 = 0;
	int hStep = (1 << desc->log2_chroma_h), wStep = (1 << desc->log2_chroma_w) * videoManager->perSize();
	std::tuple<int, int, int> tp3;
	for (int i = 0; i < pvf->height; i += hStep)
	{
		for (int j = 0; j < pvf->width * videoManager->perSize(); j += wStep)
		{
			jj = (j >> desc->log2_chroma_w);
			yi = yi0 + j, ui = ui0 + jj, vi = vi0 + jj;
			y = videoManager->getVal(&data[0][yi]);
			u = videoManager->getVal(&data[1][ui]);
			v = videoManager->getVal(&data[2][vi]);
			tp3 = video::getRGB_BT601(y, u, v);
			std::tie(r, g, b) = tp3;

			pMin = (r > g ? &g : &r), pMax = (r > g ? &r : &g);
			pMin = *pMin > b ? &b : pMin;
			pMax = *pMax > b ? pMax : &b;
			for (auto* p : { &r,&g,&b })
				if (p != pMin && p != pMax)
					pMid = p;

			gap1 = ((*pMax - *pMid));
			gap2 = ((*pMid - *pMin));
			gap1 *= m_chrom;
			gap2 *= m_chrom;
			if (gap1 == 0 && gap2 == 0)
				continue;

			*pMax += gap1;
			*pMin -= gap2;
			tp3 = video::getYUV_BT601(r, g, b);
			std::tie(y2, u, v) = tp3;
			y2 = video::clamp(y2, minVal, maxY) - y;
			u = video::clamp(u, minVal, maxV);
			v = video::clamp(v, minVal, maxV);
			for (int k1 = 0; k1 < hStep; ++k1)
			{
				int yi2 = yi + k1 * pvf->linesize[0];

				for (int k2 = 0; k2 < wStep; k2 += videoManager->perSize())
				{
					y = videoManager->getVal(&data[0][yi2 + k2]) + y2;
					videoManager->setVal(&data[0][yi2 + k2], y);
				}
			}
			videoManager->setVal(&data[1][ui], u);
			videoManager->setVal(&data[2][vi], v);
		}
		yi0 += (pvf->linesize[0] << desc->log2_chroma_h), ui0 += pvf->linesize[1], vi0 += pvf->linesize[2];
	}*/

	/*int h = (pvf->height >> desc->log2_chroma_h), w = (pvf->width >> desc->log2_chroma_w);
	int u, v, mid, gap1, gap2, idx, * pMin, * pMax;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			idx = i * pvf->linesize[1] + j * videoManager->perSize();
			u = videoManager->getVal(&pvf->data[1][idx]);
			v = videoManager->getVal(&pvf->data[2][idx]);

			pMax = u > v ? &u : &v;
			pMin = pMax == &u ? &v : &u;

			mid = (u + v) / 2;
			gap1 = (*pMax - mid) * m_chrom;
			gap2 = (mid - *pMin) * m_chrom;
			*pMax += gap1;
			*pMin -= gap2;
			videoManager->setVal(&pvf->data[1][idx], clamp(u, minVal, maxV));
			videoManager->setVal(&pvf->data[2][idx], clamp(v, minVal, maxV));
		}
	}*/

	return res;
}

void VideoChromAdjust::setChrom(int b)
{
	m_chrom = static_cast<float>(b) / 100;
	m_chrom = std::max(-1.0f, m_chrom);
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

void VideoContrastAdjust::setContrast(int b)
{
	float contrast = static_cast<float>(b) / 100;
	m_contrast = std::max(-1.0f, contrast);
	m_contrast = std::min(1.0f, m_contrast);
}

VideoClrTempAdjust::VideoClrTempAdjust(QObject* p) :IVideoFrameProcessor(p)
{

}

FrameSPtr VideoClrTempAdjust::processFrame(FrameSPtr spf)
{
	if (!spf || !spf->isVideo() || std::abs(m_clrTemp) < 1e-4)
		return spf;

	AVFrame* pvf = this->getAVFrame(spf.get());

	// 获取像素格式描述信息
	const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(pvf->format));
	if (!desc || desc->nb_components <= 0) {
		return spf;
	}

	const int depth = desc->comp[0].depth;
	const int maxV = video::getUVMax(depth), minV = video::getYUVMin(depth);
	auto videoManager = video::IVideoManager::getManagerByDepth(depth);
	const int index = m_clrTemp < 0 ? 1 : 2;
	const int height = (pvf->height >> desc->log2_chroma_h), width = (pvf->width >> desc->log2_chroma_w);
	if (videoManager == nullptr)
		return spf;

	FrameSPtr res = spf;
	//if (!res)
	//	return spf;
	pvf = this->getAVFrame(res.get());

	auto func = videoManager->AddDecPerFunc(minV, maxV, std::abs(m_clrTemp));
	for (int i = 0; i < height; ++i)
	{
		video::seriesProc(&pvf->data[index][i * pvf->linesize[index]], width, videoManager->perSize(), func);
	}

	return res;
}

void VideoClrTempAdjust::setClrTemp(int temp)
{
	m_clrTemp = static_cast<float>(temp) / 100;
}
