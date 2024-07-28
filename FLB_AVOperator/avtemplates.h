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


template <class T>
class VolProcessFunc
{
public:
	void operator()(AVFrame*, const FAVInfo*, double factor);
};

template <class T>
void VolProcessFunc<T>::operator()(AVFrame* pframe, const FAVInfo* pinfo, double factor)
{
	//factor = 1.0;
	//static_assert(sizeof(H) > sizeof(T));
	if (pinfo->m_aIndx < 0 || pframe->nb_samples <= 0)
		return;

	T maxV = std::numeric_limits<T>::max();
	T minV = std::numeric_limits<T>::lowest();
	if (factor > 1)
	{
		maxV = maxV / factor;
		minV = minV / factor;
	}
	if (av_sample_fmt_is_planar(pinfo->m_sampleFmt))
	{
		for (int i = 0; i < pinfo->m_nChannel; ++i)
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
		for (int i = 0; i < pinfo->m_nChannel * pframe->nb_samples; ++i)
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


#endif // !AVTEMPLATES_H

