#pragma once

#include "Command.h"
#include "Core/Dag.h"

#include "Anima_nsbegin.h"
class AnimationPart;
class Modder;

class AnimPartMovePosCommand : public Command
{
private:
	//AnimationPart *animPart;
	DAG objDag;
	QDomDocument serDoc;
	//int oldIdx;

public:
	virtual VoidP Exec();
	virtual void Undo();
};

////class MergeCommand : public Command
////{
////private:
////	SceneObject *obj;
////	int offset;
////	QList<double> sizeList;
////public:
////	virtual VoidP Exec();
////	virtual void Undo();
////};
////
////class SplitCommand : public Command
////{
////private:
////	SceneObject *obj;
////	QList<double> sizeList;
////public:
////	virtual VoidP Exec();
////	virtual void Undo();
////};

class ModderCommand : public Command
{
private:
	QDomDocument serDoc;
	QDomDocument outSerDoc;
	DAG objDag;
public:
	virtual VoidP Exec();
	virtual void Undo();
};
#include "Anima_nsend.h"