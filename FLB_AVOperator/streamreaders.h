#ifndef STREAMREADERS_H
#define STREAMREADERS_H

#include "avreader.h"
#include "avutils.h"

#include <future>

class FAVFileBaseStream :public IFAVStream
{
public:
	FAVFileBaseStream(const QString& filePath, AVMediaType type);

	virtual ~FAVFileBaseStream();

	virtual FrameSPtr getNxtFrames()override;

	virtual StreamPtr clone()const override;

	virtual void setStart(double s)override;

	virtual void setEnd(double e)override;

	virtual void seekSecond(double sec)override;

	virtual double startMin()const override;

	virtual double endMax()const override;

	double secLastDec()const;

	const AVStream* avStream()const;

	const AVCodecContext* avDecCtx()const;

	const QString& getFilePath()const;

	void cpyBFSTime(const FAVFileBaseStream& other);
protected:

	void decodePacket();

private:
	void openFile(const QString& path, AVMediaType type);

	void getEndTime();

	FrameSPtr readOneFrame();
private:
	BufferUPtr m_upFrameBuffer;
	FrameSPtr m_spCurFrame;

	QString m_filePath;
	AVFormatContext* m_pInFmtCtx = nullptr;
	AVCodecContext* m_pDecCtx = nullptr; // 解码器的实例
	AVStream* m_pSrcStream = nullptr; // 源文件的音视频流
	AVPacket* m_pPacket;

	std::future<FrameSPtr> m_futureNxtFrame;
	QMutex m_mutexDecode;

	double m_decLastSec = 0;
	double m_decStart = 0;
	double m_decEnd = 0;
	double m_curTime = 0;
	double m_decWholeStart = 0;
	double m_decWholeEnd = 0;

	int m_streamIndex = -1;
	bool m_bIsInited = false;
};

class AudioFileStream :public IAudioStream
{
public:
	explicit AudioFileStream(const QString& filePath);

	virtual FrameSPtr getNxtFrames()override;

	virtual StreamPtr clone()const override;

	virtual void setStart(double s)override;

	virtual void setEnd(double e)override;

	virtual  void moveHorizon(double gap)override;

	virtual void seekSecond(double sec)override;

	virtual double startMin()const override;

	virtual double endMax()const override;

	virtual QBrush backgroundBrush()const override;

	virtual void bindStreamManager(const std::shared_ptr<StreamManager>& spManager)override;
private:
	FAVFileBaseStream m_fileStream;
};

class VideoFileStream :public IVideoStream
{
public:
	explicit VideoFileStream(const QString& filePath);

	virtual ~VideoFileStream();

	virtual FrameSPtr getNxtFrames()override;

	virtual StreamPtr clone()const override;

	virtual void setStart(double s)override;

	virtual void setEnd(double e)override;

	virtual  void moveHorizon(double gap)override;

	virtual void seekSecond(double sec)override;

	virtual double startMin()const override;

	virtual double endMax()const override;

	virtual QBrush backgroundBrush()const override;

	virtual void bindStreamManager(const std::shared_ptr<StreamManager>& spManager)override;

	virtual void onOutParamUpdated()override;

	virtual	FrameSPtr swsObjFrame(const FrameSPtr& spf)override;
private:
	FAVFileBaseStream m_fileStream;

	FrameSPtr m_spCurFrame;
	FrameSPtr m_spNxtFrame;

	QMutex m_mutexSws;
	struct SwsContext* m_pSwsCtx = nullptr;
	AVPixelFormat m_srcPixFormat = AV_PIX_FMT_YUV420P;
	double m_second = 0.0;
	int m_srcWidth = 0;
	int m_srcHeight = 0;
};


//class VideoImageStream :public IVideoStream
//{
//	Q_OBJECT
//public:
//	explicit VideoImageStream(const QString& path);
//
//private:
//	QString m_pathImage;
//
//	QMutex m_mutexSws;
//	struct SwsContext* m_pSwsCtx = nullptr;
//};

#endif // !STREAMREADERS_H

