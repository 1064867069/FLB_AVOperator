#ifndef AVUTILS_H
#define AVUTILS_H

#include <functional>
//#include "avtemplates.h"
#include <QString>
#include <QMutex>
#include <QHash>
#include <array>
#include <vector>

#include "interfaces.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
};

enum class FrameFlag
{
	NormalFrame,
	PadFrame
};

class IAudioFrameProcessor;
class FFrame
{
public:
	explicit FFrame();

	FFrame(const FFrame&) = delete;

	FFrame(AVSampleFormat fmt, AVChannelLayout ch_layout, int sample_rate, int nb_samples);

	FFrame(AVPixelFormat fmt, int height, int width, const QString& clr = "#000000");

	virtual ~FFrame();

	std::shared_ptr<FFrame> deepAFClone()const;

	virtual std::shared_ptr<FFrame> deepVFClone()const;

	const AVFrame* getAVFrame()const;

	FFrame& operator=(const FFrame&) = delete;

	int decode(AVCodecContext*, AVStream*);

	int send2Encoder(AVCodecContext* encCtx);

	void setPts(int64_t pts);

	void setPtsSecond(double sec);

	void setTimeBase(int num, int den);

	void setDuration(int d);

	void copy2AudioBufferPlanar(std::vector<std::vector<uint8_t>>& buffer, double second = -1);

	void fromAudioBufferPlanar(std::vector<std::vector<uint8_t>>& buffer, float factor);

	void drawFrom(FrameSPtr other, int x = 0, int y = 0);

	void outPutSample();

	void outPutImage(int c = 0, int step = 15);

	void setExternInfo(const QString& info);

	void setFlag(FrameFlag flag);

	QImage toQImage()const;

	FrameFlag flag()const;

	bool valid()const;

	bool isAudio()const;

	bool isVideo()const;

	int getSample()const;

	int64_t getPts()const;

	double getSecond()const;

	double getDurationSecond()const;

	double getEndSecond()const;

	AVRational getTimeBase()const;

	int width()const;

	int height()const;

	int swrFrame(SwrContext* swrCtx, unsigned char* outBuf, int max_size);

	int swrFrame(SwrContext* swrCtx, AVFrame* other, int max_size);

	virtual int swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame);

	void imageRelate(uint8_t* buffer, AVPixelFormat format, int w, int h);

protected:
	AVFrame* allocVideoFrame(AVPixelFormat fmt, int height, int width);

	void fillColor(const QColor& color);
protected:
	AVFrame* m_pFrame;
	FrameFlag m_flag = FrameFlag::NormalFrame;
	//int64_t m_pts = AV_NOPTS_VALUE;
	bool m_valid = false;

	QString m_externInfo;
	friend class IAudioFrameProcessor;
	friend class IVideoFrameProcessor;
};
using FrameSPtr = std::shared_ptr<FFrame>;

class VideoFrameSuperPosed : public QObject, public FFrame
{
	Q_OBJECT
public:
	VideoFrameSuperPosed(AVPixelFormat fmt, int height, int width, const QString& clr = "");

	//virtual ~VideoFrameSuperPosed()override;

	virtual std::shared_ptr<FFrame> deepVFClone()const override;

	virtual int swsVideoFrame(SwsContext* swsCtx, int sh, std::shared_ptr<FFrame> yuvFrame)override;

	void addStreamFrame(const VideoStreamPtr& spStream, const FrameSPtr& spFrame);

	void redrawWhole();

	void redrawHide();
public slots:
	void hideStreamFrame(IVideoStream* pStream);

	void cancelHidden();

	void onStreamRemoved(VideoStreamPtr spStream);

	QList<VideoStreamPtr> sortStreamLevel();

	bool checkVersion()const;
private:
	QHash<VideoStreamPtr, FrameSPtr> m_hashStreamFrame;
	QHash<VideoStreamPtr, int> m_hashDrawnVersion;
	IVideoStream* m_pStreamHide = nullptr;
	QColor m_clrFill;
	bool m_drawnByHide = false;
};


class FrameSafeUtils
{
public:
	static FrameSafeUtils& getInstance();

	template<class ...Args>
	FrameSPtr make_shared_frame(Args&&... _args)
	{
		QMutexLocker locker(&m_mutex);
		return std::make_shared<FFrame>(std::forward<Args>(_args)...);
	}

private:
	FrameSafeUtils() = default;
	FrameSafeUtils(const FrameSafeUtils&) = delete;
	FrameSafeUtils& operator=(const FrameSafeUtils&) = delete;

private:
	QMutex m_mutex;
};

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
	QList<AVSampleFormat> getSupportedSampleFormats(const AVCodec* pCodec);

	AVCodecID getAudioCodecID(const QString& suffix);
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

	inline int getYUVMid(int depth)
	{
		return 1 << (depth - 1);
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

	inline std::tuple<int, int, int> getRGB_BT601_DV(int y, int u, int v, int depth)
	{
		int mid = getYUVMid(depth);
		v -= mid;
		u -= mid;
		y -= getYUVMin(depth);
		y *= 1.164f;
		const int r = y + static_cast<int>(y + 1.596f * v);
		const int g = y - static_cast<int>(y - 0.392f * u - 0.813 * v);
		const int b = y + static_cast<int>(y + 2.017 * u);

		return std::tuple<int, int, int>(r, g, b);
	}

	inline std::tuple<int, int, int> getYUV_BT601(int r, int g, int b, int mid)
	{
		const int y = 0.299f * r + 0.587f * g + 0.114f * b;
		const int u = static_cast<int>(-0.169f * r - 0.331f * g) + (b >> 1) + mid;
		const int v = (r >> 1) - static_cast<int>(0.419f * g + 0.081f * b) + mid;

		return std::tuple<int, int, int>(y, u, v);
	}

	inline std::tuple<int, int, int> getYUV_BT601_DV(int r, int g, int b, int depth)
	{
		int mid = getYUVMid(depth);
		const int y = 0.257f * r + 0.504f * g + 0.098f * b + getYUVMin(depth);
		const int u = -0.148f * r - 0.291f * g + 0.439f * b + mid;
		const int v = 0.439f * r - 0.368f * g - 0.071f * b + mid;

		return std::tuple<int, int, int>(y, u, v);
	}
}

#endif // !AVUTILS_H

