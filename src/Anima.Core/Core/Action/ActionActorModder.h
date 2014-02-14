#pragma once

#include "Core/Action.h"
#include "Anima_nsbegin.h"

class ANIMA_CORE_API ActionActorModder : public Action
{
private:


public:

	ActionActorModder();
	virtual ~ActionActorModder();

	virtual bool CanTarget(SceneObject *obj);
	virtual void Load(QDomElement e);

        virtual ActionType GetType(){return Action::ActionActorModder;}
	virtual void *Execute(Command *cmd,SceneObject *obj,int pos);
	virtual void Undo(Command *cmd,SceneObject *obj,int pos,void *tag);
};

#include "Anima_nsend.h"
