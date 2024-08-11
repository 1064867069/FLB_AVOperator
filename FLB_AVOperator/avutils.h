#ifndef AVUTILS_H
#define AVUTILS_H

#include <functional>
//#include "avtemplates.h"
#include <QString>
#include <QMutex>
#include <QHash>

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
	QString m_filePath;//文件路径

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

#endif // !AVUTILS_H

