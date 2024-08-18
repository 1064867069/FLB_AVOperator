#include "avutils.h"
//#include "avoperator.h"
#include "avtemplates.h"
#include "audiospeed.h"
#include "audiomanager.h"
#include "videomanager.h"

#include<array>
#include<QMutexLocker>

void tst(const FAVInfo* i)
{

}


namespace audio
{
	AMPtr getManagerPtr(AVSampleFormat fmt)
	{
		if (fmt == AV_SAMPLE_FMT_U8 || fmt == AV_SAMPLE_FMT_U8P)
			return std::make_unique<AudioManager<uint8_t>>(fmt);
		else if (fmt == AV_SAMPLE_FMT_S16 || fmt == AV_SAMPLE_FMT_S16P)
			return std::make_unique<AudioManager<int16_t>>(fmt);
		else if (fmt == AV_SAMPLE_FMT_S32 || fmt == AV_SAMPLE_FMT_S32P)
			return std::make_unique<AudioManager<int32_t>>(fmt);
		else if (fmt == AV_SAMPLE_FMT_S64 || fmt == AV_SAMPLE_FMT_S64P)
			return std::make_unique<AudioManager<int64_t>>(fmt);
		else if (fmt == AV_SAMPLE_FMT_FLT || fmt == AV_SAMPLE_FMT_FLTP)
			return std::make_unique<AudioManager<float>>(fmt);
		else if (fmt == AV_SAMPLE_FMT_DBL || fmt == AV_SAMPLE_FMT_DBLP)
			return std::make_unique<AudioManager<double>>(fmt);

		return nullptr;
	}

	VolAdjstFunc getVolAdjustFunc(AVSampleFormat fmt)
	{

		if (fmt == AV_SAMPLE_FMT_U8 || fmt == AV_SAMPLE_FMT_U8P)
			return VolProcessFunc<uint8_t>();
		else if (fmt == AV_SAMPLE_FMT_S16 || fmt == AV_SAMPLE_FMT_S16P)
			return VolProcessFunc<int16_t>();
		else if (fmt == AV_SAMPLE_FMT_S32 || fmt == AV_SAMPLE_FMT_S32P)
			return VolProcessFunc<int32_t>();
		else if (fmt == AV_SAMPLE_FMT_S64 || fmt == AV_SAMPLE_FMT_S64P)
			return VolProcessFunc<int64_t>();
		else if (fmt == AV_SAMPLE_FMT_FLT || fmt == AV_SAMPLE_FMT_FLTP)
			return VolProcessFunc<float>();
		else if (fmt == AV_SAMPLE_FMT_DBL || fmt == AV_SAMPLE_FMT_DBLP)
			return VolProcessFunc<double>();

		return VolAdjstFunc();
	}

	ResampleFunc getSpeedResampleFunc(AVSampleFormat fmt)
	{
		if (fmt == AV_SAMPLE_FMT_U8 || fmt == AV_SAMPLE_FMT_U8P)
			return speedResample<uint8_t>;
		else if (fmt == AV_SAMPLE_FMT_S16 || fmt == AV_SAMPLE_FMT_S16P)
			return speedResample<int16_t>;
		else if (fmt == AV_SAMPLE_FMT_S32 || fmt == AV_SAMPLE_FMT_S32P)
			return speedResample<int32_t>;
		else if (fmt == AV_SAMPLE_FMT_S64 || fmt == AV_SAMPLE_FMT_S64P)
			return speedResample<int64_t>;
		else if (fmt == AV_SAMPLE_FMT_FLT || fmt == AV_SAMPLE_FMT_FLTP)
			return speedResample<float>;
		else if (fmt == AV_SAMPLE_FMT_DBL || fmt == AV_SAMPLE_FMT_DBLP)
			return speedResample<double>;

		return ResampleFunc();
	}

	SpeedFunc getSpeedFunc(AVSampleFormat fmt, int ch, int sampleRate)
	{
		if (ch < 1 || sampleRate < 20)
			return SpeedFunc();

		if (fmt == AV_SAMPLE_FMT_U8 || fmt == AV_SAMPLE_FMT_U8P)
			return AudioSpeedScale<uint8_t>(ch, sampleRate);
		else if (fmt == AV_SAMPLE_FMT_S16 || fmt == AV_SAMPLE_FMT_S16P)
			return AudioSpeedScale<int16_t>(ch, sampleRate);
		else if (fmt == AV_SAMPLE_FMT_S32 || fmt == AV_SAMPLE_FMT_S32P)
			return AudioSpeedScale<int32_t>(ch, sampleRate);
		else if (fmt == AV_SAMPLE_FMT_S64 || fmt == AV_SAMPLE_FMT_S64P)
			return AudioSpeedScale<int64_t>(ch, sampleRate);
		else if (fmt == AV_SAMPLE_FMT_FLT || fmt == AV_SAMPLE_FMT_FLTP)
			return AudioSpeedScale<float>(ch, sampleRate);
		else if (fmt == AV_SAMPLE_FMT_DBL || fmt == AV_SAMPLE_FMT_DBLP)
			return AudioSpeedScale<double>(ch, sampleRate);

		return SpeedFunc();
	}
}

namespace video
{
	//const std::array<float, 65536> g_ry = getIdxCoe(0.299);
	//const std::array<float, 65536> g_gy = getIdxCoe(0.587);
	//const std::array<float, 65536> g_by = getIdxCoe(0.114);

	//const std::array<float, 65536> g_ru = getIdxCoe(-0.169);
	//const std::array<float, 65536> g_gu = getIdxCoe(-0.331);

	//const std::array<float, 65536> g_gv = getIdxCoe(-0.419);
	//const std::array<float, 65536> g_bv = getIdxCoe(-0.081);

	void seriesProc(uint8_t* data, int size, int per_size, SingleDataFunc func)
	{
		int cur = 0;
		for (int i = 0; i < size; ++i)
		{
			func(&data[cur]);
			cur += per_size;
		}
	}

	VideoMemoryManager* VideoMemoryManager::getInstance()
	{
		static VideoMemoryManager ls_manager;
		return &ls_manager;
	}

	VideoMemoryManager::~VideoMemoryManager()
	{
		QMutexLocker locker(&m_mutex);
		for (auto itr = m_hashPointerSize.begin(); itr != m_hashPointerSize.end(); ++itr)
			m_allocator.deallocate(itr.key(), itr.value());
	}

	bool VideoMemoryManager::allocateImageMem(uint8_t* pointers[4], int linesizes[4],
		int w, int h, enum AVPixelFormat pix_fmt)
	{
		if (linesizes[0] <= 0 || w <= 0 || h <= 0)
			return false;

		// 获取像素格式描述信息
		const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(static_cast<AVPixelFormat>(pix_fmt));
		if (!desc || desc->nb_components <= 0) {
			return false;
		}

		auto vm = IVideoManager::getManagerByDepth(desc->comp[0].depth);
		int sizePerComp = linesizes[0] * h, allSize = sizePerComp;
		int uvw = (w >> desc->log2_chroma_w), uvh = (h >> desc->log2_chroma_h);
		std::array<int, 4> szArr = { 0 };

		szArr[0] = sizePerComp;
		for (int i = 1; i < 4; ++i)
		{
			if (linesizes[i] <= 0)
			{
				szArr[i] = 0;
				break;
			}
			else
			{
				/*float ratioFrom1 = static_cast<float>(linesizes[i]) / linesizes[0];
				ratioFrom1 *= ratioFrom1;*/
				szArr[i] = uvh * linesizes[i];
				allSize += szArr[i];
			}
		}

		QMutexLocker locker(&m_mutex);
		uint8_t* res = m_allocator.allocate(allSize);
		if (res == nullptr)
			return false;

		int cur = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (szArr[i] <= 0)
				break;

			pointers[i] = &res[cur];
			cur += szArr[i];
		}

		m_hashPointerSize[res] = allSize;
		return true;
	}

	void VideoMemoryManager::freeMem(uint8_t* p)
	{
		QMutexLocker locker(&m_mutex);
		if (m_hashPointerSize.find(p) != m_hashPointerSize.end())
		{
			m_allocator.deallocate(p, m_hashPointerSize[p]);
			m_hashPointerSize.remove(p);
		}
	}
}
