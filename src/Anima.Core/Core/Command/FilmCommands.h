#pragma once

#include "Command.h"

#include "Anima_nsbegin.h"
class FilmLayer;
class Animation;

// add_layer [name]
// add_anim [name]
class FilmCommand : public Command
{
protected:
	//class FilmCommandEvent : public CommandEvent
	//{
	//public:
	//	FilmCommandEvent(Command *cmd):CommandEvent(cmd){}
	//	virtual void OnMouseDown(MouseCollisionEvent &ev);
	//};

private:
	FilmLayer *filmLayer;
	Animation *animation;
public:

	FilmCommand();

	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
};
#include "Anima_nsend.h"