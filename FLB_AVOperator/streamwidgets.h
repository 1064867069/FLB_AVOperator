#ifndef STREAMWIDGETS_H
#define STREAMWIDGETS_H

#include "interfaces.h"
#include <QWidget>
#include <QMenu>

class AVStreamItemWidget;
class StreamManager;

class AVStreamBlock :public QWidget
{
	Q_OBJECT
public:
	AVStreamBlock(const StreamPtr& spStream, std::shared_ptr<StreamManager> pManager, QWidget* p = nullptr);

	~AVStreamBlock();

	StreamPtr getStreamPtr()const;

	void setPad(int pad);

	void setBlockPre(AVStreamBlock* pPre);

	void setBlockBack(AVStreamBlock* pBack);

	void removeFromLink();
public slots:
	void onStartXChanged();

	void onEndXChanged();

	void secMoveRight(double gap);

	void secStartMoveRight(double gap);

	void secEndMoveRight(double gap);

signals:
	void pressed();

protected slots:
	virtual void paintEvent(QPaintEvent* event) override;

private slots:
	void onStartChanged(double start);
	void onEndChanged(double end);
	void resetGeometry();

private:
	AVStreamBlock* m_pBlockPre = nullptr;
	AVStreamBlock* m_pBlockBack = nullptr;

	StreamPtr m_spStream;
	std::shared_ptr<StreamManager> m_spStreamManager;
	int m_pad = 5;
};

enum class PressedState
{
	NoPressed,

	LeftPressed,
	RightPressed,
	TopPressed,
	BottomPressed,
	LTPressed,
	LBPressed,
	RTPressed,
	RBPressed,
	InnerPressed
};

class StreamTimeWidget;
class AVStreamItemWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AVStreamItemWidget(std::shared_ptr<StreamManager>& m_spStreamManager, QWidget* p = nullptr);

	void addBlock(AVStreamBlock* block);

	double endTime()const;
signals:
	void backEndChanged(double);

	void blockTimeNeedSet(AVStreamBlock*);
protected slots:
	bool eventFilter(QObject* watched, QEvent* event)Q_DECL_OVERRIDE;

private:
	void initBlockMenu();

private slots:
	void onStreamEndChanged(double end);

	void onBlockTimeTriggered();

	void onBlockRemoveTriggered();
private:
	std::shared_ptr<StreamManager> m_spStreamManager;
	QHash<AVStreamBlock*, int> m_blkPtrIndex;
	QList<AVStreamBlock*> m_blkPtrs;

	QMenu* m_pMenuBlock;
	AVStreamBlock* m_pBlockForMenu = nullptr;

	AVStreamBlock* m_pBlockPressed = nullptr;
	PressedState m_pressedState = PressedState::NoPressed;
	int m_xPosPressed = 0;
	int m_xPrevious = 0;
	int m_widthPrevious = 0;
	int m_xDragMin = 0;
	int m_xDragMax = 0;
};

class StreamTimeSlider : public QWidget
{
	Q_OBJECT
public:
	explicit StreamTimeSlider(FAVPlayer* pPlayer, QWidget* parent = nullptr);

	void setManager(const std::shared_ptr<StreamManager>& spManager);
public slots:
	//上层窗口尺寸变化专用
	void modifyPos();

	void setEndPos(int endPos);

	void moveProp(double p);
signals:
	void seekProp(double p);

protected slots:
	virtual void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;
	virtual void moveEvent(QMoveEvent* event)Q_DECL_OVERRIDE;
	virtual void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
	FAVPlayer* m_pPlayer;
	std::shared_ptr<StreamManager> m_spManager;

	double m_prop = 0;
	int m_endPos = 0;

	QPoint m_pressedPoint;
	bool m_pressed = false;
};

class StreamSliderController :public ITimeObserver
{
	Q_OBJECT
public:
	explicit StreamSliderController(FAVPlayer* pPlayer, StreamTimeSlider* pSlider);

public slots:
	virtual	void onDurationChanged(double)Q_DECL_OVERRIDE;

protected slots:
	virtual void onSecondChanged(double)Q_DECL_OVERRIDE;
private:
	StreamTimeSlider* m_pSlider;
};

class VideoOpenGLPlayer;
class VideoStreamRectSetWidget :public QWidget
{
	Q_OBJECT
public:
	explicit VideoStreamRectSetWidget(const VideoStreamPtr& spStream, QRectF rctf = QRectF(0, 0, 1, 1));

	void bindVideoPlayer(VideoOpenGLPlayer* pVPlayer);

public slots:
	void showFrame(const FrameSPtr& spf);

	void enterSetting();

	void exitSetting();
signals:
	void pressed();

	void released();

	void rectWillSet();

	void setWillFinish();

	void repaintReady();

	void rectPropGeoChanged(const QRectF&);

	void rectPropCutChanged(const QRectF&);
protected slots:
	virtual void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	virtual void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	virtual void hideEvent(QHideEvent* event)Q_DECL_OVERRIDE;
private slots:
	void onRectDisplayChanged(const QRect& rectDisplay);

	void innerPressedMove(QMouseEvent* event);

	void leftPressedMove(QMouseEvent* event);
	void leftPressedMove(QMouseEvent* event, QRect& rect);

	void rightPressedMove(QMouseEvent* event);
	void rightPressedMove(QMouseEvent* event, QRect& rect);

	void topPressedMove(QMouseEvent* event);
	void topPressedMove(QMouseEvent* event, QRect& rect);

	void bottomPressedMove(QMouseEvent* event);
	void bottomPressedMove(QMouseEvent* event, QRect& rect);

	void updateGeoRect();

	void onVideoPlayerDestroy();
private:
	VideoStreamWPtr m_wpVStream;
	QImage m_imageBackGround;

	QRectF m_rectPropGeo;
	QRectF m_rectPropCut;
	QRect m_rectPressed;
	QRect m_rectRelative;
	QPoint m_posPressed;

	PressedState m_statePressed = PressedState::NoPressed;
	bool m_bRectSetting = false;
};

#endif
