#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

#include "avutils.h"
#include "avtemplates.h"

#include <array>
#include <QDebug>

namespace video
{



	class IVideoManager
	{
	public:
		virtual int perSize() = 0;

		virtual SingleDataFunc AddDecPerFunc(int low, int high, float brtCoe) = 0;

		virtual SingleDataFunc midADPerFunc(int low, int high, int mid, float brtCoe) = 0;

		virtual void avr2dData(const uint8_t* data, int* outNum, int w, int h, int ln_sz, int step = 1) = 0;

		static IVideoManager* getManagerByDepth(int);
	};

	template<class T>
	class VideoManager : public IVideoManager
	{
	public:
		using UpperType = typename fmttp::UpperInType<T>::type;

		virtual int perSize()override { return sizeof(T); }

		virtual SingleDataFunc AddDecPerFunc(int low, int high, float brtCoe)override {
			int gap = (high - low) * brtCoe;
			auto func = [low, high, gap](uint8_t* data)
			{
				auto* cur = reinterpret_cast<T*>(data);
				UpperType tmp = *cur;
				tmp = tmp + gap;

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
