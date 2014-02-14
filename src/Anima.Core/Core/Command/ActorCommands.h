#pragma once

#include "Command.h"
#include "Anima_nsbegin.h"

class IMesh;
class ModelInstance;
class MovePath;
//class PathOffsetFunction;
class PathManipolator;

class ActorCommand : public Command
{
private:
	Animation *anim;
	ModelInstance *modelInstance;
	ModelInstance *new_modelInstance;

public:
	virtual VoidP Exec();
	virtual void Undo();
};


class ModelInstanceCommand : public Command
{
	class ModelInstanceCommandEvent : public CommandEvent
	{
	public:
		ModelInstanceCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseUp(MouseCollisionEvent &coll);
		virtual void OnMouseDrag(MouseCollisionEvent &coll);
	};

private:
	PathManipolator *pathManipolator;

	int idx;
	int markIdx;
	MovePath *path;
	Ogre::Vector3 startPos;
	Ogre::Vector3 startOffset;
	Ogre::Vector3 directionXZ;
	Ogre::Vector3 directionXZ90;
	QString dir;
	ModelInstance *model;
	double spacePerc;
	//PathOffsetFunction *offFun;
	QString speedSerial;
	//QString offsetSerial;
	//double offsetDistance;

public:
	ModelInstanceCommand();
	virtual ~ModelInstanceCommand();

	virtual VoidP Exec();
	virtual void Undo();
};

#include "Anima_nsend.h"
