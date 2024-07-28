#ifndef AVUTILS_H
#define AVUTILS_H

#include <functional>
//#include "avtemplates.h"
#include <QString>

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

struct FAVInfo
{
	QString m_filePath;//文件路径

	int m_vIndx = -1;//视频流索引
	int m_aIndx = -1;//音频流索引

	//音频参数
	int m_nChannel; //声道数
	int m_sampleRate; //采样率
	int m_frameSize; //帧采样点数

	AVSampleFormat m_sampleFmt; //输入采样格式
	AVChannelLayout m_chLayout;

	//视频参数
	AVPixelFormat m_pixFmt;
	AVRational m_aspect_ratio;
	double m_avgfRate;
	int m_width;
	int m_height;

	bool m_isOpen = false;

	void reset()noexcept;

	//暂时返回false，记得改
	bool equalAudioInfo(const FAVInfo&)const;
};


namespace audio {
	using VolAdjstFunc = std::function<void(AVFrame*, const FAVInfo*, double)>;
	VolAdjstFunc getVolAdjustFunc(AVSampleFormat);

	using ResampleFunc = std::function<void(uint8_t * dst, int dst_sample, uint8_t * src, int src_sample, int ch)>;
	ResampleFunc getSpeedResampleFunc(AVSampleFormat);
};

#endif // !AVUTILS_H

