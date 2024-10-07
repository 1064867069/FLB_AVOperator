#ifndef AVFILEWRITER_H
#define AVFILEWRITER_H

#include <QObject>
#include <QThread>

#include "interfaces.h"

enum class WriterState
{
	Paused,
	Working,
	Stop
};

class AudioResampleProcessor;
class AVFileWriter :public QObject
{
	Q_OBJECT
public:
	AVFileWriter(const ReaderSPtr& spReader, QObject* p = nullptr);

	~AVFileWriter();

	bool writeFile(const QString& spReader, const QString& sizeStr);

	WriterState getState()const;
public slots:
	void stop();

	void pause(bool);

signals:
	//void progressVal(int);

	void seek(double);

	void curSecChanged(double);

	void workBegin();

	void finished();
private:
	bool openOutputFile(const QString& sizeStr);

	void allocSwsCtx(int width, int height);

	void freeSwsCtx();

	void work();

	void workAudioLoop();

	void workVideoLoop();

	void workAudioVideoLoop();

	void reopenVCodecForVStream();

	void reset(const QString& newPath = "");
private:
	QThread m_threadRead;
	QThread m_threadWrite;
	ReaderSPtr m_spReader;

	QString m_pathOutput;
	QString m_pathOutputTemp;
	AVFormatContext* m_pFmtCtx = nullptr;
	AVStream* m_pVideoStream = nullptr;
	AVStream* m_pAudioStream = nullptr;
	AVCodecContext* m_pVideoCodecCtx = nullptr;
	AVCodecContext* m_pAudioCodecCtx = nullptr;

	struct SwsContext* m_pSwsCtx = nullptr;
	int m_width = 0;
	int m_height = 0;

	std::shared_ptr<AudioResampleProcessor> m_spResampleProc;
	QMutex m_mutex;
	QWaitCondition m_condWaitStopPause;
	std::atomic<WriterState> m_state = WriterState::Stop;
};

#endif // !AVFILEWRITER_H

