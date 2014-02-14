#pragma once

#include "Command.h"
#include "Anima_nsbegin.h"

class SceneObject;
class Action;

class ActionCommand : public Command
{
	class ActionCommandEvent : public CommandEvent
	{
	public:
		ActionCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseMove(MouseCollisionEvent &ev);
		virtual void OnMouseDown(MouseCollisionEvent &ev);
		virtual void OnMouseUp(MouseCollisionEvent &ev);
		virtual void OnMouseDrag(MouseCollisionEvent &ev);
	};

private:
	SceneObject *obj;
	Action *action;
	void *tag;
	int idx;

public:
	ActionCommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
	virtual void Abort();

	virtual bool IsPathCommand();
};
#include "Anima_nsend.h"
