#ifndef AVUTILS_H
#define AVUTILS_H

#include <functional>
//#include "avtemplates.h"
#include <QString>
#include <QMutex>
#include <QHash>
#include <array>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

};

class IAudioFrameProcessor;
class FFrame
{
public:
	explicit FFrame();

	FFrame(const FFrame&) = delete;

	~FFrame();

	std::shared_ptr<FFrame> deepAFClone()const;

	std::shared_ptr<FFrame> deepVFClone()const;

	const AVFrame* getAVFrame()const;

	FFrame& operator=(const FFrame&) = delete;

	int decode(AVCodecContext*, AVStream*);

	bool valid()const;

	bool isAudio()const;

	bool isVideo()const;

	int64_t getPts()const;

	double getSecond()const;

	AVRational getTimeBase()const;

	int width()const;

	int height()const;

	int swrFrame(SwrContext* swrCtx, unsigned char* outBuf, int max_size);

	int swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame);

	void imageRelate(uint8_t* buffer, AVPixelFormat format, int w, int h);
private:
	AVFrame* m_pFrame;
	//int64_t m_pts = AV_NOPTS_VALUE;
	bool m_valid = false;

	friend class IAudioFrameProcessor;
	friend class IVideoFrameProcessor;
};
using FrameSPtr = std::shared_ptr<FFrame>;


class FAVInfo;

struct FAVInfo
{
	QString m_avPath;//文件路径

	int m_vIndx = -1;//视频流索引
	int m_aIndx = -1;//音频流索引

	//音频参数
	int m_nChannel; //声道数
	int m_sampleRate; //采样率
	int m_frameSize; //帧采样点数
	AVRational m_timeBaseAudio; //时间基
	AVSampleFormat m_sampleFmt; //输入采样格式
	AVChannelLayout m_chLayout;

	//视频参数
	AVPixelFormat m_pixFmt;
	AVRational m_aspect_ratio;
	AVRational m_timeBaseVideo; //时间基
	double m_avgfRate;
	int m_width;
	int m_height;

	bool m_isOpen = false;

	void reset()noexcept;


	//暂时返回false，记得改
	bool equalAudioInfo(const FAVInfo&)const;
};




namespace audio {

	class IAudioManager;
	class FAVInfo;
	using AMPtr = std::unique_ptr<IAudioManager>;
	AMPtr getManagerPtr(AVSampleFormat);

	using VolAdjstFunc = std::function<void(AVFrame*, double)>;
	VolAdjstFunc getVolAdjustFunc(AVSampleFormat);

	using ResampleFunc = std::function<void(uint8_t * dst, int dst_sample, uint8_t * src, int src_sample, int ch)>;
	ResampleFunc getSpeedResampleFunc(AVSampleFormat);

	using SpeedFunc = std::function<std::pair<int, int>(uint8_t * out, int max_outSample, uint8_t * in, int nb_samples, float speed)>;
	SpeedFunc getSpeedFunc(AVSampleFormat, int, int);

	//int getAudioFrameSize(int aIndex, AVFormatContext* inFmtCtx, AVCodecContext* decCtx);
};

namespace video
{
	class VideoMemoryManager
	{
	public:
		static VideoMemoryManager* getInstance();

		bool allocateImageMem(uint8_t* pointers[4], int linesizes[4],
			int w, int h, enum AVPixelFormat pix_fmt);

		void freeMem(uint8_t*);
	private:
		QHash<uint8_t*, int> m_hashPointerSize;
		std::allocator<uint8_t> m_allocator;

		QMutex m_mutex;

	private:
		VideoMemoryManager() = default;
		VideoMemoryManager(const VideoMemoryManager&) = delete;
		VideoMemoryManager& operator=(const VideoMemoryManager&) = delete;

		~VideoMemoryManager();
	};

	using SingleDataFunc = std::function<void(uint8_t*)>;


	void seriesProc(uint8_t* data, int size, int per_size, SingleDataFunc func);
}

namespace video
{
	inline int getYUVMin(int depth)
	{
		return 16 * ((1 << depth) - 1) / 255;
	}

	inline int getYMax(int depth)
	{
		return 235 * ((1 << depth) - 1) / 255;
	}

	inline int getUVMax(int depth)
	{
		return 240 * ((1 << depth) - 1) / 255;
	}

	inline std::tuple<int, int, int> getRGB_BT601(int y, int u, int v, int mid)
	{
		v -= mid;
		u -= mid;
		const int r = y + static_cast<int>(1.4075f * v);
		const int g = y - static_cast<int>(0.3455f * u + 0.7169f * v);
		const int b = y + static_cast<int>(1.779f * u);

		return std::tuple<int, int, int>(r, g, b);
	}

	//inline std::tuple<int, int, int> getYUV_BT601_Dep16(int r, int g, int b)
	//{
	//	const int y = g_ry[r] + g_gy[g] + g_by[b];
	//	const int u = static_cast<int>(g_ru[r] + g_gu[g]) + (b >> 1) + 128;
	//	const int v = (r >> 1) + static_cast<int>(g_gv[g] + g_bv[b]) + 128;

	//	return std::tuple<int, int, int>(y, u, v);
	//}

	inline std::tuple<int, int, int> getYUV_BT601(int r, int g, int b, int mid)
	{
		const int y = 0.299 * r + 0.587 * g + 0.114 * b;
		const int u = static_cast<int>(-0.169 * r - 0.331 * g) + (b >> 1) + mid;
		const int v = (r >> 1) - static_cast<int>(0.419 * g + 0.081 * b) + mid;

		return std::tuple<int, int, int>(y, u, v);
	}
}

#endif // !AVUTILS_H

