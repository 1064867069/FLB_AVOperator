#ifndef STREAMMANAGER_H
#define STREAMMANAGER_H

#include "interfaces.h"
#include <QSet>
#include <set>
#include <QMap>


class AudioStreamManager : public QObject
{
	Q_OBJECT
public:
	AudioStreamManager(const std::shared_ptr<FAVInfo>& spInfo);

	bool addStream(const StreamPtr& spStream);

	void removeStream(const StreamPtr& spStream);

	void seekSecond(double sec);

	void setNumSample(int nbSample);

	void setChannelLayout(AVChannelLayout chLayout);

	void setSampleRate(int sr);

	void setFormat(AVSampleFormat format);

	FrameSPtr readFrame();

	void cpyStreamFrom(const AudioStreamManager& other);

	double getCurSecond()const;

	bool hasStream()const;

	bool isEnd()const;

private slots:
	void onStreamEndChanged(double);

private:
	void searchEndStream();

	void resetFramePad();

	void timeMovByFrame(const FrameSPtr& spf);

	FrameSPtr getFrameByBuffer();
private:
	QMap<AudioStreamPtr, std::vector<std::vector<uint8_t>>> m_streams;
	std::vector<uint8_t> m_bufferSamples;

	FrameSPtr m_spFramePad;
	IAudioStream* m_pEndStream;
	std::shared_ptr<FAVInfo> m_spInfo;
	int64_t m_pts = 0;
	double m_second = 0;
};

class VideoOpenGLPlayer;
class VideoStreamManager : public QObject
{
	Q_OBJECT
public:
	VideoStreamManager(const std::shared_ptr<FAVInfo>& spInfo);

	void installVideoPlayer(VideoOpenGLPlayer* pPlayer);

	bool addStream(const StreamPtr& spStream);

	void removeStream(const StreamPtr& spStream);

	void seekSecond(double sec);

	FrameSPtr readFrame();

	void setFrameRate(int f_rate);

	double getCurSecond()const;

	VideoOpenGLPlayer* getVideoPlayer();

	int width()const;

	int height()const;

	bool hasStream()const;

	bool isEnd()const;

	void cpyStreamFrom(const VideoStreamManager& other);
public slots:
	//void onStreamWillSet();

	void onStreamWillSet(IVideoStream* pStream = nullptr);

	void onStreamWillSetDefault();
signals:
	void videoParamUpdated();

	void streamRemoved(VideoStreamPtr spStream);

	void streamEnterSetting(IVideoStream*);
	//void needRefresh();
private:
	void searchEndStream();

private slots:
	void unbindVPlayer();
private:
	VideoOpenGLPlayer* m_pVideoPlayer = nullptr;
	VideoStreamRectSetWidget* m_pWidgetSetting = nullptr;
	VideoStreamPtr m_spStreamSetting;
	QSet<VideoStreamPtr> m_streams;

	std::shared_ptr<FAVInfo> m_spInfo;
	IVideoStream* m_pEndStream;
	double m_second = 0;
	int64_t m_pts = 0;
	int m_levelCnt = 0;
};

enum class PlayState;
class StreamManager : public QObject
{
	Q_OBJECT
public:
	static std::shared_ptr<StreamManager> createManager();

	int sizePerSecond()const;

	bool addStream(const StreamPtr& spStream);

	void removeStream(const StreamPtr& spStream);

	bool seekSecond(double sec);

	FrameSPtr readAudioFrame();

	FrameSPtr readVideoFrame();

	FrameSPtr lastFrame();

	void start();

	void stop();

	void waitStream(unsigned long ms = 20);

	VideoStreamManager* getVideoManager();

	bool hasStream()const;

	std::shared_ptr<StreamManager> clone()const;

	std::shared_ptr<FAVInfo> getInfo()const;

	double getSecEnd()const;

	double getSecCurPlay()const;

	bool isEnd()const;

	bool isPaused()const;

public slots:
	void onEndChanged(double);

	void onPlayStateChanged(PlayState);

	void setCurPlaySecond(double sec);
signals:
	void endChanged(double);

	void abandoned();

	void hasStreamGotton();

	void streamReqRefresh();

	void stateChanged();
private:
	StreamManager();

	StreamManager(const StreamManager&) = delete;
	StreamManager& operator=(const StreamManager&) = delete;
private:
	std::shared_ptr<FAVInfo> m_spInfo;
	AudioStreamManager m_audioManager;
	VideoStreamManager m_videoManager;

	mutable QMutex m_mutex;
	mutable QWaitCondition m_condHasStream;

	FrameSPtr m_spLastFrame;

	double m_secCurPlay = 0;
	double m_secCurDec = 0;
	double m_secEnd = 0;

	bool m_bPause = true;
};

class ResourceFactoryFileAudio : public IAVResourceFactory
{
public:
	virtual QString fileFilter()override;

	virtual QString title()override;

	virtual QList<StreamPtr> createStreams(const QString& desc)override;
};

class ResourceFactoryFileVideo : public IAVResourceFactory
{
public:
	virtual QString fileFilter()override;

	virtual QString title()override;

	virtual QList<StreamPtr> createStreams(const QString& desc)override;
};

#endif // !STREAMMANAGER_H

