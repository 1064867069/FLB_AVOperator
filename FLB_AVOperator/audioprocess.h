#ifndef AUDIOPROCESS_H
#define AUDIOPROCESS_H

#include "interfaces.h"
#include "avutils.h"

class FAVPlayer;

class AudioListProcessor : public IAudioFrameProcessor
{
	Q_OBJECT
public:
	explicit AudioListProcessor(QObject* p = nullptr);

	void addProcessor(AProcessSPtr);

	void removeProcessor(IAudioFrameProcessor*);

	virtual FrameSPtr processFrame(FrameSPtr pf, FAVInfo*)override;

private:
	QList<AProcessSPtr> m_listProcessors;
};

class VolumnAdjustProcessor :public IAudioFrameProcessor
{
	Q_OBJECT
public:
	explicit VolumnAdjustProcessor(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr, FAVInfo*)Q_DECL_OVERRIDE;

public slots:
	void onVolChanged(int);
private:

	double m_factor = 1.0;
};

class AudioSpeedProcessor :public IAudioFrameProcessor
{
	Q_OBJECT
public:
	explicit AudioSpeedProcessor(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr, FAVInfo*)Q_DECL_OVERRIDE;

	float setSpeed(float, int);

	float getSpeed()const;

private:
	float m_speed = 1.0;
	int dst_sample = 0;
};

#endif // !AUDIOPROCESS_H

