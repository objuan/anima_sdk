#pragma once
#include "Anima_nsbegin.h"

enum QueryFlags
{
	CV_SOLID		= 1<<0,
	CV_IMAGINARY	= 1<<1,
	CV_GIZMO		= 1<<2,
	CV_NULL			= 1<<3,
};

#define UPM 1.0f
enum { LIGHT_SPOT, LIGHT_POINT };

class Line3D;
class View;
class PhysicsClass;
class Listener;

class ANIMA_CORE_API OgreRenderer
{
public:
	
	Listener* timer;

	OgreRenderer():
		mRoot(0), ogreScene(0), mWindow(0), 
			currentViewportLayout(7), renderMode(1), gridOn(true),timer(NULL)
	{ }
	virtual ~OgreRenderer(); //if(mRoot) ANIMA_DEL mRoot; 
	//if(mRaySceneQuery) ANIMA_DEL mRaySceneQuery; 

	int	Initialise(Ogre::Root *root,Ogre::SceneManager* scene,Ogre::RenderWindow* window,Ogre::RenderWindow* windowPhysic);//HWND hwnd);
	void loadup();

	PhysicsClass *GetPhysics(){return physicsClass;}
	View *GetPhysicView();

	//sets out viewport pos
	void	_setupViews(int layout);	//view layout num 0-7
	void    GetViewList(QList<View *> &list);
	virtual void OnTimerTick(int timeMilliseconds);
	void UpdatePhysic();
	Ogre::SceneManager *GetPhysicSceneMgr(){return physicSceneMgr;}

	void 	OnResize();
	void	OnResizePhysic();
	void	render();
	void	centerView(int viewnum);

	Ogre::RenderWindow *GetRenderWindow(){return mWindow;}

	Ogre::SceneManager*	getSceneManager();		//naughty
	float			getModelSize(Ogre::Entity* ent);			//returns size of a model (diameter)
	void			shutDown();

	int GetWinW(){return mWindow->getWidth();}
	int GetWinH(){return mWindow->getHeight();}
	View *GetView(int x,int y);
	bool IsMouseOnDrags(int mousex,int mousey,bool isLeftDown);

	PhysicsClass *physicsClass;
	int		currentViewportLayout;
	int			viewsRender;	//tells us to render again (all views)
	bool		viewsResize;	//tells us need to do glviewport
	
	//int entityID;
	//int lightID;
	//Ogre::Light* sun;
	//Ogre::Light* sun2;
	View*		mouseView;				//current view mouse is in
	View*		currentView;			//last view used
	Ogre::Entity*	marker;
	int				renderMode;
	int Hbar;
	int Vbar;
	float HbarPerc;
	float VbarPerc;
	int			Hcursor;		//bool
	int			Vcursor;		//bool
	int			CrossCursor;	//bool
	int			XCursor;
	float TOOLBAR;
	float TOOLBAR2;

	QList<View*>		viewList;
	float			frameTime;

	View *physicView;

	// mouse

	Ogre::Vector3 center;
	bool invertX;
	bool invertY;
	Ogre::Vector3 upAxis;

	bool symEnabled;
	double lastTimePhysics;

protected:
	std::string		createTexture(const char * name);
    Ogre::RaySceneQuery *mRaySceneQuery; 
	int			HbarOn;			//0=off 1=on 2=top 3=bottom
	int			VbarOn;
	bool		groundOn;
	bool		gridOn;
	Ogre::Entity*		groundPlane;
private:
	//vars
	Ogre::Root*	mRoot;
	Ogre::SceneManager* ogreScene;
	Ogre::SceneManager *physicSceneMgr;

	Ogre::RenderWindow* mWindow;
	Ogre::RenderWindow* mWindowPhysic;
	//Line3D*		line;

};

#include "Anima_nsend.h"
