#pragma once

#include "Command.h"
#include "../DAG.h"
#include "Anima_nsbegin.h"
class MeshStatic;
class MovePath;
class AnimClipCollection;
class LibClip;

// load , save
class IOCommand : public Command
{
private:
	bool loaded;
public:
	virtual bool HaveUndo(){return false;}
	virtual VoidP Exec();
	virtual void Undo();
};

class PropertyCommand : public Command
{
public:
	virtual bool HaveUndo(){return false;}
	virtual VoidP Exec();
};

class SelectCommand : public Command
{
public:
	virtual bool HaveLog(){return false;}
	virtual bool HaveUndo(){return false;}
	virtual VoidP Exec();
};

// import_mesh [path]
class MeshCommand : public Command
{
private:
	MeshStatic *mesh;
public:
	virtual VoidP Exec();
	virtual void Undo();
};

class DeleteCommand : public Command
{
private:
	struct DeleteCommandEntry
	{
		int idx;
		DAG parentDAG;
		QDomDocument serDoc;
		SceneObject *base_obj;
		Ogre::Vector3 pos;
		AObject::Type objType;
	};

	QList<DeleteCommandEntry> opList;

	QList<SceneObject *> objList;

public:
	virtual void Dispose();
	virtual VoidP Exec();
	virtual void Undo();
};

class RenameCommand : public Command
{
private:
	SceneObject *obj;
	QString oldName;
public:
	virtual VoidP Exec();
	virtual void Undo();
};


class CopyCommand : public Command
{
public:
	virtual bool HaveUndo(){return false;}
	virtual VoidP Exec();
	//virtual void Undo();
};

class PasteCommand : public Command
{
private:
	SceneObject *obj;
	SceneObject *copyObj;
public:
	virtual VoidP Exec();
	virtual void Undo();
};

class CloneCommand : public Command
{
private:
	SceneObject *newObj;
public:
	virtual VoidP Exec();
	virtual void Undo();
};
class PropertyChangeCommand : public Command
{
private:
	//SceneObject *obj;
	DAG objDAG;
	QDomDocument serDoc;
	QString propName;

public:
	virtual VoidP Exec();
	virtual void Undo();
};
#include "Anima_nsend.h"


