#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>
#include <QSlider>
#include <QMutex>
#include <QWaitCondition>

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

class FAVPlayer;

class ITimeObserver :public QObject
{
	Q_OBJECT
public:
	explicit ITimeObserver(FAVPlayer* pObj, QObject* par = nullptr);

protected slots:

	virtual void onSecondChanged(double) = 0;

	virtual	void onDurationChanged(double) = 0;



protected:
	double m_second = -1;
	double m_duration = -1;
};


class FFrame;
using FrameSPtr = std::shared_ptr<FFrame>;

struct FAVInfo;
struct AVFrame;
enum class FAVProcessState;

class IAudioFrameProcessor : public QObject
{
	Q_OBJECT
public:
	explicit IAudioFrameProcessor(QObject* p = nullptr);

	virtual ~IAudioFrameProcessor() = default;

	////是否剩余完整的帧缓存
	//virtual bool restWhole();

	virtual FrameSPtr getRestFrame(bool);

	//无效帧和不处理帧会原样返回
	virtual FrameSPtr processFrame(FrameSPtr) = 0;

protected:
	AVFrame* getAVFrame(FFrame*);

	void setFrameValid(FFrame*);

	FrameSPtr createNewFrameBySample(const FAVInfo*, int);
};

using AProcessSPtr = std::shared_ptr<IAudioFrameProcessor>;

class IVideoFrameProcessor : public QObject
{
	Q_OBJECT
public:
	explicit IVideoFrameProcessor(QObject* p = nullptr);

	//无效帧和不处理帧会原样返回
	virtual FrameSPtr processFrame(FrameSPtr) = 0;

	virtual bool skip()const;
protected:
	AVFrame* getAVFrame(FFrame*);

	void setFrameValid(FFrame*);

	// FrameSPtr createNewFrame(const FAVInfo*);
};
using VProcessSPtr = std::shared_ptr<IVideoFrameProcessor>;

class IAVPathManager : public QObject
{
	Q_OBJECT
public:
	const QStringList& getPaths()const;

	const QStringList& getNames()const;

	void setDlgParent(QWidget*);

public slots:
	virtual void importPath() = 0;

	virtual bool delFile(int) = 0;

signals:
	void newfp(int);

protected:
	QStringList m_listFPath;
	QStringList m_listFName;
	QString m_sqlTable;

	QWidget* m_parent = nullptr;
};

class FAVInfo;
class FAVProcessors;
class FAVPlayer;
class VideoOpenGLPlayer;
class AudioListProcessor;
class FAVFrameBuffer;

class IAVReader;
using ReaderSPtr = std::shared_ptr<IAVReader>;
using BufferUPtr = std::unique_ptr<FAVFrameBuffer>;

class AVStreamBlock;
class StreamManager;
class IFAVStream;
class IAudioStream;
class IVideoStream;

using StreamPtr = std::shared_ptr<IFAVStream>;
using AudioStreamPtr = std::shared_ptr<IAudioStream>;
using VideoStreamPtr = std::shared_ptr<IVideoStream>;
using VideoStreamWPtr = std::weak_ptr<IVideoStream>;
class IFAVStream : public QObject
{
	Q_OBJECT
public:
	IFAVStream() = default;

	virtual ~IFAVStream() = default;

	IFAVStream(const IFAVStream&) = delete;

	IFAVStream& operator=(const IFAVStream&) = delete;

	virtual FrameSPtr getNxtFrames() = 0;

	virtual void setStart(double s);

	virtual void setEnd(double e);

	virtual void seekSecond(double sec) = 0;

	virtual StreamPtr clone()const = 0;

	virtual QBrush backgroundBrush()const;

	virtual double startMin()const;

	virtual double endMax()const;

	virtual  void moveHorizon(double gap);

	virtual void bindStreamManager(const std::shared_ptr<StreamManager>& spManager);

	virtual void connectBlock(AVStreamBlock* pBlock);

	bool valid()const;

	bool inTimeRange(double sec)const;

	double timeStart()const;

	double timeEnd()const;

signals:
	void startChanged(double);
	void endChanged(double);

	void needRefresh();
protected:
	std::weak_ptr<StreamManager> m_wpStreamManager;

	double m_timeStart = 0;
	double m_timeEnd = 0;
	bool m_valid = false;
};

class AudioResampleProcessor;
class VolumnAdjustProcessor;
class AudioListProcessor;
class IAudioStream : public IFAVStream
{
	Q_OBJECT
public:
	IAudioStream();

	void setOutParam(int newOutRate, AVSampleFormat dstFmt, AVChannelLayout out_channel);

	float getVolProp()const;

protected:
	std::unique_ptr<AudioResampleProcessor> m_upSwrProcessor;
	std::unique_ptr<AudioListProcessor> m_upListProcs;
	std::shared_ptr<VolumnAdjustProcessor> m_spVolProcessor;

	AVSampleFormat m_inFormat = AV_SAMPLE_FMT_NONE;
	AVChannelLayout m_inChannels;
	int m_inSampleRate = 0;
};

class VideoStreamRectSetWidget;
class VideoOpenGLPlayer;
class IVideoStream : public IFAVStream, public std::enable_shared_from_this<IVideoStream>
{
	Q_OBJECT
public:
	IVideoStream();

	virtual ~IVideoStream()override;

	virtual void connectBlock(AVStreamBlock* pBlock)override;

	virtual void bindStreamManager(const std::shared_ptr<StreamManager>& spManager)override;

	virtual void onOutParamUpdated();

	virtual FrameSPtr swsObjFrame(const FrameSPtr& spf);

	void bindVideoPlayer(VideoOpenGLPlayer* pPlayer);

	VideoStreamPtr getSharedPointer();

	void widgetShowStreamFrame(const FrameSPtr&);

	void setPixelFormat(AVPixelFormat pixFmt);

	void setFrameRate(int fr);

	void setLevel(int level);

	void setRemoved(bool removed = true);

	void hideRectWidget();

	void cpyFrom(const IVideoStream* pStream);

	bool removed()const;

	int width()const;

	int height()const;

	int xPos()const;

	int yPos()const;

	int level()const;

	int version()const;
signals:
	void StreamWillAdjust();

	void StreamAdjustWillFinished();

	void streamWillAdjustThis(IVideoStream*);
protected slots:
	void onRectPropChanged(const QRectF&);

	void onBlockPressed();

	void initStreamRectWidget();

	void onWillAdjust();

	void updateVersion();
protected:
	VideoStreamRectSetWidget* m_pRectSetWidget = nullptr;
	AVPixelFormat m_pixelFormat = AV_PIX_FMT_YUV420P;
	QRect m_rect;
	int m_frameRate = 20;
	int m_xPos = 0;
	int m_yPos = 0;
	int m_width = 720;
	int m_height = 480;

	int m_rectVersion = 0;
	int m_level = 0;
	bool m_bRemoved = true;
};

inline uint qHash(const StreamPtr& key, uint seed) {
	return qHash(key.get(), seed);
}

inline uint qHash(const AudioStreamPtr& key, uint seed) {
	return qHash(key.get(), seed);
}

inline uint qHash(const VideoStreamPtr& key, uint seed) {
	return qHash(key.get(), seed);
}

//using StreamPtr = std::shared_ptr<IFAVStream>;
//using AudioStreamPtr = std::shared_ptr<IAudioStream>;
//using VideoStreamPtr = std::shared_ptr<IVideoStream>;
//using VideoStreamWPtr = std::weak_ptr<IVideoStream>;

class IAVResourceFactory
{
public:
	virtual QString fileFilter() { return ""; }

	virtual QString title() = 0;

	virtual QList<StreamPtr> createStreams(const QString& desc) = 0;
};

#endif // !INTERFACES_H

