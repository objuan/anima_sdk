#pragma once

#include "Core/Action.h"
#include "Anima_nsbegin.h"

class LibClip;

class ANIMA_CORE_API ActionClip : public Action
{
private:

	LibClip *clip;
	LibClip *old_clip;
public:

	ActionClip();
	virtual ~ActionClip();

	LibClip *GetClip(){return clip;}

	virtual bool CanTarget(SceneObject *obj);
	virtual void Load(QDomElement e);

        virtual ActionType GetType(){return Action::ActionClip;}
	virtual void *Execute(Command *cmd,SceneObject *obj,int pos);
	virtual void Undo(Command *cmd,SceneObject *obj,int pos,void *tag);
};

#include "Anima_nsend.h"
