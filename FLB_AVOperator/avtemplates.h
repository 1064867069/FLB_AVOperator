#ifndef AVTEMPLATES_H
#define AVTEMPLATES_H

#include "avutils.h"
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

};

class FAVInfo;

namespace fmttp
{
	template<class T>
	class UpperInType
	{
	public:
		typedef int64_t type;
		static_assert(std::numeric_limits<T>::max() <= std::numeric_limits<type>::max()
			&& std::numeric_limits<T>::lowest() >= std::numeric_limits<type>::lowest(), "range of type smaller than T");
	};

	template<>
	class UpperInType<float>
	{
	public:
		typedef double type;

	};

	template<>
	class UpperInType<double>
	{
	public:
		typedef long double type;

	};

	template<>
	class UpperInType<uint8_t>
	{
	public:
		typedef int32_t type;

	};

	template<>
	class UpperInType<int16_t>
	{
	public:
		typedef int32_t type;

	};


}

template <class T>
class VolProcessFunc
{
public:
	void operator()(AVFrame*, double factor);
};

template <class T>
void VolProcessFunc<T>::operator()(AVFrame* pframe, double factor)
{
	//factor = 1.0;
	//static_assert(sizeof(H) > sizeof(T));
	if (pframe->nb_samples <= 0 || pframe->format == AV_SAMPLE_FMT_NONE)
		return;

	T maxV = std::numeric_limits<T>::max();
	T minV = std::numeric_limits<T>::lowest();
	int channels = pframe->ch_layout.nb_channels;
	if (factor > 1)
	{
		maxV = maxV / factor;
		minV = minV / factor;
	}
	if (av_sample_fmt_is_planar(static_cast<AVSampleFormat>(pframe->format)))
	{
		for (int i = 0; i < channels; ++i)
		{
			T* data = reinterpret_cast<T*>(pframe->data[i]);

			for (int j = 0; j < pframe->nb_samples; ++j)
			{
				if (data[j] > maxV)
				{
					data[j] = std::numeric_limits<T>::max();
				}
				else if (data[j] < minV)
				{
					data[j] = std::numeric_limits<T>::min();
				}
				else
				{
					data[j] *= factor;
				}
				//qDebug() << "新：" << data[j] << data[j] - t2;
			}
		}
	}
	else
	{
		T* data = reinterpret_cast<T*>(pframe->data[0]);
		for (int i = 0; i < channels * pframe->nb_samples; ++i)
		{
			if (data[i] > maxV)
			{
				data[i] = std::numeric_limits<T>::max();
			}
			else if (data[i] < minV)
			{
				data[i] = std::numeric_limits<T>::min();
			}
			else
			{
				data[i] *= factor;
			}
		}
	}
}

namespace audio
{
	template <class T>
	void speedResample(uint8_t* dst, int dst_sample, uint8_t* src, int src_sample, int ch)
	{
		if (dst == nullptr || src == nullptr)
		{
			qDebug() << __FUNCTION__ << "audio data is null!";
			return;
		}

		if (src_sample <= 0 || dst_sample <= 0)
		{
			qDebug() << __FUNCTION__ << "num of sample is invalid to resample!";
			return;
		}

		T* srcData = reinterpret_cast<T*>(src);
		T* dstData = reinterpret_cast<T*>(dst);
		float srcPGap = static_cast<float>(src_sample) / dst_sample;
		float w2AddGap = 1.0 / dst_sample;

		for (int i = 0; i < ch; ++i)
		{
			float w2 = 0.0, curSrcPos = 0.0;
			auto curDst = &dstData[i];
			auto curSrc = &srcData[i];
			for (int j = 0; j < dst_sample; j += ch, curSrcPos += srcPGap, w2 += w2AddGap)
			{
				int ls = static_cast<int>(curSrcPos);
				int hs = ls >= src_sample - 1 ? ls : ls + 1;

				curDst[j] = curSrc[ls * ch] * (curSrcPos - ls) + curSrc[hs * ch] * (hs - curSrcPos);
				if (src_sample > dst_sample)
				{
					curDst[j] = curSrc[j] * (1 - w2) + curDst[j] * w2;
				}
				else if (j < src_sample)
				{
					curDst[j] = curSrc[j] * (1 - w2) + curDst[j] * w2;
				}
			}
		}
	}


}

namespace video
{

}

#endif // !AVTEMPLATES_H

