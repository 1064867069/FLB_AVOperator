#ifndef VIDEOOPENGLPLAYER_H
#define VIDEOOPENGLPLAYER_H

#include <QOpenGLWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <memory>

class FAVPlayer;
class FAVFileReader;
class FFrame;
class VideoFrameProcesser;


using ReaderSPtr = std::shared_ptr<FAVFileReader>;
using FrameSPtr = std::shared_ptr<FFrame>;
using VProcessorUPtr = std::unique_ptr<VideoFrameProcesser>;

class VideoOpenGLPlayer :public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit VideoOpenGLPlayer(QWidget* p = nullptr);
	virtual ~VideoOpenGLPlayer();

	void bindReader(ReaderSPtr);
	void setVideoWidth(int width, int height);

	FAVPlayer* getPlayer();

public slots:
	bool initProcessor();
	void playFrame();
	void pause(bool);


protected slots:
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int window_W, int window_H) Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	void resetGLVertex(int window_W, int window_H);

private:
	ReaderSPtr m_spReader;
	FrameSPtr m_spCurFrame;
	FrameSPtr m_spNextFrame;

	FAVPlayer* m_pPlayer;
	VProcessorUPtr m_upVideoProcessor;
	QTimer m_timer;


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

	float mPicIndexX; // 按比例显示情况下 图像偏移量百分比 (相对于窗口大小的)
	float mPicIndexY; //
	int m_width; // 视频宽度
	int m_height; // 视频高度

	bool mIsOpenGLInited = false; // OpenGL初始化函数是否执行过了
	bool gVideoKeepAspectRatio = true;
};

#endif
