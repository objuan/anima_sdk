#pragma once

class QTTask : public QThread
{
private:
	QApplication *app;
public:
	QTTask(QApplication *app);
	~QTTask();

	// QThread
    void run();

};

class myLogListener : public Ogre::LogListener, QWidget
{
public:
	myLogListener(QWidget* parent = 0);
	~myLogListener();

	void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );

	QString getMsg();

private:
	QString logMsg;
};

enum GLViewMouseAction
{ 
	MouseAction_CAMERA_NOTHING, 
	MouseAction_CAMERA_PAN,
	MouseAction_CAMERA_ROTATE,
	MouseAction_CAMERA_ZOOM, 
	MouseAction_CAMERA_ZOOM_EXT
};

class GLView : public QWidget//QGLWidget 
{
	//Q_OBJECT
public:
    enum ViewControlMode
    {
        ViewControlNone = 0,
        ViewControlPan,
        ViewControlZoom,
        ViewControlRotate
    };
private:
	//bool mainView;
	//GLMultiView *glMultiView;
	QCursor  editCursor;

	Ogre::String oldMsgLog;
	myLogListener* mLogListener;

	//Deplacement
	Ogre::Vector3 mDirection;
	Ogre::Real mRotate;
	Ogre::Real mMove;
	bool isMoving;
	QPoint mousePressPos;
	QPoint mousePos;
	float angleX, angleY;
	float rotX, rotY;

	bool mouseLeftPressed;
	bool mouseRightPressed;
	bool mouseMiddleBtn;
	bool isLoopOn;
	bool isAnimEnabled;

    ViewControlMode viewControlMode;

public:
	GLView();
	void Start();
	void Stop();
//	void SetMouseAction(GLViewMouseAction action);
	int OpenSelectMultiContextMenu(const QPoint &pos);
	int OpenContextMenu(const QPoint &pos,Anima::SceneObject* e);
    void SetDefCursor(const QCursor &cur);
	void Update();

    ViewControlMode GetViewControlMode() {return viewControlMode;}
    void SetViewControlMode(ViewControlMode mode) {viewControlMode = mode;}
protected:

	void resizeEvent(QResizeEvent* evt);
	void timerEvent(QTimerEvent* evt);
	void paintEvent(QPaintEvent* evt);

	void dragEnterEvent(QDragEnterEvent* evt);
	void dropEvent(QDropEvent* evt);

	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);

};

