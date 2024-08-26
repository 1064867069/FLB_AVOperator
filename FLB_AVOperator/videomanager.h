#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include "avutils.h"
#include "avtemplates.h"

#include <array>
#include <QDebug>

namespace video
{

	// 将值调整到指定的最小值和最大值之间
	inline int clamp(int value, int min, int max) {
		if (value > max) {
			return max;
		}
		else if (value < min) {
			return min;
		}
		else {
			return value;
		}
	}

	class IVideoManager
	{
	public:
		virtual int perSize() = 0;

		virtual int getVal(uint8_t*) = 0;

		virtual void setVal(uint8_t*, int val) = 0;

		virtual SingleDataFunc AddDecPerFunc(int low, int high, float brtCoe) = 0;

		virtual SingleDataFunc midADPerFunc(int low, int high, int mid, float brtCoe) = 0;

		virtual void avr2dData(const uint8_t* data, int* outNum, int w, int h, int ln_sz, int step = 1) = 0;

		virtual void chromAdjust(AVFrame* pvf, const AVPixFmtDescriptor* desc, float chrom, int depth) = 0;

		static IVideoManager* getManagerByDepth(int);
	};

	template<class T>
	class VideoManager : public IVideoManager
	{
	public:
		using UpperType = typename fmttp::UpperInType<T>::type;

		virtual int perSize()override { return sizeof(T); }

		virtual int getVal(uint8_t* data)override
		{
			return *reinterpret_cast<T*>(data);
		}

		virtual void setVal(uint8_t* data, int val)override
		{
			*reinterpret_cast<T*>(data) = static_cast<T>(val);
		}

		virtual SingleDataFunc AddDecPerFunc(int low, int high, float brtCoe)override {
			const int gapAll = high - low, gap = gapAll * brtCoe;
			auto func = [low, high, gap, gapAll](uint8_t* data)
			{
				auto* cur = reinterpret_cast<T*>(data);
				UpperType tmp = *cur;
				tmp = tmp + gap * static_cast<float>(tmp - low) / gapAll;

				if (gap < 0)
					tmp = std::max(static_cast<UpperType>(low), tmp);
				else if (gap > 0)
					tmp = std::min(static_cast<UpperType>(high), tmp);

				//if (brtCoe > 0.2 && tmp > 225)
				//	qDebug() << tmp << cur << tmp - cur;
				*cur = static_cast<T>(tmp);
			};
			return func;
		}

		virtual SingleDataFunc midADPerFunc(int low, int high, int mid, float brtCoe)override {
			int gap_h = (high - mid), gap_l = (mid - low);
			auto func = [=](uint8_t* data)
			{
				auto* cur = reinterpret_cast<T*>(data);
				UpperType tmp = *cur;

				if (tmp < mid && gap_l > 0)
				{
					tmp -= (tmp - low) * (mid - tmp) * brtCoe / gap_l;
					tmp = std::min(tmp, static_cast<UpperType>(mid));
					tmp = std::max(tmp, static_cast<UpperType>(low));
				}
				else if (tmp > mid&& gap_h > 0)
				{
					tmp += (high - tmp) * (tmp - mid) * brtCoe / gap_h;
					tmp = std::min(tmp, static_cast<UpperType>(high));
					tmp = std::max(tmp, static_cast<UpperType>(mid));
				}

				*cur = static_cast<T>(tmp);
			};

			return func;
		}

		virtual void avr2dData(const uint8_t* data, int* outNum, int w, int h, int ln_sz, int step)override
		{
			if (data == nullptr || outNum == nullptr)
				return;

			int cnt = 0;
			const T* data2T = reinterpret_cast<const T*>(data);
			for (int i = 0; i < h; i += step)
			{
				int curStep = 0;
				while (curStep < w)
				{
					*outNum += data2T[i * ln_sz + curStep];
					curStep += step;
					++cnt;
				}
			}

			if (cnt > 0)
				*outNum /= cnt;
		}

		virtual void chromAdjust(AVFrame* pvf, const AVPixFmtDescriptor* desc, float chrom, int depth)override
		{
			auto data = reinterpret_cast<T**>(pvf->data);
			int ii, jj, yi, ui, vi, y, yg, ug, vg, u, v, r, g, b, gap1, gap2, * pMin, * pMax, * pMid;
			int minVal = 16 * ((1 << depth) - 1) / 255;
			int maxV = 240 * ((1 << depth) - 1) / 255, maxY = 235 * ((1 << depth) - 1) / 255;
			int mid = (1 << (depth - 1));
			int yi0 = 0, ui0 = 0, vi0 = 0;
			int hStep = (1 << desc->log2_chroma_h), wStep = (1 << desc->log2_chroma_w);
			int yLnSz = pvf->linesize[0] / perSize(), uvLnSz = pvf->linesize[1] / perSize();
			int yp = 0, up = 0, vp = 0, ypg = 0, upg = 0, vpg = 0;

			//auto getR2Y = depth <= 16 ? video::getYUV_BT601_Dep16 : video::getYUV_BT601;
			std::tuple<int, int, int> tp3;
			for (int i = 0; i < pvf->height; i += hStep)
			{
				for (int j = 0; j < pvf->width; j += wStep)
				{
					jj = (j >> desc->log2_chroma_w);
					yi = yi0 + j, ui = ui0 + jj, vi = vi0 + jj;
					y = data[0][yi];
					u = data[1][ui];
					v = data[2][vi];

					//if (y != yp || u != up || v != vp)
					if (std::abs(y - yp) > 6 || std::abs(u - up) > 6 || std::abs(v - vp) > 6)
					{
						yp = y;
						up = u;
						vp = v;
						tp3 = video::getRGB_BT601(y, u, v, mid);
						std::tie(r, g, b) = tp3;

						pMin = (r > g ? &g : &r), pMax = (r > g ? &r : &g);
						pMin = *pMin > b ? &b : pMin;
						pMax = *pMax > b ? pMax : &b;
						for (auto* p : { &r,&g,&b })
							if (p != pMin && p != pMax)
								pMid = p;

						gap1 = ((*pMax - *pMid));
						gap2 = ((*pMid - *pMin));
						gap1 *= chrom;
						gap2 *= chrom;
						/*if (gap1 == 0 && gap2 == 0)
							continue;*/

						*pMax += gap1;
						*pMin -= gap2;
						tp3 = video::getYUV_BT601(r, g, b, mid);
						std::tie(yg, ug, vg) = tp3;
						ypg = yg = clamp(yg, minVal, maxY) - y;
						upg = ug = clamp(ug, minVal, maxV) - u;
						vpg = vg = clamp(vg, minVal, maxV) - v;
					}
					else
					{
						yg = ypg;
						ug = upg;
						vg = vpg;
					}

					for (int k1 = 0; k1 < hStep; ++k1)
					{
						int yi2 = yi + k1 * yLnSz;
						for (int k2 = 0; k2 < wStep; ++k2)
						{
							data[0][yi2 + k2] += yg;
						}
					}
					data[1][ui] += ug;
					data[2][vi] += vg;
				}
				yi0 += (yLnSz << desc->log2_chroma_h), ui0 += uvLnSz, vi0 += uvLnSz;
			}
		}

	private:
		VideoManager() = default;

		VideoManager(const VideoManager<T>&) = delete;
		VideoManager& operator=(const VideoManager<T>&) = delete;

		friend class IVideoManager;
	};



	inline IVideoManager* IVideoManager::getManagerByDepth(int depth)
	{
		static VideoManager<uint8_t> ls_vmU8;
		static VideoManager<int16_t> ls_vmS16;
		static VideoManager<int32_t> ls_vmS32;
		static std::array<IVideoManager*, 3> ls_vms = { &ls_vmU8, &ls_vmS16, &ls_vmS32 };

		for (int i = 0; i < ls_vms.size(); ++i)
		{
			if (depth <= (ls_vms[i]->perSize() << 3))
				return ls_vms[i];
		}

		return nullptr;
	}
}

#endif
