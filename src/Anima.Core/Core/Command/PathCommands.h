#pragma once

#include "Command.h"
#include "Core/Dag.h"
#include "Anima_nsbegin.h"

class IMesh;
class MovePath;
class MovePathStorage;
class PathManipolator;

class InvertCommand : public Command
{

private:
	DAG objDAG;

public:
	/*InvertCommand();
	~InvertCommand();*/
	virtual VoidP Exec();
	virtual void Undo();
};

class PointCommand : public Command
{
	class PointCommandEvent : public CommandEvent
	{
	public:
		PointCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseUp(MouseCollisionEvent &coll);
		virtual void OnMouseDrag(MouseCollisionEvent &coll);
	};

private:
	int idx;
	Ogre::Vector3 pos;
	Ogre::Vector3 lastGoodPos;

	//MovePath *copyPath;
	//QDomDocument serDoc;
	DAG objDAG;
	MovePath *orig_path;

	PathManipolator *pathManipolator;

	AnimationPart *animPart;
	AnimationPart *srcAnimPart;
public:
	PointCommand();
	~PointCommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual bool IsPathCommand(){return true;}
};

class PathTrxCommand : public Command
{
	class PathTrxCommandEvent : public CommandEvent
	{
	public:
		PathTrxCommandEvent(Command *cmd):CommandEvent(cmd){}
		virtual void OnMouseUp(MouseCollisionEvent &coll);
		virtual void OnMouseDrag(MouseCollisionEvent &coll);
	};
	MovePath *path;
//	MovePath *copyPath;
	DAG objDAG;
	QDomDocument serDoc;

	//MovePathStorage *storage;
    Ogre::Vector3 prevDragPoint;
	QPoint prevMousePoint;
    float prevDragAngle;
	int prev_mx;

	PathManipolator *pathManipolator;

public:
	PathTrxCommand();
	~PathTrxCommand();
	virtual VoidP Exec();
	virtual void Undo();
	virtual bool IsPathCommand(){return true;}
};

#include "Anima_nsend.h"