#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include "interfaces.h"
#include <QList>

class VideoProcessList : public IVideoFrameProcessor
{
public:
	VideoProcessList(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

	void addVProcessor(const VProcessSPtr&);

private:
	QList<VProcessSPtr> m_listProcs;
};

class VideoBrightAdjust : public IVideoFrameProcessor
{
public:
	explicit VideoBrightAdjust(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

	void setBright(float);


private:
	float m_bright = 0;
};

class VideoChromAdjust : public IVideoFrameProcessor
{
public:
	explicit VideoChromAdjust(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

	void setChrom(float);


private:
	float m_chrom = 0;
};

class VideoContrastAdjust : public IVideoFrameProcessor
{
public:
	explicit VideoContrastAdjust(QObject* p = nullptr);

	virtual FrameSPtr processFrame(FrameSPtr)Q_DECL_OVERRIDE;

	void setContrast(float);


private:
	float m_contrast = 0;
};

#endif // !VIDEOPROCESS_H




