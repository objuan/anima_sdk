#pragma once

#include "Core/Mesh.h"

#include "Anima_nsbegin.h"
class AnimationPart;
class AnimationPartClipSpline;
class SceneObject;
class View;
class GfxLabel;

struct EditorCollisionInfo
{
	double distance;
	CollisionPoint collisionPoint;
	SceneObject *object;
	int subIdx;
	QStringList clickArgs;
	int level;
};
namespace Editor
{
enum PathModeType
{
    PathModeEdit=0,
    PathModeAddSpline=1,
    PathModeAddStairs=2,
    PathModeAddFixed=3,
    PathModeAddEscalator=4
};

enum  PickMode
{
  //  PickDefault = 0,
	PickNull = 0,
    PickSelectMove = 1,
    PickRotate = 2,
	PickAddDelPoint = 3
};

enum  PickMoveMode
{
    PickMoveLocal = 0,
    PickMoveGlobal = 1,
};

enum CursorMode
{
    CursorNormal = 0,
    CursorInsertPoint,
    CursorDelPoint,
	CursorAddPoint,
	CursorMovePath,
	CursorRotatePath,
	CursorMovePathPoint,
	CursorMovePathPointBad,
	CursorMovePathOffset,
	CursorMoveClipBound,
	CursorMoveUIAxis,
	CursorMoveActor,
	//CursorAddPointReplace,
	CursorAddPointFixed,
	CursorAddPointEscalator1,
	CursorAddPointEscalator2,
	CursorAddPointStair1,
	CursorAddPointStair2,
};
}
//close namescpase to avoid Anima::Anima namespace
#include "Anima_nsend.h"
#include "Core/Action/ActionModes/SplineDesignMode.h"
#include "Anima_nsbegin.h"

class CollisionManager
{
public:

	int objStackCount;
	int backStackCount;
	EditorCollisionInfo allStack[9999];
	EditorCollisionInfo objStack[9999];
	EditorCollisionInfo backStack[9999];
	
private:
	void BackgroundCollision(const Ogre::Vector3 &min,const Ogre::Vector3 &max,Ogre::Matrix4 &trx);
	void BackgroundCollision(Ogre::Ray &ray);
	void GUICollision(Ogre::Camera *camera,float mx, float my,float vx, float vy);
	void Sort(EditorCollisionInfo *list,int count);
	void Add(Ogre::Ray &ray);
	void Add(Ogre::MovableObject *movable,float distance,Ogre::Ray &ray);
	void Add(SceneObject *obj,float distance,const Ogre::Vector3 &contact,int subIdx);

public:
	CollisionManager();

	int allStackCount(){return objStackCount + backStackCount;}

	bool BackgroundPick(int w,int h,int mx,int my);
	bool BackgroundCollision(Ogre::Ray &ray,CollisionPoint &point);
	bool BackgroundCollision(const Ogre::Vector3 &min,const Ogre::Vector3 &max,Ogre::Matrix4 &trx,CollisionPoint &point);
	bool GUIPick(int w,int h,int mx,int my);
	bool AllPick(int w,int h,int mx,int my);
	bool RectanglePick(Ogre::Camera *mCamera,int w,int h,QRectF &selectionRectange);

	EditorCollisionInfo *GetAlltack(){return allStack;}
	EditorCollisionInfo *GetBackStack(){return backStack;}
	EditorCollisionInfo *GetObjStack(){return objStack;}

};

class ANIMA_CORE_API EditorManager : public QObject  , public BObject
{
	Q_OBJECT
friend class Command;
public:
    typedef struct MarkPoint
    {
	    double xzAngle;
	    double point[3]; 
        AnimationPart *animationPart;
	    int subIdx;
	    double space;
    };

private:
	bool disableInner;
	bool lock;
	bool mouseDown;
	bool isPathEditing;

	int last_gui_w;
	int last_gui_h;

    Editor::PickMode oldPickMode;
	Editor::PickMode pickMode;
	Editor::PickMoveMode moveMode;
    Editor::CursorMode cursorMode;

    Editor::PathModeType pathMode;
   // bool isCreatingNewPath;
    Editor::PathModeType oldPathMode;

	//AnimationPart *newPart;
    
	MouseCollisionEvent pressMouseEvent;
	MouseCollisionEvent moveMouseEvent;
	MouseCollisionEvent overMouseEvent;
	bool isOverMouse;

	QString coll_cameraName;
	//int coll_w,coll_h,coll_mx,coll_my;
	MouseCollisionEvent::MouseButtonType coll_buttonType ;
	
    MovePath * editablePath;
    MovePath * oldEditablePath;

	CollisionManager collisionManager;

	//Ogre::Vector3 lastAddPoint;
	//int lastDelPointIdx;
	QTime toolTipTime;
	bool tooltipOn;
	bool rectSelecting;
	//QDateTime lastMoveTime;
	GfxLabel *tooltipLabel;

private:

	//bool SendSelectEvent(int stackCollIdx,Qt::KeyboardModifiers &m);
	SceneObject *GetMovePath(SceneObject *so);

public:

	static EditorManager *Singleton;

	EditorManager(void);
	~EditorManager(void);

	void InitUI();

	CollisionManager &GetCollisionManager(){return collisionManager;}

	bool BackgroundPick(int mx,int my);

	int GetLastScreenW(){return last_gui_w;}
	int GetLastScreenH(){return last_gui_h;}

	bool IsOver(MouseCollisionEvent &overMouseEvent);
	//bool SelectCollisionStack(int stackCollIdx,Qt::KeyboardModifiers &m);

	int GetLastGuiW(){return last_gui_w;}
	int GetLastGuiH(){return last_gui_h;}

	void SetLock(bool lock);
	bool GetLock(){return lock; }

	Editor::PickMode GetPickMode(){return pickMode; }
	void SetPickMode(Editor::PickMode mode);

	Editor::PickMoveMode GetMoveMode(){return moveMode;}
	void SetMoveMode(Editor::PickMoveMode moveMode);

    Editor::PathModeType GetPathMode() {return pathMode;}
    void SetPathMode(Editor::PathModeType mode);
	void BeginSetPathMode(Editor::PathModeType mode,MovePath *editPath); // for complex edit
    void EndSetPathMode();

    MovePath * GetEditablePath() {return editablePath;}
    MovePath * GetOldEditablePath() {return oldEditablePath;}

	void PushMode();
	void PopMode();

	bool IsPathEditing(){return isPathEditing;}

	void NotifyMouseCollisionEventDown(MouseCollisionEvent &ev);
	void NotifyMouseCollisionEventUp(MouseCollisionEvent &ev);
	void NotifyMouseCollisionEventDrag(MouseCollisionEvent &ev);
	void NotifyMouseCollisionEventEnterOver(MouseCollisionEvent &ev);
	void NotifyMouseCollisionEventExitOver(MouseCollisionEvent &ev);

	bool MousePressed(View *view,int w,int h,QMouseEvent *e);
	bool MouseMove(View *view,int w,int h,QMouseEvent *e);
	bool MouseRelease(QMouseEvent *e);

	bool KeyPressEvent(QKeyEvent *e);

	void Clear();
	void NotifyDelete(SceneObject *obj);

	void OnTimerTick(int timeMilliseconds);

	// mesh

	Ogre::Vector3 GetLocalCoordinate(Ogre::Vector3 &worldDir);

	void Draw(bool selectMode);

	QString GetDesc();

signals:
	 void OnLockChanged();
	// void OnModeChanged(int modeLevel);
     void OnPathModeChanged(Editor::PathModeType mode);
	 void OnPickModeChanged(Editor::PickMode mode);

     void NewCursorMode(Editor::CursorMode mode);
public slots:
     void OnSelectionChanged();
};

#include "Anima_nsend.h"
