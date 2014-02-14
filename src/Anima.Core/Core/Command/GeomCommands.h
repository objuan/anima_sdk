#pragma once

#include "Command.h"
#include "Anima_nsbegin.h"

class SceneObject;
class Action;
class MovePath;
class SceneObjectMovable;
class ISceneObjectMovable;

class MoveCommand : public Command
{
	class MoveCommandEvent : public CommandEvent
	{
	public:
		MoveCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseDown(MouseCollisionEvent &ev);
		virtual void OnMouseUp(MouseCollisionEvent &ev);
		virtual void OnMouseDrag(MouseCollisionEvent &ev);
	};

private:
	SceneObject *obj;
	QString mode;

	int idx;
	int markIdx;
	MovePath *path;
	Ogre::Vector3 startPos;
	Ogre::Vector3 mousePos;
	Ogre::Vector3 startOffset;
	Ogre::Vector3 directionXZ;
	Ogre::Vector3 directionXZ90;

public:
	MoveCommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
	virtual void Abort();
};

class RotateCommand : public Command
{
private:
	class RotateCommandEvent : public CommandEvent
	{
	public:
		RotateCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseDown(MouseCollisionEvent &ev);
		virtual void OnMouseUp(MouseCollisionEvent &ev);
		virtual void OnMouseDrag(MouseCollisionEvent &ev);
	};

	QString mode;
	SceneObject *obj;
	Ogre::Vector3 mousePos;

public:
	RotateCommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
	virtual void Abort();
};

class MoveUICommand : public Command
{
	class MoveUICommandEvent : public CommandEvent
	{
	public:
		MoveUICommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseDown(MouseCollisionEvent &ev);
		virtual void OnMouseUp(MouseCollisionEvent &ev);
		virtual void OnMouseDrag(MouseCollisionEvent &ev);
	};

private:
	SceneObject *_obj;
	ISceneObjectMovable *obj;
	int mode;
	Ogre::Vector3 position;
	float rot_xz;
	float virtualY;

	// snapping
	bool doSnapping;
	Ogre::Vector3 origin;
	Ogre::AxisAlignedBox box;
	Ogre::Matrix4  trx;
	//Ogre::Vector3 lastPosOK;

public:
	MoveUICommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual void Exit();
	virtual void Abort();
	virtual bool IsPathCommand(){return true;}
	void InvalidateBuild();
};

#include "Anima_nsend.h"
