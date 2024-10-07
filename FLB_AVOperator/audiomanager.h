#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include "avutils.h"
#include "audiospeed.h"
#include "avtemplates.h"

namespace audio
{
	class IAudioManager
	{
	public:
		virtual int perSize()const = 0;

		virtual AVSampleFormat getSampleFmt()const = 0;

		virtual SpeedFunc getSpeedFunc(int, int)const = 0;

		virtual void Add2OutBuffer(uint8_t* outBuffer, const uint8_t* inBuffer, float factor, int size) = 0;
	};

	template<class T>
	class AudioManager :public IAudioManager
	{
		using UpperType = typename fmttp::UpperInType<T>::type;
	public:
		//typedef T type;

		explicit AudioManager(AVSampleFormat fmt) :m_fmt(fmt) {}

		virtual int perSize()const override { return sizeof(T); }

		virtual AVSampleFormat getSampleFmt()const override { return m_fmt; }

		virtual SpeedFunc getSpeedFunc(int ch, int spr)const override { return AudioSpeedScale<T>(ch, spr); }

		virtual void Add2OutBuffer(uint8_t* outBuffer, const uint8_t* inBuffer, float factor, int size)override
		{
			if (factor < 1e-4)
				return;
			auto dataOut = reinterpret_cast<T*>(outBuffer);
			auto dataInput = reinterpret_cast<const T*>(inBuffer);

			for (int i = 0; i < size; ++i)
			{
				UpperType temp = dataOut[i] + dataInput[i] * factor;
				temp = std::min(m_maxVal, temp);
				temp = std::max(m_minVal, temp);
				dataOut[i] = temp;
			}
		}

	private:
		AVSampleFormat m_fmt;
		static constexpr UpperType m_maxVal = std::numeric_limits<T>::max();
		static constexpr UpperType m_minVal = std::numeric_limits<T>::lowest();
	};
};


#endif
