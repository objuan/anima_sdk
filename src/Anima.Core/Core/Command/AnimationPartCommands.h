#pragma once

#include "Command.h"

#include "Anima_nsbegin.h"
class FilmLayer;
class Animation;

// add_layer [name]
// add_anim [name]
class AnimationPartAddCommand : public Command
{
protected:
	class AnimationPartAddCommandEvent : public CommandEvent
	{
	public:
		AnimationPartAddCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseDown(MouseCollisionEvent &ev);
		virtual void OnMouseMove(MouseCollisionEvent &coll);
	};

private:
	FilmLayer *filmLayer;
	Animation *animation;
	AnimationPart *animationPart;
public:

	AnimationPartAddCommand();

	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
};
#include "Anima_nsend.h"