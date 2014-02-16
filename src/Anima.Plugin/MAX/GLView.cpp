#include "stdafx.h"

#include "GLView.h"
#include "Core/Context.h"
#include "Core/Animation.h"
#include "Core/Film.h"
#include "Core/ModelInstance.h"
#include "Core/AnimationPart.h"
#include "Core/PathOffsetFunction.h"
#include "Core/FilmLayer.h"
#include "Core/CoreException.h"
#include "Core/EditorManager.h"

#include "Gfx/gfxContext.h"
#include "Gfx/Render/View.h"

//using namespace qglviewer;
using namespace std;

const char *glView_desc[] = {"Top","Bottom","Right","Left","Front","Back","Perpective","Camera"};
const char *glView_mode[] = {"Hit","Wireframe","Solid","Smooth","Textured","Physic","Admin"};

static int _id=0;

// ==================================================

myLogListener::myLogListener(QWidget* parent) : LogListener(), QWidget(parent)
{

}
myLogListener::~myLogListener()
{

}
void myLogListener::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
{
	logMsg = message.c_str();
}

QString myLogListener::getMsg()
{
	return logMsg;
}

// ============================================================================

QTTask::QTTask(QApplication *app) : app(app)
{
}

QTTask::~QTTask()
{
}

void QTTask::run()
{
	while(true)
	{
		QApplication::processEvents();
		QThread::sleep(10);
	}
	/*int ret = app->exec();
	int y=0;*/
}

// ============================================================================

GLView::GLView()
{
	setWindowTitle("Ogre View");
	editCursor =Anima::ResourceManager::Singleton.cursor_normal;

	setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

	this->resize(1,1);
	this->hide();
	this->setMouseTracking(true);
	
	mouseLeftPressed = false;
	mouseRightPressed = false;
	mouseMiddleBtn = false;
	isMoving = false;
	isLoopOn = false;
	isAnimEnabled = false;

	mDirection = Ogre::Vector3::ZERO;
	mMove = 5;
	mRotate = 0.0005;
	mousePos = QPoint(0, 0);
	mousePressPos = QPoint(0, 0);
    angleX = 0.0; // Added these because the display started spinning
    angleY = 0.0; // when I upgraded to Qt4.2, they were left undefined.
	rotX = 0.0;
	rotY = 0.0;

	oldMsgLog = "";

	setAcceptDrops(true);

	mLogListener = new myLogListener();
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener(mLogListener);
    viewControlMode = ViewControlNone;
}

void GLView::Start()
{
//	scene->GetCamera(type,camera());
	//animationTimerId_ = startTimer(AnimationPeriod());
	//animationStarted_ = true;
}

void GLView::Stop()
{
	/*animationStarted_ = false;
	if (animationTimerId_ != 0)
		killTimer(animationTimerId_);*/
}

void GLView::Update()
{
	//Ogre::Camera* camera = GfxContext::Singleton.getCamera();
	//camera->moveRelative(mDirection);
	//camera->yaw(Ogre::Radian(angleX));
	//camera->pitch(Ogre::Radian(angleY));

	Anima::GfxContext::Singleton->Update();
}

void GLView::resizeEvent(QResizeEvent *evt)
{
	try
	{
		Q_UNUSED(evt);
		Anima::GfxContext::Singleton->OnResize();
		Anima::GfxContext::Singleton->Update();
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(Ogre::Exception &e)
	{
		Anima::LogError(e.getFullDescription().c_str());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("resizeEvent"));
	}
}
void GLView::timerEvent(QTimerEvent* evt)
{
	Anima::Context::Singleton.OnTimerTick();
}

void GLView::paintEvent(QPaintEvent* evt)
{
	Q_UNUSED(evt);
	try
	{
		Update();
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(Ogre::Exception &e)
	{
		Anima::LogError(e.getFullDescription().c_str());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("paintEvent"));
	}
}

void GLView::keyPressEvent(QKeyEvent* evt)
{
	try
	{
		Ogre::Entity* mainEnt = Anima::GfxContext::Singleton->getMainEnt();
		Ogre::SceneNode* mainNode = Anima::GfxContext::Singleton->getMainNode();

		if(mainEnt != NULL && mainNode != NULL){
			switch(evt->key()){
				case Qt::Key_W:
				case Qt::Key_Up:
					rotX = -0.1;
					mainNode->pitch(Ogre::Radian(rotX));
					Update();
					break;
				case Qt::Key_S:
				case Qt::Key_Down:
					rotX = 0.1;
					mainNode->pitch(Ogre::Radian(rotX));
					Update();
					break;
				case Qt::Key_A:
				case Qt::Key_Left:
					rotY = -0.1;
					mainNode->yaw(Ogre::Radian(rotY));
					Update();
					break;
				case Qt::Key_D:
				case Qt::Key_Right:
					rotY = 0.1;
					mainNode->yaw(Ogre::Radian(rotY));
					Update();
					break;
			}
		}

	// Get event modifiers key
		const Qt::KeyboardModifiers modifiers = evt->modifiers();
		bool handled = false;
		if (handled)
			evt->accept();
		//else if (EditorManager::Singleton.KeyPressEvent(evt))
		//{
		//}
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("keyPressEvent"));
	}
}

void GLView::keyReleaseEvent(QKeyEvent* evt)
{
	try
	{
		Ogre::Entity* mainEnt = Anima::GfxContext::Singleton->getMainEnt();
		Ogre::SceneNode* mainNode = Anima::GfxContext::Singleton->getMainNode();

		if(mainEnt != NULL && mainNode != NULL){
			switch(evt->key()){
				case Qt::Key_W:
				case Qt::Key_Up:
					rotX = 0.0;
					mainNode->pitch(Ogre::Radian(rotX));
					Update();
					break;
				case Qt::Key_S:
				case Qt::Key_Down:
					rotX = 0.0;
					mainNode->pitch(Ogre::Radian(rotX));
					Update();
					break;
				case Qt::Key_A:
				case Qt::Key_Left:
					rotY = 0.0;
					mainNode->yaw(Ogre::Radian(rotY));
					Update();
					break;
				case Qt::Key_D:
				case Qt::Key_Right:
					rotY = 0.0;
					mainNode->yaw(Ogre::Radian(rotY));
					Update();
					break;
			}
		}
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("keyReleaseEvent"));
	}
}

void GLView::mousePressEvent(QMouseEvent* evt)
{
	try
	{
        const Qt::KeyboardModifiers modifiers = evt->modifiers();
		if(evt->button() == Qt::LeftButton)
		{
			mousePressPos = evt->pos();
			mousePos = evt->pos();
			mouseLeftPressed = true;
		}
		if(evt->button() == Qt::RightButton){
			mousePressPos = evt->pos();
			mousePos = evt->pos();
			mouseRightPressed=true;
		}
		if(evt->button() == Qt::MidButton)
		{
			mousePressPos = evt->pos();
			mousePos = evt->pos();
			mouseMiddleBtn = true;
		}

		if (modifiers != Qt::CTRL &&  modifiers != Qt::ALT && viewControlMode == ViewControlNone)
		{
			Anima::View *currentView =  Anima::GfxContext::Singleton->GetView(evt->pos().x(),evt->pos().y());
			if (currentView!=NULL)
			{
			    /*if (EditorManager::Singleton.MousePressed(currentView,this->width(),this->height(),evt))
				    return;*/
			}
		}
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("mousePressEvent"));
	}

}

void GLView::mouseReleaseEvent(QMouseEvent* evt)
{
	try
	{
		Q_UNUSED(evt);
		mouseLeftPressed = false;
		mouseRightPressed = false;
		mouseMiddleBtn = false;
        const Qt::KeyboardModifiers modifiers = evt->modifiers();
        if (modifiers != Qt::CTRL &&  modifiers != Qt::ALT && viewControlMode == ViewControlNone)
        {
		   /* if (EditorManager::Singleton.MouseRelease(evt))
                return;*/
        }
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("mouseReleaseEvent"));
	}
}

void GLView::mouseMoveEvent(QMouseEvent* evt)
{
	try
	{
		// bar ?

		if (Anima::GfxContext::Singleton->IsMouseOnDrags(evt->pos().x(),evt->pos().y(),mouseLeftPressed))
		{
			if (Anima::GfxContext::Singleton->Vcursor)
			{
				if (Anima::GfxContext::Singleton->Hcursor)
					this->setCursor(Qt::SizeAllCursor  );
				else
					this->setCursor(Qt::SizeHorCursor   );
			}
			else
				this->setCursor(Qt::SizeVerCursor  );
			if (mouseLeftPressed)
			{
				repaint();
				mousePos = evt->pos();
				evt->accept();
				return;
			}
		}
		else 
        {
            const Qt::KeyboardModifiers modifiers = evt->modifiers();
            if (modifiers == Qt::CTRL  || viewControlMode == ViewControlPan)
			{
				if (mouseRightPressed)
					setCursor(Anima::ResourceManager::Singleton.cursor_zoom);
				else
					setCursor(Anima::ResourceManager::Singleton.cursor_pan);
			}
            else if (modifiers == Qt::ALT  || viewControlMode == ViewControlRotate)
                setCursor(Anima::ResourceManager::Singleton.cursor_rotate);
            else if (viewControlMode == ViewControlZoom)
                setCursor(Anima::ResourceManager::Singleton.cursor_zoom);
            else 
			{
				this->setCursor(editCursor);
			}
        }

		// ------

		if (mouseLeftPressed || mouseRightPressed|| true)
		{
			Anima::View *currentView =  Anima::GfxContext::Singleton->GetView(evt->pos().x(),evt->pos().y());
			//Ogre::Camera* camera = GfxContext::Singleton.GetCamera(evt->pos().x(),evt->pos().y());

			if (currentView!=NULL) 
			{
				/*if (EditorManager::Singleton.MouseMove(currentView,this->width(),this->height(),evt))
					return;*/

				const Qt::KeyboardModifiers modifiers = evt->modifiers();
				QPoint currentPos = evt->pos();
                if (modifiers == Qt::CTRL || viewControlMode == ViewControlPan)
				{
					float ox =  (float)(currentPos.x() - mousePos.x()) / 10;
					float oy =  (float)(currentPos.y() - mousePos.y()) / 10;

					// MOVE
					if (mouseLeftPressed)
					{
					//	Ogre::Vector3 offset(currentPos.x() - mousePos.x(),0,currentPos.y() - mousePos.y());
					//	camera->moveRelative(offset / 10);
						currentView->panCamera(ox,oy ,0);

					}
					else if (mouseRightPressed)// && lCamera->GetProjectionType() == KFbxCamera::ePERSPECTIVE)
					{
						//Ogre::Vector3 offset(0,-currentPos.y() + mousePos.y(),0);*/
					//	camera->moveRelative(offset / 10);
						currentView->panCamera(0,0,oy);
					}
					//Update();
                    
					repaint();
					mousePos = evt->pos();
					evt->accept();
				}
				else if (modifiers == Qt::ALT || viewControlMode == ViewControlRotate)//&& lCamera->GetProjectionType() == KFbxCamera::ePERSPECTIVE)
				{
					if (mouseLeftPressed)
					{
						double angleX = currentPos.x() - mousePos.x();
						double angleY = currentPos.y() - mousePos.y();
					/*	camera->yaw(Ogre::Radian(Ogre::Degree(-angleX)));
						camera->pitch(Ogre::Radian(Ogre::Degree(-angleY)));*/
						if (!currentView->ortho)
							currentView->rotateCamera(angleX,-angleY,0);
					}
					else if (mouseRightPressed)
					{
					//	CameraPan(scene->scene,lCamera, gCamPosition, gCamCenter, gRoll,e->x()-gX, 0,-(gY-e->y()),false);
					//	updateGL();
					}
					//Update();
                    
					repaint();
					mousePos = evt->pos();
					evt->accept();
				}
                else if (viewControlMode == ViewControlZoom)
                {
                    if (mouseLeftPressed || mouseRightPressed)
                    {
                        float dy = currentPos.y() - mousePos.y();
                        currentView->rotateCamera(0,0, -(float)dy*UPM/75.0f);
                        
                        repaint();
                        
                    }
                    mousePos = evt->pos();
                }
			}
		}
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("mouseMoveEvent"));
	}
}

void GLView::wheelEvent(QWheelEvent* evt)
{
	try
	{
		Anima::View *currentView =  Anima::GfxContext::Singleton->GetView(evt->pos().x(),evt->pos().y());
		if (currentView!=NULL)
		{
			int delta = evt->delta();
			currentView->rotateCamera(0,0, (float)delta*UPM/300.0f);
			repaint();
		}
	}
	catch(Anima::CoreException &e)
	{
		Anima::LogError(e.Message());
	}
	catch(...)
	{
		Anima::LogError(QString("Errore Generico: %1").arg("wheelEvent"));
	}
}
void GLView::SetDefCursor(const QCursor &cur)
{
	editCursor = cur; 
	//setCursor(cur);
}

//**************************************************************************

void GLView::dragEnterEvent(QDragEnterEvent* evt)
{
//#if defined(Q_WS_WIN)
//    if(evt->mimeData()->hasUrls())
//    {
//        QRegExp fileMatcher("^/([A-Z]:/.*\\.mesh)$");
//        if( fileMatcher.exactMatch(evt->mimeData()->urls().first().path()) )
//            evt->acceptProposedAction();
//    }
//#else
//	QRegExp fileMatcher("^file://(/.*\\.mesh)$");
//	if( fileMatcher.exactMatch(evt->mimeData()->text()) )
//		evt->acceptProposedAction();
//#endif
};

void GLView::dropEvent(QDropEvent* evt)
{
//#if defined(Q_WS_WIN)
//    QRegExp fileMatcher("^/([A-Z]:/.*\\.mesh)$");
//    if( fileMatcher.exactMatch(evt->mimeData()->urls().first().path()) )
//    {
//        QString meshName = fileMatcher.cap(1);
//        meshName.replace("/", "\\");
//#else
//    QRegExp fileMatcher("^file://(/.*\\.mesh)$");
//    if( fileMatcher.exactMatch(evt->mimeData()->text()) )
//    {
//        QString meshName = fileMatcher.cap(1);
//#endif
//		((MainWindow*)parentWidget())->loadObj(meshName);
//        evt->acceptProposedAction();
//    }
};


// ================================
// MENU CONTESTUALE
// ================================

int GLView::OpenSelectMultiContextMenu(const QPoint &pos)
{
	return 0;
}


int GLView::OpenContextMenu(const QPoint &pos,Anima::SceneObject* obj)
{
	return 0;
}
