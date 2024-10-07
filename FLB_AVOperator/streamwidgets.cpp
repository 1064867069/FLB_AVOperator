#include "streamwidgets.h"
#include "streamutils.h"
#include "avoperator.h"
#include "streamtimewidget.h"
#include "videoopenglplayer.h"

#include <QDebug>
#include <QPainter>

AVStreamBlock::AVStreamBlock(const StreamPtr& spStream, std::shared_ptr<StreamManager> spManager, QWidget* p)
	:QWidget(p), m_spStream(spStream), m_spStreamManager(spManager)
{
	if (m_spStream && m_spStream->valid())
	{
		connect(m_spStream.get(), &IFAVStream::startChanged, this, &AVStreamBlock::onStartChanged);
		connect(m_spStream.get(), &IFAVStream::endChanged, this, &AVStreamBlock::onEndChanged);
		m_spStream->connectBlock(this);

		this->setWindowFlags(Qt::FramelessWindowHint);

		this->onStartChanged(m_spStream->timeStart());
		this->onEndChanged(m_spStream->timeEnd());
		this->setAttribute(Qt::WA_Hover, true);
	}
}

AVStreamBlock::~AVStreamBlock()
{
	this->removeFromLink();
}

StreamPtr AVStreamBlock::getStreamPtr()const
{
	return m_spStream;
}

void AVStreamBlock::onStartXChanged()
{
	m_spStream->setStart(this->x() / static_cast<double>(m_spStreamManager->sizePerSecond()));
}

void AVStreamBlock::onEndXChanged()
{
	m_spStream->setEnd(static_cast<double>(this->x() + this->width()) / m_spStreamManager->sizePerSecond());
}

void AVStreamBlock::secMoveRight(double gap)
{
	if (gap > 0 && m_pBlockBack != nullptr)
	{
		gap = std::min((double)(m_pBlockBack->x() - this->x() - this->width()) / m_spStreamManager->sizePerSecond(), gap);
	}
	else if (gap < 0 && m_pBlockPre != nullptr)
	{
		gap = std::max((double)(this->x() - m_pBlockPre->x() - m_pBlockPre->width()) / m_spStreamManager->sizePerSecond(),
			gap);
	}

	if (std::abs(gap) < 1e-4)
		return;
	m_spStream->moveHorizon(gap);
	this->resetGeometry();
}

void AVStreamBlock::secStartMoveRight(double gap)
{
	if (gap < 0 && m_pBlockPre != nullptr)
	{
		gap = std::max(gap, m_pBlockPre->getStreamPtr()->timeEnd() - m_spStream->timeStart());
	}

	m_spStream->setStart(m_spStream->timeStart() + gap);
}

void AVStreamBlock::secEndMoveRight(double gap)
{
	if (gap > 0 && m_pBlockBack != nullptr)
	{
		gap = std::min(gap, m_pBlockBack->getStreamPtr()->timeStart() - m_spStream->timeEnd());
	}

	m_spStream->setEnd(m_spStream->timeEnd() + gap);
}

void AVStreamBlock::setPad(int pad)
{
	m_pad = pad;
	this->repaint();
}

void AVStreamBlock::setBlockPre(AVStreamBlock* pPre)
{
	m_pBlockPre = pPre;
}

void AVStreamBlock::setBlockBack(AVStreamBlock* pBack)
{
	m_pBlockBack = pBack;
}

void AVStreamBlock::removeFromLink()
{
	if (m_pBlockPre != nullptr)
	{
		m_pBlockPre->setBlockBack(m_pBlockBack);
	}

	if (m_pBlockBack != nullptr)
	{
		m_pBlockBack->setBlockPre(m_pBlockPre);
	}

	m_pBlockBack = nullptr;
	m_pBlockPre = nullptr;
}

void AVStreamBlock::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	if (m_spStream)
		painter.setBrush(m_spStream->backgroundBrush());
	else
		painter.setBrush(Qt::NoBrush);

	QPen pen;
	pen.setColor(Qt::white);

	QRect drawRect = this->rect();
	drawRect.adjust(0, m_pad, 0, -m_pad);
	painter.drawRect(drawRect); // 在位置 (10, 10) 绘制一个 100x100 像素的矩形
}

void AVStreamBlock::onStartChanged(double start)
{
	int endPos = this->x() + this->width();
	int startPos = start * (m_spStreamManager ? m_spStreamManager->sizePerSecond() : 10);

	this->setGeometry(startPos, 0, endPos - startPos, this->height());
}

void AVStreamBlock::onEndChanged(double end)
{
	int startPos = this->x();
	int endPos = end * (m_spStreamManager ? m_spStreamManager->sizePerSecond() : 10);

	this->setGeometry(startPos, 0, endPos - startPos, this->height());
}

void AVStreamBlock::resetGeometry()
{
	int newX = m_spStream->timeStart() * m_spStreamManager->sizePerSecond();
	int newWidth = m_spStream->timeEnd() * m_spStreamManager->sizePerSecond() - newX;
	this->setGeometry(newX, 0, newWidth, this->height());
}

AVStreamItemWidget::AVStreamItemWidget(std::shared_ptr<StreamManager>& spStreamManager, QWidget* p)
	: QWidget(p), m_spStreamManager(spStreamManager)
{
	this->setMouseTracking(true);
	this->initBlockMenu();
}

void AVStreamItemWidget::initBlockMenu()
{
	m_pMenuBlock = new QMenu(this);

	QAction* pActionTime = new QAction("时间设置", m_pMenuBlock);
	QAction* pActionRemove = new QAction("移除", m_pMenuBlock);
	m_pMenuBlock->addAction(pActionTime);
	m_pMenuBlock->addAction(pActionRemove);

	connect(pActionTime, &QAction::triggered, this, &AVStreamItemWidget::onBlockTimeTriggered);
	connect(pActionRemove, &QAction::triggered, this, &AVStreamItemWidget::onBlockRemoveTriggered);
}

void AVStreamItemWidget::addBlock(AVStreamBlock* pBlock)
{
	//暂时只支持一个
	if (m_blkPtrIndex.size() > 0 || pBlock == nullptr)
		return;

	StreamPtr spStream = pBlock->getStreamPtr();
	if (!spStream || !spStream->valid())
		return;

	pBlock->setParent(this);
	int x = m_blkPtrs.size() > 0 ? m_blkPtrs.back()->x() + m_blkPtrs.back()->width()
		: spStream->timeStart() * m_spStreamManager->sizePerSecond();

	if (!m_blkPtrs.isEmpty())
	{
		m_blkPtrs.back()->setBlockBack(pBlock);
		pBlock->setBlockPre(m_blkPtrs.back());
	}
	m_blkPtrs.append(pBlock);
	m_blkPtrIndex[pBlock] = m_blkPtrs.size() - 1;
	connect(spStream.get(), &IFAVStream::endChanged, this, &AVStreamItemWidget::onStreamEndChanged);
	emit spStream->endChanged(spStream->timeEnd());

	pBlock->installEventFilter(this);
	this->setFixedWidth(x + pBlock->width() + 3);
	pBlock->move(x, 0);
	pBlock->setFixedHeight(this->height());
	pBlock->show();
}

double AVStreamItemWidget::endTime()const
{
	StreamPtr spEndStream;
	if (!m_blkPtrs.isEmpty())
		spEndStream = m_blkPtrs.back()->getStreamPtr();

	return spEndStream ? spEndStream->timeEnd() : 0;
}

bool AVStreamItemWidget::eventFilter(QObject* watched, QEvent* event)
{
	static constexpr int ls_gap = 5;
	auto pStreamBlock = dynamic_cast<AVStreamBlock*>(watched);
	if (pStreamBlock != nullptr && m_blkPtrIndex.contains(pStreamBlock) && m_spStreamManager->isPaused())
	{
		//qDebug() << event->type();
		auto xInLeft = [pStreamBlock](int x)
		{
			return std::abs(x - pStreamBlock->x()) < ls_gap;
		};

		auto xInRight = [pStreamBlock](int x)
		{
			return std::abs(pStreamBlock->x() + pStreamBlock->width() - x) < ls_gap;
		};

		if (event->type() == QEvent::HoverMove)
		{
			QHoverEvent* hoverEvent = (QHoverEvent*)event;
			const int x = pStreamBlock->mapToParent(hoverEvent->pos()).x();
			if (m_pressedState == PressedState::NoPressed)
			{
				if (xInLeft(x) || xInRight(x))
					pStreamBlock->setCursor(Qt::SizeHorCursor);
				else
					pStreamBlock->setCursor(Qt::ArrowCursor);
			}
			else if (m_pressedState == PressedState::LeftPressed)
			{
				int xRightMove = x - m_xPosPressed;
				int objX = std::max(xRightMove + m_xPrevious, m_xDragMin);
				objX = std::min(objX, m_xDragMax);
				xRightMove = objX - m_xPrevious;

				pStreamBlock->setGeometry(objX, 0, m_widthPrevious - xRightMove, pStreamBlock->height());
			}
			else if (m_pressedState == PressedState::RightPressed)
			{
				int xLeftMoved = m_xPosPressed - x;
				int objXEnd = std::max(m_xPrevious - xLeftMoved, m_xDragMin);
				objXEnd = std::min(objXEnd, m_xDragMax);

				int blkX = pStreamBlock->x();
				pStreamBlock->setGeometry(blkX, 0, objXEnd - blkX, pStreamBlock->height());
			}
		}
		else if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouseEvent = (QMouseEvent*)event;
			if (mouseEvent->button() == Qt::LeftButton)
			{
				int blkIndex = m_blkPtrIndex[pStreamBlock];
				const int x = pStreamBlock->mapToParent(mouseEvent->pos()).x();
				if (xInLeft(x))
				{
					m_xPosPressed = x;
					m_pBlockPressed = pStreamBlock;
					m_pressedState = PressedState::LeftPressed;

					auto spStream = pStreamBlock->getStreamPtr();
					m_xDragMin = spStream->startMin() * m_spStreamManager->sizePerSecond();
					m_xDragMax = spStream->timeEnd() * m_spStreamManager->sizePerSecond();
					m_xPrevious = pStreamBlock->x();
					m_widthPrevious = pStreamBlock->width();

					if (blkIndex > 0)
					{
						auto pBlk = m_blkPtrs[blkIndex - 1];
						m_xDragMax = std::min(m_xDragMax, pBlk->x() + pBlk->width() + 1);
					}
				}
				else if (xInRight(x))
				{
					m_xPosPressed = x;
					m_pressedState = PressedState::RightPressed;
					m_pBlockPressed = pStreamBlock;

					auto spStream = pStreamBlock->getStreamPtr();
					m_xDragMin = spStream->timeStart() * m_spStreamManager->sizePerSecond();
					m_xDragMax = spStream->endMax() * m_spStreamManager->sizePerSecond();
					m_xPrevious = pStreamBlock->x() + pStreamBlock->width();
					m_widthPrevious = pStreamBlock->width();

					if (blkIndex < m_blkPtrs.size() - 1)
					{
						m_xDragMax = std::min(m_xDragMax, m_blkPtrs[blkIndex + 1]->x() - 1);
					}
				}
			}
			emit pStreamBlock->pressed();
			return true;
			//pStreamBlock->setFocus();
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			if (m_pBlockPressed != nullptr)
			{
				if (m_pressedState == PressedState::LeftPressed)
					pStreamBlock->onStartXChanged();
				else if (m_pressedState == PressedState::RightPressed)
					pStreamBlock->onEndXChanged();

				m_pressedState = PressedState::NoPressed;
				m_pBlockPressed->setCursor(Qt::ArrowCursor);
			}

			QMouseEvent* mouseEvent = (QMouseEvent*)event;
			if (mouseEvent->button() == Qt::RightButton)
			{
				m_pBlockForMenu = pStreamBlock;
				m_pMenuBlock->exec(m_pBlockForMenu->mapToGlobal(mouseEvent->pos()));
			}
			return true;
		}

	}

	return QWidget::eventFilter(watched, event);
}

void AVStreamItemWidget::onStreamEndChanged(double end)
{
	if (m_blkPtrs.size() == 0)
		return;
	auto senderStream = dynamic_cast<IFAVStream*>(sender());
	if (senderStream != m_blkPtrs.back()->getStreamPtr().get())
		return;

	auto pBlock = m_blkPtrs.back();
	this->setFixedWidth(pBlock->x() + pBlock->width() + 3);
	emit backEndChanged(end);
}

void AVStreamItemWidget::onBlockTimeTriggered()
{
	if (m_pBlockForMenu)
	{
		emit blockTimeNeedSet(m_pBlockForMenu);
	}
}

void AVStreamItemWidget::onBlockRemoveTriggered()
{
	if (m_pBlockForMenu)
	{
		m_spStreamManager->removeStream(m_pBlockForMenu->getStreamPtr());

		bool bBack = false;
		if (m_blkPtrIndex.contains(m_pBlockForMenu))
		{
			bBack = (m_blkPtrIndex[m_pBlockForMenu] == m_blkPtrs.size() - 1);
			m_blkPtrs.removeAt(m_blkPtrIndex[m_pBlockForMenu]);
			m_blkPtrIndex.remove(m_pBlockForMenu);
		}

		if (bBack)
		{
			emit backEndChanged(this->endTime());
		}

		auto spStream = m_pBlockForMenu->getStreamPtr();
		emit spStream->needRefresh();

		m_pBlockForMenu->removeEventFilter(this);
		m_pBlockForMenu->deleteLater();
		m_pBlockForMenu = nullptr;
	}
}

StreamTimeSlider::StreamTimeSlider(FAVPlayer* pPlayer, QWidget* parent) :QWidget(parent), m_pPlayer(pPlayer)
{
	this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	this->setFixedWidth(5);
}

void StreamTimeSlider::setManager(const std::shared_ptr<StreamManager>& spManager)
{
	m_spManager = spManager;
}

void StreamTimeSlider::modifyPos()
{
	this->setFixedHeight(this->parentWidget()->height());
	this->move(m_prop * m_endPos, 0);
}

void StreamTimeSlider::setEndPos(int endPos)
{
	endPos = std::max(0, endPos);
	m_endPos = std::min(endPos, parentWidget()->width() - this->width());
	if (endPos < this->x())
		this->move(m_endPos, 0);
}

void StreamTimeSlider::moveProp(double p)
{
	if (p < 0)
		p = 0;
	else if (p > 1)
		p = 1;

	//qDebug() << p << m_endPos * p;
	this->move(m_endPos * p, 0);
}

void StreamTimeSlider::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_pPlayer || m_pPlayer->state() != PlayState::Play)
		{
			m_pressedPoint = event->pos();
			m_pressed = true;
		}
	}
	QWidget::mousePressEvent(event);
}

void StreamTimeSlider::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit seekProp(m_prop);
		m_pressed = false;
	}
	QWidget::mouseReleaseEvent(event);
}

void StreamTimeSlider::mouseMoveEvent(QMouseEvent* event)
{
	if (m_pressed)
	{
		QPoint nxtPt(std::max(event->pos().x() - m_pressedPoint.x() + this->x(), 0), 0);
		nxtPt.setX(std::min(nxtPt.x(), this->parentWidget()->width() - this->width()));
		nxtPt.setX(std::min(m_endPos, nxtPt.x()));
		this->move(nxtPt);
	}
	QWidget::mouseMoveEvent(event);
}

void StreamTimeSlider::leaveEvent(QEvent* event)
{
	m_pressed = false;
	QWidget::leaveEvent(event);
}

void StreamTimeSlider::moveEvent(QMoveEvent* event)
{
	m_prop = m_endPos > 0 ? double(event->pos().x()) / m_endPos : 0;
	QWidget::moveEvent(event);
}

void StreamTimeSlider::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	painter.setBrush(QBrush(Qt::white));
	painter.setPen(Qt::NoPen);

	QRect drawRect = this->rect();
	painter.drawRect(drawRect); // 在位置 (10, 10) 绘制一个 100x100 像素的矩形
}

StreamSliderController::StreamSliderController(FAVPlayer* pPlayer, StreamTimeSlider* pSlider)
	:ITimeObserver(pPlayer, pSlider), m_pSlider(pSlider)
{

}

void StreamSliderController::onSecondChanged(double sec)
{
	m_second = std::max(sec, 0.0);
	if (m_duration > 0 && m_pSlider)
	{
		double prop = sec / m_duration;
		m_pSlider->moveProp(prop);
	}
}

void StreamSliderController::onDurationChanged(double dur)
{
	m_duration = dur;
	if (m_duration > 0)
	{
		m_second = std::max(0.0, m_second);
		double prop = m_second / m_duration;
		m_pSlider->moveProp(prop);
	}
}

namespace
{
	static constexpr int g_srMinSize = 20;
	static constexpr int g_srMinPad = 5;
}

VideoStreamRectSetWidget::VideoStreamRectSetWidget(const VideoStreamPtr& spStream, QRectF rctf)
	: m_wpVStream(spStream), m_rectPropGeo(rctf)
{
	this->setMinimumSize(g_srMinSize, g_srMinSize);
	this->setMouseTracking(true);
}

void VideoStreamRectSetWidget::bindVideoPlayer(VideoOpenGLPlayer* pVPlayer)
{
	if (pVPlayer)
	{
		this->setParent(pVPlayer);
		this->onRectDisplayChanged(pVPlayer->getRectDisplay());

		connect(pVPlayer, &VideoOpenGLPlayer::rectChanged, this, &VideoStreamRectSetWidget::onRectDisplayChanged);
		connect(pVPlayer, &VideoOpenGLPlayer::willDestroy, this, &VideoStreamRectSetWidget::onVideoPlayerDestroy);
		connect(this, &VideoStreamRectSetWidget::repaintReady, pVPlayer, &VideoOpenGLPlayer::repaintFrame);
	}
}

void VideoStreamRectSetWidget::showFrame(const FrameSPtr& spf)
{
	m_imageBackGround = spf->toQImage();
}

void VideoStreamRectSetWidget::enterSetting()
{
	m_bRectSetting = true;
	emit rectWillSet();
	emit repaintReady();
}

void VideoStreamRectSetWidget::exitSetting()
{
	m_imageBackGround = QImage();
	m_bRectSetting = false;
	emit setWillFinish();
	emit repaintReady();
}

void VideoStreamRectSetWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	if (m_bRectSetting && !m_imageBackGround.isNull())
	{
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.drawImage(this->rect(), m_imageBackGround);
	}

	QPen pen;
	pen.setColor(Qt::white);
	painter.drawRect(this->rect());
}

void VideoStreamRectSetWidget::hideEvent(QHideEvent* event)
{
	this->exitSetting();
	QWidget::hideEvent(event);
}

void VideoStreamRectSetWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_bRectSetting)
			this->enterSetting();

		int px = event->pos().x(), py = event->pos().y();
		bool leftPressed = px < g_srMinPad;
		bool rightPressed = px > this->width() - g_srMinPad;
		//按住了上面
		if (py < g_srMinPad)
		{
			if (leftPressed)
				m_statePressed = PressedState::LTPressed;
			else if (rightPressed)
				m_statePressed = PressedState::RTPressed;
			else
				m_statePressed = PressedState::TopPressed;
		}
		else if (py < this->height() - g_srMinPad)
		{
			if (leftPressed)
				m_statePressed = PressedState::LeftPressed;
			else if (rightPressed)
				m_statePressed = PressedState::RightPressed;
			else
				m_statePressed = PressedState::InnerPressed;
		}
		else
		{
			if (leftPressed)
				m_statePressed = PressedState::LBPressed;
			else if (rightPressed)
				m_statePressed = PressedState::RBPressed;
			else
				m_statePressed = PressedState::BottomPressed;
		}

		m_posPressed = this->mapToParent(event->pos());
		m_rectPressed = QRect(this->x(), this->y(), this->width(), this->height());
	}
}

void VideoStreamRectSetWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_statePressed != PressedState::NoPressed)
	{
		m_statePressed = PressedState::NoPressed;
		this->updateGeoRect();
	}
}

void VideoStreamRectSetWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_statePressed == PressedState::NoPressed)
	{
		int px = event->pos().x(), py = event->pos().y();
		bool leftPressed = px < g_srMinPad;
		bool rightPressed = px > this->width() - g_srMinPad;

		if (py < g_srMinPad)
		{
			if (leftPressed)
				this->setCursor(Qt::SizeFDiagCursor);
			else if (rightPressed)
				this->setCursor(Qt::SizeBDiagCursor);
			else
				this->setCursor(Qt::SizeVerCursor);
		}
		else if (py < this->height() - g_srMinPad)
		{
			if (leftPressed || rightPressed)
				this->setCursor(Qt::SizeHorCursor);
			else
				this->setCursor(Qt::ArrowCursor);
		}
		else
		{
			if (leftPressed)
				this->setCursor(Qt::SizeBDiagCursor);
			else if (rightPressed)
				this->setCursor(Qt::SizeFDiagCursor);
			else
				this->setCursor(Qt::SizeVerCursor);
		}

		QWidget::mouseMoveEvent(event);
		return;
	}

	if (m_statePressed == PressedState::InnerPressed)
	{
		this->innerPressedMove(event);
		//this->updateGeoRect();
		return;
	}

	QRect newRect = m_rectPressed;
	if (m_statePressed == PressedState::LeftPressed || m_statePressed == PressedState::LBPressed
		|| m_statePressed == PressedState::LTPressed)
	{
		this->leftPressedMove(event, newRect);
	}
	else if (m_statePressed == PressedState::RightPressed || m_statePressed == PressedState::RTPressed
		|| m_statePressed == PressedState::RBPressed)
	{
		this->rightPressedMove(event, newRect);
	}

	if (m_statePressed == PressedState::TopPressed || m_statePressed == PressedState::LTPressed
		|| m_statePressed == PressedState::RTPressed)
	{
		this->topPressedMove(event, newRect);
	}
	else if (m_statePressed == PressedState::BottomPressed || m_statePressed == PressedState::LBPressed
		|| m_statePressed == PressedState::RBPressed)
	{
		this->bottomPressedMove(event, newRect);
	}

	//qDebug() << m_rectPressed << newRect;
	this->setGeometry(newRect);

}

void VideoStreamRectSetWidget::innerPressedMove(QMouseEvent* event)
{
	QPoint pt = this->mapToParent(event->pos());
	int xGap = pt.x() - m_posPressed.x(), yGap = pt.y() - m_posPressed.y();

	//qDebug() << m_rectPressed << pt;

	QRect newRect = m_rectPressed;
	newRect.translate(xGap, yGap);
	this->setGeometry(newRect);
	//qDebug() << newRect;
}

void VideoStreamRectSetWidget::leftPressedMove(QMouseEvent* event)
{
	QPoint pt = event->pos();
	int xGap = pt.x() - m_posPressed.x();

	QRect newRect = m_rectPressed;
	newRect.setX(newRect.x() + xGap);
	newRect.setLeft(newRect.left() + xGap);
	this->setGeometry(newRect);
}

void VideoStreamRectSetWidget::leftPressedMove(QMouseEvent* event, QRect& rect)
{
	QPoint pt = this->mapToParent(event->pos());
	int xGap = pt.x() - m_posPressed.x();

	rect.setX(rect.x() + xGap);
}

void VideoStreamRectSetWidget::rightPressedMove(QMouseEvent* event)
{
	QPoint pt = event->pos();
	int xGap = pt.x() - m_posPressed.x();

	QRect newRect = m_rectPressed;
	newRect.setRight(newRect.right() + xGap);
	this->setGeometry(newRect);
}

void VideoStreamRectSetWidget::rightPressedMove(QMouseEvent* event, QRect& rect)
{
	QPoint pt = this->mapToParent(event->pos());
	int xGap = pt.x() - m_posPressed.x();

	rect.setWidth(rect.width() + xGap);
}

void VideoStreamRectSetWidget::topPressedMove(QMouseEvent* event)
{
	QPoint pt = event->pos();
	int xGap = pt.y() - m_posPressed.y();

	QRect newRect = m_rectPressed;
	newRect.setTop(newRect.top() + xGap);
	this->setGeometry(newRect);
}

void VideoStreamRectSetWidget::topPressedMove(QMouseEvent* event, QRect& rect)
{
	QPoint pt = this->mapToParent(event->pos());
	int yGap = pt.y() - m_posPressed.y();

	rect.setY(rect.y() + yGap);
}

void VideoStreamRectSetWidget::bottomPressedMove(QMouseEvent* event)
{
	QPoint pt = event->pos();
	int xGap = pt.y() - m_posPressed.y();

	QRect newRect = m_rectPressed;
	newRect.setBottom(newRect.bottom() + xGap);
	this->setGeometry(newRect);
}

void VideoStreamRectSetWidget::bottomPressedMove(QMouseEvent* event, QRect& rect)
{
	QPoint pt = this->mapToParent(event->pos());
	int yGap = pt.y() - m_posPressed.y();

	rect.setHeight(rect.height() + yGap);
}

void VideoStreamRectSetWidget::onRectDisplayChanged(const QRect& rectDisplay)
{
	m_rectRelative = rectDisplay;

	int newX = rectDisplay.x() + m_rectPropGeo.x() * rectDisplay.width();
	int newY = rectDisplay.y() + m_rectPropGeo.y() * rectDisplay.height();
	int newHeight = rectDisplay.height() * m_rectPropGeo.height();
	int newWidth = rectDisplay.width() * m_rectPropGeo.width();

	this->setGeometry(newX, newY, newWidth, newHeight);
}

void VideoStreamRectSetWidget::updateGeoRect()
{
	if (!m_rectRelative.isNull())
	{
		float gx = (this->x() - m_rectRelative.x()) / (float)(m_rectRelative.width());
		float gy = (this->y() - m_rectRelative.y()) / (float)(m_rectRelative.height());
		float gh = (float)(this->height()) / m_rectRelative.height();
		float gw = (float)(this->width()) / m_rectRelative.width();

		m_rectPropGeo = QRectF(gx, gy, gw, gh);
		//qDebug() << m_rectPropGeo << this->x() << this->y();
		emit rectPropGeoChanged(m_rectPropGeo);
	}
}

void VideoStreamRectSetWidget::onVideoPlayerDestroy()
{
	m_rectRelative = QRect();

	if (this->parentWidget())
		this->disconnect(this->parentWidget());
	this->setParent(nullptr);
}
