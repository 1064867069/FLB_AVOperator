#ifndef AVOPERATOR_H
#define AVOPERATOR_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>

#include <memory>

#include <SDL.h>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

};

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

struct FAVProcessors
{
	AVFormatContext* m_pInFmtCtx = nullptr;
	AVCodecContext* m_pVDecCtx = nullptr; // 视频解码器的实例
	AVCodecContext* m_pADecCtx = nullptr; // 音频解码器的实例
	AVStream* m_pSrcVideo = nullptr; // 源文件的视频流
	AVStream* m_pSrcAudio = nullptr; // 源文件的音频流

	void reset()noexcept;
};


class FFrame
{
public:
	FFrame();
	~FFrame();

	int decode(AVCodecContext*, AVStream*);

	bool valid()const;

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
};
using FrameSPtr = std::shared_ptr<FFrame>;

class VideoFrameProcesser
{
public:
	explicit VideoFrameProcesser(const FAVInfo*);

	void processFrame(FrameSPtr);

	void reset();

	int size()const;

	int objWidth()const;

	int objHeight()const;

	bool valid()const;

	const uint8_t* getYuvBuffer()const;
private:
	SwsContext* m_pSwsCtx = nullptr;

	FrameSPtr m_spYuvFrame;
	uint8_t* m_pYuvBuffer = nullptr;

	enum AVPixelFormat m_yuvFormat = AV_PIX_FMT_YUV420P;
	int64_t m_lastPts = AV_NOPTS_VALUE;
	int m_objWidth = 0;
	int m_objHeight = 0;

	bool m_valid = false;
	bool m_frameDecoded = false;
};

using VProcessorUPtr = std::unique_ptr<VideoFrameProcesser>;

class FAVFrameBuffer
{
public:
	FAVFrameBuffer(size_t cap);

	bool isBeyond()const;

	bool isEmpty()const;

	void waitNotBeyond()const;

	void pushFrame(const FrameSPtr&);

	FrameSPtr popFrame();

	void clear();

private:
	QList<FrameSPtr> m_bufferFrames;
	size_t m_limCapacity;
	int m_curIndex = 0;

	mutable QMutex m_mutex;
	mutable QWaitCondition m_condNotBeyond;
};

using BufferUPtr = std::unique_ptr<FAVFrameBuffer>;

class FAVFileReader : public QObject
{
	Q_OBJECT
public:
	explicit FAVFileReader(QObject* p = nullptr);

	virtual ~FAVFileReader();

	bool openFile(const QString&);

	void readFrames();

	void stop();

	FrameSPtr popAudioFrame();

	FrameSPtr popVideoFrame();

	void seekSecond(double);

	const FAVInfo* getInfo()const;

	void reset()noexcept;

	bool decoding()const;
signals:
	void durationSecondChanged(double);

	void decodeEnd();

	void seekFinished();


private:
	double getPreciousDurationSecond();

	bool checkAndSeek();

	void waitNotBeyond();

	void decodePacket(AVCodecContext* dec_ctx, AVPacket* pkt, AVStream* strm);
private:
	BufferUPtr m_upAudioBuffer = nullptr;
	BufferUPtr m_upVideoBuffer = nullptr;

	mutable QMutex m_mutex;
	QWaitCondition m_condStop;

	FAVProcessors m_procs;
	FAVInfo m_info;

	int64_t m_aDuration = AV_NOPTS_VALUE;
	int64_t m_vDuration = AV_NOPTS_VALUE;

	double m_seekSecond = -1;

	bool m_stop = true;
	bool m_decoding = false;
};

using ReaderSPtr = std::shared_ptr<FAVFileReader>;

class FAVPlayer;
class AudioSDLPlayer :public QObject
{
	Q_OBJECT
public:
	static AudioSDLPlayer* getInstance();

	void pause();
	void cont();
	void reset();

	void bindAVPlayer(FAVPlayer*);
	void unBindAVPlayer(FAVPlayer*);

	bool audioOpen()const;

signals:
	void avPlayerBindChanged(FAVPlayer*);

private:
	ReaderSPtr m_spReader;
	FAVPlayer* m_pBindPlayer = nullptr;

	SDL_AudioSpec m_spec;
	SwrContext* m_pSwrCtx = nullptr;

	// std::vector<unsigned char> m_outBuffer;
	unsigned char* m_pOutBuffer = nullptr;
	int m_outBufferSize;
	AVSampleFormat m_outSampleFmt = AV_SAMPLE_FMT_S16;

	bool m_isPaused = true;

	mutable QMutex m_mutex;
	mutable QMutex m_mutexPause;
private:
	AudioSDLPlayer(QObject* p = nullptr);
	AudioSDLPlayer(const AudioSDLPlayer&) = delete;
	AudioSDLPlayer& operator=(const AudioSDLPlayer&) = delete;
	~AudioSDLPlayer();

	void resetNoLock();

	bool changeReader(ReaderSPtr);
	void unbindReader(const ReaderSPtr&);

	static void fill_audio(void* para, uint8_t* stream, int len);
};

enum class PlayState
{
	Play,
	Pause,
	Stop
};

class VideoOpenGLPlayer;
class FAVPlayer :public QObject
{
	Q_OBJECT
public:
	explicit FAVPlayer(QObject* p = nullptr);

	virtual ~FAVPlayer();

	bool openFile(const QString&);

	void stop();

	//void pause(bool);

	void pause();

	void playCont();

	PlayState state()const;

	double getCurSecond()const;

	double getDuration()const;

	bool getAudioEnd()const;

public slots:
	void seekProp(double);

signals:
	void secondChanged(double);

	void durationChanged(double);

	void started();

	void stopped();

private slots:
	void setCurSecond(double);

	void setDuration(double);

	void onSeekFinished();

	void onAudioEnd();

	void onVideoEnd();

	void check_stop();

private:
	VideoOpenGLPlayer* m_pVideoPlayer = nullptr;
	ReaderSPtr m_spReader;
	PlayState m_state = PlayState::Stop;

	QThread m_threadDecode;

	qint64 m_lastCntTime = -1;
	double m_durationSecond = -1;
	mutable double m_curSecond = 0;

	bool m_audioEnd = true;
	bool m_videoEnd = true;

	friend class AudioSDLPlayer;
};

#endif // !AVOPERATOR_H

