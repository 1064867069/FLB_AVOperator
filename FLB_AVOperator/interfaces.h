#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>
#include <QSlider>
#include <QMutex>
#include <QWaitCondition>

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

#endif // !INTERFACES_H

