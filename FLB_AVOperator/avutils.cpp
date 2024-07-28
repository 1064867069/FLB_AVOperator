#include "avutils.h"
//#include "avoperator.h"
#include "avtemplates.h"


void tst(const FAVInfo* i)
{

}


namespace audio
{
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
}
