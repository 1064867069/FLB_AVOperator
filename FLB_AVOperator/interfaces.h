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
struct AVFrame;
struct FAVInfo;
using FrameSPtr = std::shared_ptr<FFrame>;
class IAudioFrameProcessor : public QObject
{
	Q_OBJECT
public:
	explicit IAudioFrameProcessor(QObject* p = nullptr);

	virtual ~IAudioFrameProcessor() = default;

	virtual FrameSPtr processFrame(FrameSPtr pf, FAVInfo*) = 0;

protected:
	AVFrame* getAVFrame(FFrame*);

	void setFrameValid(FFrame*);
};

using AProcessSPtr = std::shared_ptr<IAudioFrameProcessor>;

#endif // !INTERFACES_H

