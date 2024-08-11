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

#include "interfaces.h"
#include "avutils.h"

struct FAVProcessors
{
	AVFormatContext* m_pInFmtCtx = nullptr;
	AVCodecContext* m_pVDecCtx = nullptr; // 视频解码器的实例
	AVCodecContext* m_pADecCtx = nullptr; // 音频解码器的实例
	AVStream* m_pSrcVideo = nullptr; // 源文件的视频流
	AVStream* m_pSrcAudio = nullptr; // 源文件的音频流

	void reset()noexcept;
};



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

	FrameSPtr m_spLastFrame;
	FrameSPtr m_spYuvFrame;
	uint8_t* m_pYuvBuffer = nullptr;

	enum AVPixelFormat m_yuvFormat = AV_PIX_FMT_YUV420P;
	int m_objWidth = 0;
	int m_objHeight = 0;

	bool m_valid = false;
	bool m_frameDecoded = false;
};

using VFrameProcessorUPtr = std::unique_ptr<VideoFrameProcesser>;

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

class AudioListProcessor;
class VideoProcessList;
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

	void addAudioProcessor(AProcessSPtr);

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
	std::unique_ptr<AudioListProcessor> m_upAudioProcessors;

	BufferUPtr m_upAudioBuffer = nullptr;
	BufferUPtr m_upVideoBuffer = nullptr;

	mutable QMutex m_mutex;
	QWaitCondition m_condStop;

	FAVProcessors m_procs;
	std::shared_ptr<FAVInfo> m_spInfo;

	int64_t m_aDuration = AV_NOPTS_VALUE;
	int64_t m_vDuration = AV_NOPTS_VALUE;

	double m_seekSecond = -1;
	double m_seekDecodeSecond = -1;

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

	void audioEnd(FAVPlayer*);
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
class AudioSpeedProcessor;
class FAVPlayer :public QObject
{
	Q_OBJECT
public:
	explicit FAVPlayer(QObject* p = nullptr);

	virtual ~FAVPlayer();

	bool openFile(const QString&);

	PlayState state()const;

	double getCurSecond()const;

	double getDuration()const;

	bool getAudioEnd()const;

	void addProcessor(const AProcessSPtr&);

	float setSpeed(float);
public slots:
	void stop();

	//void pause(bool);

	void pause();

	void playCont();

	void seekProp(double);

	void seekForward();

	void seekBackward();
signals:
	void secondChanged(double);

	void durationChanged(double);

	void started();

	void stopped();

private slots:
	void setCurSecond(double);

	void setDuration(double);

	void onSeekFinished();

	void onAudioEnd(FAVPlayer*);

	void onVideoEnd();

	void check_stop();

private:
	VideoOpenGLPlayer* m_pVideoPlayer = nullptr;
	ReaderSPtr m_spReader;
	std::shared_ptr<AudioSpeedProcessor> m_spSpeedProc;

	QThread m_threadDecode;

	PlayState m_state = PlayState::Stop;
	qint64 m_lastCntTime = -1;
	double m_durationSecond = -1;
	mutable double m_curSecond = 0;

	bool m_audioEnd = true;
	bool m_videoEnd = true;

	friend class AudioSDLPlayer;
};

#endif // !AVOPERATOR_H

