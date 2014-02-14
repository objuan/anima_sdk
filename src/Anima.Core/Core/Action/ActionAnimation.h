#pragma once

#include "Core/Action.h"
#include "../DAG.h"
#include "Anima_nsbegin.h"

class FilmLayer;
class Animation;

class ANIMA_CORE_API ActionAnimation : public Action
{
protected:

	Animation *animation;
	
public:

	ActionAnimation();
	virtual ~ActionAnimation();

	virtual void Load(QDomElement e);

	virtual void *Execute(SceneObject *obj,int pos);
	virtual void Undo(SceneObject *obj,void *tag);
};

class ANIMA_CORE_API ActionAnimation_AddPart : public ActionAnimation
{
private:
	AObject::Type partType;

	int widgetID;
	AnimationPart *animationPart;
	DAG ausDAG;

public:
	ActionAnimation_AddPart();
	virtual ~ActionAnimation_AddPart();

	static AObject::Type GetTypeFromName(const QString &name);

        virtual ActionType GetType(){return Action::ActionAnimation_AddPart;}
	virtual void Load(QDomElement e);
	virtual void *Execute(Command *cmd,SceneObject *obj,int pos);
	virtual void Undo(Command *cmd,SceneObject *obj,int pos,void *tag);
	virtual void Abort(Command *cmd);
	virtual void Exit(Command *cmd);
	virtual void OnMouseDown(Command *cmd,MouseCollisionEvent &ev);
	virtual void OnMouseMove(Command *cmd,MouseCollisionEvent &ev);
	virtual bool IsPathCommand(){return true;}
};
#include "Anima_nsend.h"
