#include "videoopenglplayer.h"
#include "avoperator.h"
#include "playbtmbar.h"

#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4




VideoOpenGLPlayer::VideoOpenGLPlayer(QWidget* p) :
	QOpenGLWidget(p),
	m_pProcessList(new VideoProcessList(this)),
	m_pPlayer(new FAVPlayer(this)),
	m_pPlayBtmWidget(new PlayBtmBar(m_pPlayer, this, this)),
	m_pDlgVParam(new DlgVideoParam(m_pProcessList, this))
{
	m_textureUniformY = 0;
	m_textureUniformU = 0;
	m_textureUniformV = 0;
	m_height = 0;
	m_width = 0;
	mPicIndexX = 0;
	mPicIndexY = 0;

	pVShader = nullptr;
	pFShader = nullptr;
	m_pShaderProgram = nullptr;
	m_pTextureY = nullptr;
	m_pTextureU = nullptr;
	m_pTextureV = nullptr;

	m_vertexVertices = new GLfloat[8];

	setAcceptDrops(true);

	//m_timerBtmHide.setInterval(1000);
	m_pPlayBtmWidget->hide();
	m_pPlayBtmWidget->installEventFilter(this);

	setMouseTracking(true);
	grabKeyboard();

	connect(&m_timerBtmHide, &QTimer::timeout, this, &VideoOpenGLPlayer::hideBtmWidget);
	connect(&m_timerFramePlay, &QTimer::timeout, this, &VideoOpenGLPlayer::playFrame);
	connect(m_pDlgVParam, &DlgVideoParam::paramUpdated, this, &VideoOpenGLPlayer::onParamsUpdated);
	connect(m_pPlayBtmWidget, &PlayBtmBar::setClicked, m_pDlgVParam, &DlgVideoParam::show);

	//connect(this, &VideoOpenGLPlayer::frameProcessed, this, &VideoOpenGLPlayer::playProcessedFrame);
}

VideoOpenGLPlayer::~VideoOpenGLPlayer()
{
	qDebug() << "视频播放窗口析构！";
}

FAVPlayer* VideoOpenGLPlayer::getPlayer()
{
	return m_pPlayer;
}

void VideoOpenGLPlayer::bindReader(ReaderSPtr spr)
{
	if (spr)
	{
		this->pause(true);

		//auto info = spr->getInfo();
		/*if (info->m_vIndx < 0 || info->m_avgfRate <= 0)
		{
			return;
		}*/
		//m_timer.setInterval(1000 / info->m_avgfRate);
		qDebug() << (bool)m_spReader;
		m_spReader.swap(spr);

		m_spFRdManager = VideoFrameReadManager::createObj(m_spReader, m_pProcessList);
		if (m_spFRdManager)
		{
			connect(this, &VideoOpenGLPlayer::needProc, m_spFRdManager.get(), &VideoFrameReadManager::procVFrames);

		}
	}
}

void VideoOpenGLPlayer::refreshHide()
{
	if (!m_timerBtmHide.isActive())
	{
		this->resetBtmWidget();

	}
	m_timerBtmHide.start(3000);
}

void VideoOpenGLPlayer::pause(bool isPause)
{
	if (!m_spFRdManager)
	{
		qDebug() << __FUNCTION__ << ": pause - No reader!";
		return;
	}

	if (isPause)
	{
		if (m_timerFramePlay.isActive())
		{
			m_timerFramePlay.stop();

		}
	}
	else
	{
		if (m_spFRdManager->isStopped())
			emit needProc();

		if (!m_timerFramePlay.isActive() && m_spReader->getInfo()->m_vIndx >= 0)
		{
			m_timerFramePlay.start();
		}
	}

	m_pPlayBtmWidget->setPlayState(!isPause);
}

void VideoOpenGLPlayer::onAVStop()
{
	this->pause(true);
	/*if (m_futureFrame.valid())
		m_futureFrame.get();*/

	if (m_spFRdManager)
		m_spFRdManager->stop();

	m_upVideoFrameProcessor.reset();
	m_spCurFrame.reset();

	this->update();
}

void VideoOpenGLPlayer::onPauseSeek()
{
	m_spCurFrame.reset();

	//若干毫秒还获取不到当前帧就不用了
	for (int i = 0; i < 10; ++i)
	{
		if (m_pPlayer->state() != PlayState::Pause)
			return;

		m_spCurFrame = m_spReader->popVideoFrame();
		if (m_spCurFrame)
			break;

		QThread::msleep(5);
	}

	if (!m_spCurFrame)
		return;

	if (m_spFRdManager)
		m_spCurFrame = m_spFRdManager->updateLastFrame(m_spCurFrame);
	else
		m_spCurFrame = m_pProcessList->processFrame(m_spCurFrame);
	m_upVideoFrameProcessor->processFrame(m_spCurFrame);
	this->update();
}

void VideoOpenGLPlayer::onSeek()
{
	if (!m_spReader || m_spReader->getInfo()->m_vIndx < 0)
		return;

	m_spCurFrame.reset();
	this->pause(true);
	m_spFRdManager->clear();
}

void VideoOpenGLPlayer::initializeGL()
{
	mIsOpenGLInited = true;

	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	// 着色器：就是使用OpenGL着色语言(OpenGL Shading Language, GLSL)编写的一个小函数,
	// GLSL是构成所有OpenGL着色器的语言，具体的GLSL语言的语法需要读者查找相关资料
	// 初始化顶点着色器对象
	pVShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	// 顶点着色器源码
	const char* vsrc = "attribute vec4 vertexIn; \
        attribute vec2 textureIn; \
        varying vec2 textureOut;  \
        void main(void)           \
    {                         \
            gl_Position = vertexIn; \
            textureOut = textureIn; \
    }";

	// 编译顶点着色器程序
	bool bCompile = pVShader->compileSourceCode(vsrc);
	if (!bCompile)
	{
		qDebug() << "vertex compile failed.";
	}
	// 初始化片段着色器。功能gpu中yuv转换成rgb
	pFShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	// 片段着色器源码(windows下opengl es 需要加上float这句话)
	const char* fsrc =
#if defined(WIN32)
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
#endif
		"varying vec2 textureOut; \
        uniform sampler2D tex_y; \
        uniform sampler2D tex_u; \
        uniform sampler2D tex_v; \
        void main(void) \
    { \
            vec3 yuv; \
            vec3 rgb; \
            yuv.x = texture2D(tex_y, textureOut).r - 16.0/255.0; \
            yuv.y = texture2D(tex_u, textureOut).g - 0.5; \
            yuv.z = texture2D(tex_v, textureOut).b - 0.5; \
            rgb = mat3( 1.164,       1.164,         1.164, \
                   0,       -0.392,  2.017, \
                   1.596,   -0.813,  0) * yuv; \
            gl_FragColor = vec4(rgb, 1); \
    }";
	// 以上的mat3转换矩阵采用BT601标准版

	// 将glsl源码送入编译器编译着色器程序
	bCompile = pFShader->compileSourceCode(fsrc);
	if (!bCompile)
	{
		qDebug() << "fragment compile failed.";

	}

	// 创建着色器程序容器
	m_pShaderProgram = new QOpenGLShaderProgram;
	// 将片段着色器添加到程序容器
	m_pShaderProgram->addShader(pFShader);
	// 将顶点着色器添加到程序容器
	m_pShaderProgram->addShader(pVShader);
	// 绑定属性vertexIn到指定位置ATTRIB_VERTEX，该属性在顶点着色源码其中有声明
	m_pShaderProgram->bindAttributeLocation("vertexIn", ATTRIB_VERTEX);
	// 绑定属性textureIn到指定位置ATTRIB_TEXTURE，该属性在顶点着色源码其中有声明
	m_pShaderProgram->bindAttributeLocation("textureIn", ATTRIB_TEXTURE);
	// 链接所有所有添入到的着色器程序
	m_pShaderProgram->link();
	// 激活所有链接
	m_pShaderProgram->bind();
	// 读取着色器中的数据变量tex_y、tex_u、tex_v的位置，这些变量在片段着色器源码中声明
	m_textureUniformY = m_pShaderProgram->uniformLocation("tex_y");
	m_textureUniformU = m_pShaderProgram->uniformLocation("tex_u");
	m_textureUniformV = m_pShaderProgram->uniformLocation("tex_v");

	// 顶点矩阵
	const GLfloat vertexVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
	};
	memcpy(m_vertexVertices, vertexVertices, sizeof(vertexVertices));

	// 纹理矩阵
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	};
	// 设置属性ATTRIB_VERTEX的顶点矩阵值以及格式
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, m_vertexVertices);
	// 设置属性ATTRIB_TEXTURE的纹理矩阵值以及格式
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
	// 启用ATTRIB_VERTEX属性的数据，默认是关闭的
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	// 启用ATTRIB_TEXTURE属性的数据，默认是关闭的
	glEnableVertexAttribArray(ATTRIB_TEXTURE);
	// 分别创建y、u、v纹理对象
	m_pTextureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
	m_pTextureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
	m_pTextureV = new QOpenGLTexture(QOpenGLTexture::Target2D);
	m_pTextureY->create();
	m_pTextureU->create();
	m_pTextureV->create();
	glClearColor(0.0, 0.0, 0.0, 0.0); // 将背景色设为黑色
}

void VideoOpenGLPlayer::resizeGL(int window_W, int window_H)
{
	//mLastGetFrameTime = QDateTime::currentMSecsSinceEpoch();
	if (window_H == 0) // 防止被零除
	{
		window_H = 1; // 将高设为1
	}
	// 设置视口
	glViewport(0, 0, window_W, window_H);

	//int x = window_W - ui->pushButton_close->width() - 22;
	int x = window_W - 22;
	int y = 22;
	//ui->pushButton_close->move(x, y);

	x = 0;
	/* y = window_H / 2 - ui->widget_erro->height() / 2;
	 ui->widget_erro->move(x, y);
	 ui->widget_erro->resize(window_W, ui->widget_erro->height());*/

	x = 0;
	/*y = window_H - ui->widget_name->height() - 6;
	ui->widget_name->move(x, y);
	ui->widget_name->resize(window_W, ui->widget_name->height());*/

	resetGLVertex(window_W, window_H);
}

void VideoOpenGLPlayer::resetGLVertex(int window_W, int window_H)
{
	if (m_width <= 0 || m_height <= 0 || !gVideoKeepAspectRatio) // 铺满
	{
		mPicIndexX = 0.0;
		mPicIndexY = 0.0;

		// 顶点矩阵
		const GLfloat vertexVertices[] = {
			-1.0f, -1.0f,
			1.0f, -1.0f,
			-1.0f, 1.0f,
			1.0f, 1.0f,
		};
		memcpy(m_vertexVertices, vertexVertices, sizeof(vertexVertices));

		// 纹理矩阵
		static const GLfloat textureVertices[] = {
			0.0f,  1.0f,
			1.0f,  1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f,
		};
		// 设置属性ATTRIB_VERTEX的顶点矩阵值以及格式
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, m_vertexVertices);
		// 设置属性ATTRIB_TEXTURE的纹理矩阵值以及格式
		glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
		// 启用ATTRIB_VERTEX属性的数据,默认是关闭的
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		// 启用ATTRIB_TEXTURE属性的数据,默认是关闭的
		glEnableVertexAttribArray(ATTRIB_TEXTURE);
	}
	else // 按比例
	{
		int pix_W = window_W;
		int pix_H = m_height * pix_W / m_width;

		int x = this->width() - pix_W;
		int y = this->height() - pix_H;
		x /= 2;
		y /= 2;

		if (y < 0)
		{
			pix_H = window_H;
			pix_W = m_width * pix_H / m_height;

			x = this->width() - pix_W;
			y = this->height() - pix_H;
			x /= 2;
			y /= 2;
		}

		mPicIndexX = x * 1.0 / window_W;
		mPicIndexY = y * 1.0 / window_H;

		//qDebug()<<window_W<<window_H<<pix_W<<pix_H<<x<<y;
		float index_y = y * 1.0 / window_H * 2.0 - 1.0;
		float index_y_1 = index_y * -1.0;
		float index_y_2 = index_y;

		float index_x = x * 1.0 / window_W * 2.0 - 1.0;
		float index_x_1 = index_x * -1.0;
		float index_x_2 = index_x;

		const GLfloat vertexVertices[] = {
			index_x_2, index_y_2,
			index_x_1,  index_y_2,
			index_x_2, index_y_1,
			index_x_1,  index_y_1,
		};
		memcpy(m_vertexVertices, vertexVertices, sizeof(vertexVertices));

		static const GLfloat textureVertices[] = {
			0.0f,  1.0f,
			1.0f,  1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f,
		};
		// 设置属性ATTRIB_VERTEX的顶点矩阵值以及格式
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, m_vertexVertices);
		// 设置属性ATTRIB_TEXTURE的纹理矩阵值以及格式
		glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
		// 启用ATTRIB_VERTEX属性的数据，默认是关闭的
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		// 启用ATTRIB_TEXTURE属性的数据，默认是关闭的
		glEnableVertexAttribArray(ATTRIB_TEXTURE);
	}
}

bool VideoOpenGLPlayer::initProcessor()
{
	if (m_upVideoFrameProcessor)
		m_upVideoFrameProcessor.reset();

	if (!m_spReader)
		return false;

	auto info = m_spReader->getInfo();
	if (info->m_vIndx < 0 || info->m_avgfRate <= 0)
	{
		return false;
	}
	m_timerFramePlay.setInterval(std::max(1000 / info->m_avgfRate * 0.6, 1.0));

	m_upVideoFrameProcessor = std::make_unique<VideoFrameProcesser>(m_spReader->getInfo());
	return m_upVideoFrameProcessor->valid();
}

void VideoOpenGLPlayer::playFrame()
{
	if (!m_spFRdManager)
		return;

	double preSecond = -1;
	if (m_spCurFrame)
		preSecond = m_spCurFrame->getSecond();

	auto secnd = m_pPlayer->getCurSecond();
	auto frame = m_spFRdManager->getFrame(secnd);
	if (frame)
		m_spCurFrame = frame;

	if (!frame || m_spCurFrame->getSecond() == preSecond)
	{
		if (m_spCurFrame && !frame && !m_spReader->decoding())
		{
			m_upVideoFrameProcessor->reset();
			this->update();
			//this->pause(true);
			emit videoEnd();
		}

		return;
	}

	m_upVideoFrameProcessor->processFrame(m_spCurFrame);
	int w = m_upVideoFrameProcessor->objWidth(), h = m_upVideoFrameProcessor->objHeight();
	if (m_width <= 0 || m_height <= 0 || m_width != w || m_height != h)
	{
		this->setVideoWidth(w, h);
	}

	this->update();
}

void VideoOpenGLPlayer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_upVideoFrameProcessor)
	{
		auto bufferYUV = m_upVideoFrameProcessor->getYuvBuffer();
		if (bufferYUV != nullptr)
		{
			m_pShaderProgram->bind();
			// 0对应纹理单元GL_TEXTURE0，1对应纹理单元GL_TEXTURE1，2对应纹理单元GL_TEXTURE2
			glUniform1i(m_textureUniformY, 0); // 指定y纹理要使用新值
			glUniform1i(m_textureUniformU, 1); // 指定u纹理要使用新值
			glUniform1i(m_textureUniformV, 2); // 指定v纹理要使用新值

			glActiveTexture(GL_TEXTURE0); // 激活纹理单元GL_TEXTURE0
				// 使用来自Y分量的数据生成纹理
			glBindTexture(GL_TEXTURE_2D, m_pTextureY->textureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_width, m_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bufferYUV);

			glActiveTexture(GL_TEXTURE1); // 激活纹理单元GL_TEXTURE1
			// 使用来自U分量的数据生成纹理
			glBindTexture(GL_TEXTURE_2D, m_pTextureU->textureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_width / 2, m_height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bufferYUV + m_width * m_height);

			glActiveTexture(GL_TEXTURE2); // 激活纹理单元GL_TEXTURE2
			// 使用来自V分量的数据生成纹理
			glBindTexture(GL_TEXTURE_2D, m_pTextureV->textureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_width / 2, m_height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bufferYUV + m_width * m_height * 5 / 4);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // 使用顶点数组方式绘制图形

			m_pShaderProgram->release(); // 释放小程序
		}
		else
		{
			glClearColor(0.0, 0.0, 0.0, 1.0);
		}
	}
	else
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}
}

void VideoOpenGLPlayer::mouseMoveEvent(QMouseEvent* event)
{
	QOpenGLWidget::mouseMoveEvent(event);
	if (m_pPlayer->state() != PlayState::Stop)
		this->refreshHide();
}

void VideoOpenGLPlayer::keyPressEvent(QKeyEvent* event)
{

	if (event->key() == Qt::Key_Escape)
	{
		if (this->isFullScreen())
			m_pPlayBtmWidget->onFullScreen();
	}
	else if (event->key() == Qt::Key_Space)
	{
		m_pPlayBtmWidget->onPlayPauseClicked();
	}
	else if (event->key() == Qt::Key_Left)
	{
		m_pPlayer->seekBackward();
	}
	else if (event->key() == Qt::Key_Right)
	{
		m_pPlayer->seekForward();
	}

	QOpenGLWidget::keyPressEvent(event);
}

bool VideoOpenGLPlayer::eventFilter(QObject* watched, QEvent* event)
{
	PlayBtmBar* btm = dynamic_cast<PlayBtmBar*>(watched);
	if (btm != nullptr && btm == m_pPlayBtmWidget)
	{
		//qDebug() << event->type();
		m_pPlayBtmWidget->onEventType(event->type());
	}

	return QOpenGLWidget::eventFilter(watched, event);
}

void VideoOpenGLPlayer::resizeEvent(QResizeEvent* event)
{
	QOpenGLWidget::resizeEvent(event);
	if (!m_pPlayBtmWidget->isHidden())
		this->resetBtmWidget();
}

void VideoOpenGLPlayer::hideBtmWidget()
{
	if ((!m_pPlayBtmWidget->isSliderPressed() && !m_pPlayBtmWidget->isMouseEntered()))
	{
		m_pPlayBtmWidget->hide();
		m_timerBtmHide.stop();
	}
}

void VideoOpenGLPlayer::onParamsUpdated()
{
	if (m_spCurFrame)
	{
		m_spCurFrame = m_spFRdManager->updateLastFrame();
		m_upVideoFrameProcessor->processFrame(m_spCurFrame);

		this->update();
	}
}

PlayBtmBar* VideoOpenGLPlayer::getPlayBtmWidget()
{
	return m_pPlayBtmWidget;
}

void VideoOpenGLPlayer::resetBtmWidget()
{
	int w = this->width(), h = this->height();
	int btmHeight = h / 7;
	m_pPlayBtmWidget->resize(w, btmHeight);

	int y = h - m_pPlayBtmWidget->height();
	m_pPlayBtmWidget->move(0, y);

	m_pPlayBtmWidget->show();
}

//void VideoOpenGLPlayer::playProcessedFrame()
//{
//	if (!m_futureFrame.valid())
//		return;
//
//	FrameSPtr curf = m_futureFrame.get();
//	if (!curf)
//		return;
//
//
//
//	m_spCurFrameProcessed = curf;
//	m_upVideoFrameProcessor->processFrame(m_spCurFrameProcessed);
//
//	int w = m_upVideoFrameProcessor->objWidth(), h = m_upVideoFrameProcessor->objHeight();
//	if (m_width <= 0 || m_height <= 0 || m_width != w || m_height != h)
//	{
//		this->setVideoWidth(w, h);
//	}
//
//	this->update();
//}

void VideoOpenGLPlayer::setVideoWidth(int width, int height)
{
	if (width <= 0 || height <= 0) return;
	m_width = width;
	m_height = height;
	qDebug() << __FUNCTION__ << width << height << this->isHidden();

	if (mIsOpenGLInited)
	{
		resetGLVertex(this->width(), this->height());
	}
}

void VideoOpenGLPlayer::addVProcessor(const VProcessSPtr& spProc)
{
	m_pProcessList->addVProcessor(spProc);
}


VideoFrameReadManager::VideoFrameReadManager() : QObject(nullptr)
{
	this->moveToThread(&m_threadProcFrame);
	m_threadProcFrame.start();
}

VideoFrameReadManager::~VideoFrameReadManager()
{
	this->stop();
	m_threadProcFrame.quit();
	m_threadProcFrame.wait();
}

std::shared_ptr<VideoFrameReadManager> VideoFrameReadManager::createObj(ReaderSPtr spr, VideoProcessList* pPl)
{
	if (!spr)
		return nullptr;

	auto frm = new VideoFrameReadManager();
	auto res = std::shared_ptr<VideoFrameReadManager>(frm);
	if (frm)
	{
		res->m_spReader = spr;
		res->m_pProcessList = pPl;
	}

	//connect(res.get(), &VideoFrameReadManager::needThStart, &res->m_threadProcFrame, &QThread::start);
	return res;
}

bool VideoFrameReadManager::isStopped()const
{
	QMutexLocker locker(&m_mutex);
	return m_stop;
}

FrameSPtr VideoFrameReadManager::getFrame(double s)
{
	QMutexLocker locker(&m_mutex);
	FrameSPtr res;
	/*if (m_listOrgFrames.size() > 0)
		qDebug() << m_listOrgFrames[0]->getSecond() << s;*/
	while (m_listOrgFrames.size() > 0 && m_listOrgFrames[0]->getSecond() <= s)
	{
		m_spLastFrame = m_listOrgFrames.front();
		m_listOrgFrames.pop_front();

		res = m_listProcFrames.front();
		m_listProcFrames.pop_front();
	}

	return res;
}

void VideoFrameReadManager::procVFrames()
{
	if (!m_spReader || !m_pProcessList)
		return;

	m_stop = false;

	FrameSPtr tmp;
	while (!m_stop)
	{
		if (m_listOrgFrames.size() > 5)
		{
			QThread::msleep(5);
			continue;
		}

		tmp = m_spReader->popVideoFrame();
		if (tmp)
		{
			auto procf = m_pProcessList ? m_pProcessList->processFrame(tmp) : tmp;
			if (procf)
			{
				QMutexLocker locker(&m_mutex);
				m_listOrgFrames.append(tmp);
				m_listProcFrames.append(procf);

				while (m_listOrgFrames.size() > 1 && m_listOrgFrames[0]->getSecond() > tmp->getSecond())
				{
					m_listOrgFrames.pop_front();
					m_listProcFrames.pop_front();
				}
			}
		}
		else
		{
			QThread::msleep(5);
		}
	}

	QMutexLocker locker(&m_mutex);
	m_stop = true;
	this->clear(false);
	m_condStop.wakeAll();
}

void VideoFrameReadManager::stop()
{
	QMutexLocker locker(&m_mutex);
	if (!m_stop)
	{
		m_stop = true;
		m_condStop.wait(&m_mutex);
	}
}

void VideoFrameReadManager::clear(bool lck)
{
	auto clrNoLock = [this]()->void
	{
		m_spLastFrame.reset();
		m_listOrgFrames.clear();
		m_listProcFrames.clear();
	};

	if (lck)
	{
		QMutexLocker locker(&m_mutex);
		clrNoLock();
	}
	else
	{
		clrNoLock();
	}

}

FrameSPtr VideoFrameReadManager::updateLastFrame(const FrameSPtr& spf)
{
	QMutexLocker locker(&m_mutex);
	if (spf)
		m_spLastFrame = spf;

	if (m_spLastFrame && m_pProcessList)
		return m_pProcessList->processFrame(m_spLastFrame);
	return nullptr;
}
