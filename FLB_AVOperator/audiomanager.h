#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include "avutils.h"
#include "audiospeed.h"

namespace audio
{
	class IAudioManager
	{
	public:
		virtual int perSize()const = 0;

		virtual AVSampleFormat getSampleFmt()const = 0;

		virtual SpeedFunc getSpeedFunc(int, int)const = 0;
	};

	template<class T>
	class AudioManager :public IAudioManager
	{
	public:
		typedef T type;

		explicit AudioManager(AVSampleFormat fmt) :m_fmt(fmt) {}

		virtual int perSize()const override { return sizeof(type); }

		virtual AVSampleFormat getSampleFmt()const override { return m_fmt; }

		virtual SpeedFunc getSpeedFunc(int ch, int spr)const override { return AudioSpeedScale<T>(ch, spr); }

	private:
		AVSampleFormat m_fmt;
	};
};


#endif
