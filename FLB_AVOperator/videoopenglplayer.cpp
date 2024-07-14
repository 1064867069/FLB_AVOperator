#include "videoopenglplayer.h"
#include "avoperator.h"
#include <QDebug>
#include <QDateTime>

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

VideoOpenGLPlayer::VideoOpenGLPlayer(QWidget* p) :QOpenGLWidget(p), m_pPlayer(new FAVPlayer(this))
{
	m_textureUniformY = 0;
	m_textureUniformU = 0;
	m_textureUniformV = 0;
	m_height = 0;
	m_width = 0;
	mPicIndexX = 0;
	mPicIndexY = 0;

	pVShader = NULL;
	pFShader = NULL;
	m_pShaderProgram = NULL;
	m_pTextureY = NULL;
	m_pTextureU = NULL;
	m_pTextureV = NULL;

	m_vertexVertices = new GLfloat[8];

	setAcceptDrops(true);

	connect(&m_timer, &QTimer::timeout, this, &VideoOpenGLPlayer::playFrame);

	/*mIsPlaying = false;
	mPlayFailed = false;
	mCurrentVideoKeepAspectRatio = gVideoKeepAspectRatio;
	mIsCloseAble = true;
	mIsOpenGLInited = false;
	mLastGetFrameTime = 0;*/

	//this->initializeGL();
}

VideoOpenGLPlayer::~VideoOpenGLPlayer()
{

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
	}
}

void VideoOpenGLPlayer::pause(bool isPause)
{
	if (!m_spReader)
	{
		qDebug() << __FUNCTION__ << ": pause - No reader!";
		return;
	}

	if (isPause)
	{
		if (m_timer.isActive())
		{
			m_timer.stop();
		}
	}
	else
	{
		if (!m_timer.isActive())
		{
			m_timer.start();
		}
	}
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
	if (!m_spReader)
		return false;

	auto info = m_spReader->getInfo();
	if (info->m_vIndx < 0 || info->m_avgfRate <= 0)
	{
		return false;
	}
	m_timer.setInterval(std::max(1000 / info->m_avgfRate * 0.6, 1.0));

	m_upVideoProcessor = std::make_unique<VideoFrameProcesser>(m_spReader->getInfo());
	return m_upVideoProcessor->valid();
}

void VideoOpenGLPlayer::playFrame()
{
	double preSecond = -1;
	if (m_spCurFrame)
		preSecond = m_spCurFrame->getSecond();

	if (!m_spCurFrame)
	{
		m_spCurFrame = m_spReader->popVideoFrame();

	}
	if (!m_spNextFrame)
	{
		m_spNextFrame = m_spReader->popVideoFrame();
	}

	auto secnd = m_pPlayer->getCurSecond();
	auto scd = m_spNextFrame->getSecond();
	while (m_spNextFrame && m_spNextFrame->getSecond() < secnd)
	{
		m_spCurFrame = m_spNextFrame;
		m_spNextFrame = m_spReader->popVideoFrame();
		/*qDebug() << "获得帧时间：" << m_spNextFrame->getSecond();
		qDebug() << "pts = " << m_spNextFrame->getPts();*/
	}


	if (!m_spCurFrame || m_spCurFrame->getSecond() == preSecond)
		return;

	m_upVideoProcessor->processFrame(m_spCurFrame);

	int w = m_upVideoProcessor->objWidth(), h = m_upVideoProcessor->objHeight();
	if (m_width <= 0 || m_height <= 0 || m_width != w || m_height != h)
	{
		this->setVideoWidth(w, h);
	}


	this->update();
}

void VideoOpenGLPlayer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_upVideoProcessor)
	{
		auto bufferYUV = m_upVideoProcessor->getYuvBuffer();
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
	}
	else
	{
		glClearColor(1.0, 0.0, 0.0, 1.0);
	}
}

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

