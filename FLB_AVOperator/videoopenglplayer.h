#ifndef VIDEOOPENGLPLAYER_H
#define VIDEOOPENGLPLAYER_H

#include "videoprocess.h"
#include "dlgvideoparam.h"

#include <QOpenGLWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QList>

#include <memory>
#include <future>


class FAVPlayer;
class FAVFileReader;
class FFrame;
class VideoFrameProcesser;
class PlayBtmBar;


using ReaderSPtr = std::shared_ptr<IAVReader>;
using FrameSPtr = std::shared_ptr<FFrame>;
using VFrameProcessorUPtr = std::unique_ptr<VideoFrameProcesser>;

class VideoFrameReadManager : public QObject
{
	Q_OBJECT
public:
	static std::shared_ptr<VideoFrameReadManager> createObj(ReaderSPtr, VideoProcessList*);

	~VideoFrameReadManager();

	FrameSPtr getFrame(double);

	bool isStopped()const;
public slots:
	void procVFrames();

	void recoverProc();

	void stop();

	void clear(bool lck = true);

	void seek(double sec);

	FrameSPtr updateLastFrame(const FrameSPtr & = nullptr);


private:
	VideoFrameReadManager(int fut_lim = 8);

	QThread m_threadProcFrame;
	mutable QMutex m_mutex;
	mutable QWaitCondition m_condStop;

	FrameSPtr m_spLastFrame;
	ReaderSPtr m_spReader;
	VideoProcessList* m_pProcessList = nullptr;

	QList<FrameSPtr> m_listOrgFrames;
	QList<FrameSPtr> m_listProcFrames;
	QList<std::shared_future<void>> m_listFuture;

	std::atomic<bool> m_bPaused = false;
	double m_secondGap = 0;
	double m_second = -1000;
	const int m_limFut;
	bool m_bStop = true;
};

class VideoOpenGLPlayer :public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit VideoOpenGLPlayer(QWidget* p = nullptr);
	virtual ~VideoOpenGLPlayer();

	void bindReader(ReaderSPtr);

	void setVideoWidth(int width, int height);

	void addVProcessor(const VProcessSPtr&);

	FAVPlayer* getPlayer();

	PlayBtmBar* getPlayBtmWidget();

	void removeBtmWidget();

	QRect getRectDisplay()const;
public slots:
	bool initProcessor();

	void playFrame();

	void repaintFrame();

	void pause(bool);

	void onAVStop();

	void onPauseSeek();

	void onSeekFinished();

	void refreshHide();

	//重置帧且暂停
	void onSeek();

	void hideBtmWidget();

	void onParamsUpdated();
protected slots:
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int window_W, int window_H) Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent* event)Q_DECL_OVERRIDE;
	bool eventFilter(QObject* watched, QEvent* event)Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent* event)Q_DECL_OVERRIDE;

	void resetBtmWidget();
	//void playProcessedFrame();
private:
	void resetGLVertex(int window_W, int window_H);

signals:
	void videoEnd();

	void needProc();

	void willDestroy();

	void rectChanged(const QRect&);

	void curPlayFrameChanged(FrameSPtr);
private:
	std::shared_ptr<VideoFrameReadManager> m_spFRdManager;
	VideoProcessList* m_pProcessList;
	//std::future<FrameSPtr> m_futureFrame;
	//int m_timeProcStart = -1;
	//int m_avrProcessTime = 0;//毫秒为单位

	ReaderSPtr m_spReader;
	FrameSPtr m_spCurFrame;
	/*FrameSPtr m_spNextFrame;
	FrameSPtr m_spCurFrameProcessed;*/

	FAVPlayer* m_pPlayer;
	PlayBtmBar* m_pPlayBtmWidget;
	DlgVideoParam* m_pDlgVParam;
	VFrameProcessorUPtr m_upVideoFrameProcessor;

	QTimer m_timerFramePlay;

	QTimer m_timerBtmHide;

	// 用于OpenGL绘制图像
	GLuint m_textureUniformY; // y纹理数据位置
	GLuint m_textureUniformU; // u纹理数据位置
	GLuint m_textureUniformV; // v纹理数据位置
	QOpenGLTexture* m_pTextureY;  // y纹理对象
	QOpenGLTexture* m_pTextureU;  // u纹理对象
	QOpenGLTexture* m_pTextureV;  // v纹理对象
	QOpenGLShader* pVShader;  // 顶点着色器程序对象
	QOpenGLShader* pFShader;  // 片段着色器对象
	QOpenGLShaderProgram* m_pShaderProgram; // 着色器程序容器
	GLfloat* m_vertexVertices; // 顶点矩阵

	QRect m_rectDisplay;
	float mPicIndexX; // 按比例显示情况下 图像偏移量百分比 (相对于窗口大小的)
	float mPicIndexY; //
	int m_width; // 视频宽度
	int m_height; // 视频高度

	bool mIsOpenGLInited = false; // OpenGL初始化函数是否执行过了
	bool gVideoKeepAspectRatio = true;
};

#endif
