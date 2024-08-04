#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>
#include <QSlider>

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



#endif // !INTERFACES_H

