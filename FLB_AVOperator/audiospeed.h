#ifndef AUDIOSPEED_H
#define AUDIOSPEED_H

#include <algorithm>
#include <vector>
#include <QDebug>
#include "avtemplates.h"



namespace audio
{
	template <class T>
	class AudioSpeedScale
	{
	public:
		static constexpr int getRemainSample(int period, float speed);

		static constexpr int getNewSample(int period, float speed);

		static constexpr float getPeriodInRatio(float speed);

	public:
		AudioSpeedScale(int ch, int sample_rate);

		//使用该类时，务必保证out空间足够，返回值代表已输入，输出采样点数。输入采样点序列视为交错模式
		std::pair<int, int> operator()(uint8_t* out, int max_outSample, uint8_t* in, int nb_samples, float speed);

	private:
		std::pair<int, int> process(T* out, int max_outSample, T* in, int nb_samples, float speed);



		int getBestPeriod(T* data, int samp, int minp, int maxp, int skip = 1);

		int downSample(T* out, T* in, int nb_samples, int skip);

		void overlapAdd(int numSamples, T* out, T* rampDown, T* rampUp);




	private:
		std::vector<T> m_bufferIn;

		int m_channels;
		int m_periodMax;
		int m_periodMin;
		float m_speed = 1.0;
	};
}

template<class T>
audio::AudioSpeedScale<T>::AudioSpeedScale(int ch, int sample_rate) : m_channels(ch)
{
	m_periodMax = sample_rate / 65;
	m_periodMin = sample_rate / 400;
}

template<class T>
std::pair<int, int> audio::AudioSpeedScale<T>::operator()(uint8_t* out, int max_outSample,
	uint8_t* in, int nb_samples, float speed)
{
	return this->process(reinterpret_cast<T*>(out), max_outSample, reinterpret_cast<T*>(in), nb_samples, speed);
}

template<class T>
std::pair<int, int> audio::AudioSpeedScale<T>::process(T* out, int max_outSample, T* in, int nb_samples, float speed)
{
	if (speed <= 1e-4 || std::abs(speed - 1.0) < 1e-4)
	{
		memcpy(out, in, nb_samples * m_channels * sizeof(T));
		std::copy(in, in + m_channels * nb_samples, out);
		return { nb_samples, nb_samples };
	}

	T* oo = out, * oi = in;

	m_speed = speed;

	std::vector<T> tempArr;
	T* dwnIn = in;

	if (m_channels > 1)
	{
		m_bufferIn.resize(nb_samples);
		this->downSample(&m_bufferIn[0], in, m_channels * nb_samples, m_channels);
		dwnIn = &m_bufferIn[0];
	}

	int curInPos = 0, curOutPos = 0;
	while (1)
	{
		int skip = std::max(nb_samples / 100, 1);
		tempArr.resize(nb_samples / skip + 1);
		int dwn_samples = this->downSample(&tempArr[0], dwnIn, nb_samples - curInPos, skip);

		//获取最符合当前音频段的周期
		int bestPeriod = this->getBestPeriod(&tempArr[0], tempArr.size() / m_channels, m_periodMin, m_periodMax, skip);

		bestPeriod = this->getBestPeriod(dwnIn, nb_samples - curInPos, std::max(bestPeriod - skip << 2, m_periodMin),
			std::min({ bestPeriod + skip << 2, m_periodMax }));

		if (bestPeriod <= 0)
			break;

		int newSamples = this->getNewSample(bestPeriod, m_speed);
		int remainNum = this->getRemainSample(bestPeriod, m_speed);

		int needInPos = m_speed > 1.0 ?
			curInPos + bestPeriod + newSamples + remainNum : curInPos + newSamples + remainNum;
		int needOutPos = m_speed > 1.0 ? curOutPos + newSamples + remainNum
			: curOutPos + bestPeriod + newSamples + remainNum;

		if (needInPos > nb_samples || needOutPos > max_outSample)
			break;

		if (m_speed < 1.0)
		{
			int size = m_channels * bestPeriod;
			std::copy(in + curInPos * m_channels, in + (bestPeriod + curInPos) * m_channels, out + curOutPos * m_channels);
			curOutPos += bestPeriod;
		}

		overlapAdd(newSamples, out + curOutPos * m_channels, in + curInPos * m_channels,
			in + m_channels * (bestPeriod + curInPos));
		curInPos += newSamples;
		curOutPos += newSamples;
		if (m_speed > 1.0)
		{
			curInPos += bestPeriod;
		}
		auto curIn = in + curInPos * m_channels;

		int curOutCh = curOutPos * m_channels;
		for (int i = 0; i < m_channels * remainNum; ++i)
		{
			*(out + curOutCh++) = (*(curIn++));
		}
		curInPos += remainNum;
		curOutPos += remainNum;

		if (curInPos + bestPeriod > nb_samples)
			break;
		dwnIn = &m_bufferIn[0] + curInPos;
	}

	/*if (curInPos > 0)
	{
		for (int i = 0; i < std::max(curInPos, curOutPos) * m_channels; ++i)
		{
			qDebug() << i << (i < curInPos * m_channels ? oi[i] : 0) << (i < curOutPos * m_channels ? oo[i] : 0);
		}
	}*/

	return { curInPos, curOutPos };
}

template<class T>
constexpr int audio::AudioSpeedScale<T>::getNewSample(int period, float speed)
{
	if (speed < 0.5f) {
		return period * speed / (1.0f - speed);
	}
	else if (speed < 2.0f) {
		return period;
	}
	else
	{
		return period / (speed - 1.0f);
	}
}

template<class T>
constexpr int audio::AudioSpeedScale<T>::getRemainSample(int period, float speed)
{
	if (speed <= 0.5 || speed >= 2.0)
	{
		return 0;
	}
	else if (speed < 1.0f)
	{
		return period * (2.0f * speed - 1.0f) / (1.0f - speed);
	}
	else
	{
		return period * (2.0f - speed) / (speed - 1.0f);
	}
}

template<class T>
constexpr float audio::AudioSpeedScale<T>::getPeriodInRatio(float speed)
{
	int period = 100;
	float newSamples = getNewSample(period, speed);
	float remainNum = getRemainSample(period, speed);

	float nextEnd = speed > 1.0 ?
		period + newSamples + remainNum : newSamples + remainNum;

	return period / nextEnd;
}

template<class T>
int audio::AudioSpeedScale<T>::downSample(T* out, T* in, int samp, int skip)
{
	using UpperType = fmttp::UpperInType<T>::type;

	int dwn_samples = 0;
	if (std::numeric_limits<UpperType>::max() > std::numeric_limits<T>::max())
	{

		for (int i = 0; i < samp; i += skip)
		{

			UpperType sum = 0;
			int cnt = 0;
			for (int j = 0; j < skip && i + j < samp; ++j)
			{
				++cnt;
				sum += in[i + j];
			}

			out[dwn_samples++] = (sum / cnt);

		}
	}
	else
	{
		static constexpr T ls_maxT = std::numeric_limits<T>::max();
		T maxSum = ls_maxT / (skip * m_channels);

		for (int i = 0; i < samp; i += skip)
		{

			T avg1 = 0, avg2 = 0, temp;
			int cnt = 0;
			for (int j = 0; j < skip && i + j < samp; ++j)
			{
				++cnt;
				temp = in[i + j];
				if (std::abs(temp) > maxSum)
				{
					avg1 += temp / skip;
				}
				else
				{
					avg2 += temp;
				}
			}

			avg2 /= cnt;;
			out[dwn_samples++] = (avg1 + avg2);

		}

	}

	return dwn_samples;
}

template<class T>
int audio::AudioSpeedScale<T>::getBestPeriod(T* data, int samp, int minp, int maxp, int skip)
{
	int pMin = std::max(m_periodMin / skip, 1),
		pMax = std::min(m_periodMax / skip, static_cast<int>(samp * getPeriodInRatio(m_speed)));
	if (pMin > pMax)
		return skip == 1 ? -1 : pMax;//期望更多采样点

	int bestPeriod = pMin;

	using UpperType = fmttp::UpperInType<T>::type;
	UpperType minDiff;


	for (int p = pMin; p < pMax + 1; ++p)
	{
		UpperType diff = 0;

		if (std::numeric_limits<UpperType>::max() > std::numeric_limits<T>::max())
		{
			for (int i = 0; i < p; ++i)
			{
				diff += std::abs(data[i] - data[i + p]);
			}
			diff /= p;
		}
		else
		{
			UpperType diff2 = 0, th = std::numeric_limits<T>::max() / p;
			for (int i = 0; i < p; ++i)
			{
				T d1 = data[i], d2 = data[i + p], high = std::max(d1, d2), low = std::min(d1, d2);
				if (d1 + d2 < high && std::numeric_limits<UpperType>::max() - std::abs(low) < high)
				{
					diff2 += th;
				}
				else
				{
					T gap = high - low;
					if (gap < th)
						diff += gap;
					else
						diff2 += gap / p;
				}
			}
			diff /= p;
			diff += diff2;
		}

		if (p == pMin)
		{
			minDiff = diff;
		}
		else if (diff < minDiff)
		{
			bestPeriod = p;
			minDiff = diff;
			//qDebug() << skip << pMin << pMax << bestPeriod << (T)diff;
		}
	}

	return bestPeriod * skip;
}

template<class T>
void audio::AudioSpeedScale<T>::overlapAdd(int numSamples, T* out, T* rampDown, T* rampUp)
{
	T* o, * u, * d;
	int i, t;

	float ra = 1.0 / numSamples;
	for (i = 0; i < m_channels; i++) {
		o = out + i;
		u = rampUp + i;
		d = rampDown + i;

		float r = 0;
		for (t = 0; t < numSamples; t++) {

			*o = (*d * (1 - r) + *u * r);
			o += m_channels;
			d += m_channels;
			u += m_channels;

			r += ra;
		}
	}
}

#endif // !AUDIOSPEED_H

