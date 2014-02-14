#pragma once

#include "Anima_nsbegin.h"
class FilmLayer;
class Animation;
class Model;
class AnimationPartClipSpline;
class Clip;
class IMesh;
class SceneObject;
class AnimationPart;

class ANIMA_CORE_API EventManager: public QObject 
{
	Q_OBJECT

private:


public:

	static EventManager Singleton;

public:
	EventManager(void);
	virtual ~EventManager(void);

	void Init();
	void InitUI();

	// GUI EDIT
	void OnMouseDown(MouseCollisionEvent &ev);
	void OnMouseDrag(MouseCollisionEvent &ev);
	void OnMouseMove(MouseCollisionEvent &ev);
	void OnMouseUp(MouseCollisionEvent &ev);
	
private slots:
	//void OnChangedAnimationPart(AnimationPart *part);
	//void OnChangedAnimation(Animation *anim);
	void OnEditModeChanged(int lev);
	void OnPropertyWidgetClosed(int id,bool ok,SceneObject *obj);

//	 void CurrentFrameChanged();
};
#include "Anima_nsend.h"