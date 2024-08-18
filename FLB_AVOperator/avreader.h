#ifndef AVREADER_H
#define AVREADER_H

#include "interfaces.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

};

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

class IAVReader : public QObject
{
	Q_OBJECT
public:
	virtual void playerSetHandle(FAVPlayer*) { }

	virtual void videoWidgetSetHandle(VideoOpenGLPlayer*) { }

	virtual bool openPath(const QString&) = 0;

	virtual void readFrames() = 0;

	virtual void stop();

	FrameSPtr popAudioFrame();

	FrameSPtr popVideoFrame();

	const FAVInfo* getInfo()const;

	void reset()noexcept;

	bool decoding()const;

	bool seekAble()const;

	void addAudioProcessor(AProcessSPtr);

	void setListProcessor(const std::shared_ptr<AudioListProcessor>&);

protected:
	explicit IAVReader(QObject* p = nullptr);

	void waitNotBeyond();

	void decodePacket(AVCodecContext* dec_ctx, AVPacket* pkt, AVStream* strm);

	virtual bool chkFrameCondValid(const FFrame*) { return true; }

protected:

	std::shared_ptr<AudioListProcessor> m_spAudioProcessors;

	BufferUPtr m_upAudioBuffer = nullptr;
	BufferUPtr m_upVideoBuffer = nullptr;

	mutable QMutex m_mutex;
	QWaitCondition m_condStop;

	std::unique_ptr<FAVProcessors> m_upProcs;
	std::shared_ptr<FAVInfo> m_spInfo;

	bool m_bDecoding = false;
	bool m_bSeekAble = false;
	bool m_bStop = true;
};

class FAVFileReader : public IAVReader
{
	Q_OBJECT
public:
	explicit FAVFileReader(QObject* p = nullptr);

	virtual ~FAVFileReader();

	virtual void playerSetHandle(FAVPlayer*)override;

	virtual bool openPath(const QString&)override;

	virtual void readFrames()override;

	void seekSecond(double);

signals:
	void durationSecondChanged(double);

	void decodeEnd();

	void seekFinished();

private:
	double getPreciousDurationSecond();

	bool checkAndSeek();

	virtual bool chkFrameCondValid(const FFrame*)override;
private:

	int64_t m_aDuration = AV_NOPTS_VALUE;
	int64_t m_vDuration = AV_NOPTS_VALUE;

	double m_seekSecond = -1;
	double m_seekDecodeSecond = -1;


	//bool m_decoding = false;
};

//class FAVUrlReader : public IAVReader
//{
//	Q_OBJECT
//public:
//	explicit FAVUrlReader(QObject* p = nullptr);
//
//	//virtual ~FAVUrlReader();
//
//	virtual bool openPath(const QString&)override;
//
//	virtual void readFrames()override;
//
//};

#endif
