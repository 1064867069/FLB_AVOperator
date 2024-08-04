#include "avutils.h"
//#include "avoperator.h"
#include "avtemplates.h"
#include "audiospeed.h"
#include "audiomanager.h"

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
