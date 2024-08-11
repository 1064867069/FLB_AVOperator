#ifndef AUDIOPROCESS_H
#define AUDIOPROCESS_H

#include "interfaces.h"
#include "avutils.h"

#include <queue>
#include <QMutex>

class FAVPlayer;

enum class FAVProcessState
{
	None,
	Invalid,
	Success,
	NeedMore,
	MustPop
};

class AudioListProcessor : public IAudioFrameProcessor
{
	Q_OBJECT
public:
	explicit AudioListProcessor(QObject* p = nullptr);

	void addProcessor(AProcessSPtr);

	void removeProcessor(IAudioFrameProcessor*);

	bool lastNone()const;

	virtual FrameSPtr getRestFrame(bool)Q_DECL_OVERRIDE;

	//有的处理器可能会保留一部分，传入空帧则会强行弹出
	virtual FrameSPtr processFrame(FrameSPtr)override;

private:
	QList<AProcessSPtr> m_listProcessors;

	bool m_lastNone = true;
};

class VolumnAdjustProcessor :public IAudioFrameProcessor
{
	Q_OBJECT
public:
	explicit VolumnAdjustProcessor(QObject* p = nullptr);



	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

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

	virtual FrameSPtr getRestFrame(bool)Q_DECL_OVERRIDE;

	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

	float setSpeed(float, int);
	void setAVInfo(const FAVInfo&);

	float getSpeed()const;

	void reset();

private:
	void inputFrame(AVFrame*);

	void outputFrame(AVFrame*);

	void reallocOutBuffer();

	void processInBuffer();

	FrameSPtr tryPopOutBuffer();

	FrameSPtr forcePopOutBuffer();

private:
	std::vector<uint8_t> m_bufferOutSamples;
	std::vector<uint8_t> m_bufferInSamples;

	std::queue<int64_t> m_queuePts;

	QMutex m_mutex;

	FAVInfo m_info;
	audio::AMPtr m_upAudioManager;
	audio::SpeedFunc m_func;

	float m_speed;
	int m_dstSample;
	int m_numInSample = 0;
	int m_numOutSample = 0;
	int64_t m_lastEndPts = AV_NOPTS_VALUE;
	int64_t m_duration = AV_NOPTS_VALUE;
};

#endif // !AUDIOPROCESS_H

