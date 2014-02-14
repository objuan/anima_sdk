#pragma once

#include "SceneObject.h"
#include "BuildDispatcher.h"
#include "Anima_nsbegin.h"

class Animation;
class AnimationType;
class AnimationPart;
class AnimClipModel;
class ModelInstance;
class MovePath;
class PathOffsetFunction;

struct PathOffsetFunction_Key
{
	double inSpace;
	double globalSpace;
	double value;
	//double time;
	double vm;
	PathOffsetFunction_Key(double globalSpace,double value)
	{
		this->globalSpace=globalSpace;
		this->value=value;
		inSpace=-1;
	//	time=0;
		vm=0;
	}
};
class PathOffsetFunction_PathEntry :  public BObject
{
public:
	MovePath *path;

	AList<PathOffsetFunction_Key> keyList;
	bool isConstant;

	PathOffsetFunction_PathEntry(MovePath *path);

	void AddKey(double space,double value);
	void ClearKeys();
	void SetCostant(double val);
	void CopyFrom(PathOffsetFunction_PathEntry &fromEntry);
	void Build();
	double GetPathSpace();
	double GetValue(int startKey,double localSPace);
	double GetLocalSpaceFromGlobal(int startKey,double globalSpace);
	double GetKeyFreeSpace();
};

class ANIMA_CORE_API PathOffsetFunction_Mark: public BulkSceneObject
{
public:
	PathOffsetFunction *pathFun;
	PathOffsetFunction_PathEntry *partEntry;
	double space;

	double userVal; // multiplier 

	bool isBound;
	bool userEnabled;
	
	PathOffsetFunction_Mark(PathOffsetFunction *pathFun,PathOffsetFunction_PathEntry *partEntry,bool leftBound) : pathFun(pathFun)
	{
		this->partEntry=partEntry;
		space=0;
		isBound=true;
		userEnabled=false;
		if (leftBound) 	space=0;
		else space = 1;
		userVal= 0;
	}

	PathOffsetFunction_Mark(PathOffsetFunction *pathFun,PathOffsetFunction_PathEntry *partEntry,double space,double val):pathFun(pathFun)
	{
		this->partEntry=partEntry;
		this->space=space;
		this->userVal=val;
		isBound=false;
		userEnabled=true;
	//	isAuto=false;
	}	
	~PathOffsetFunction_Mark()
	{
		int y=0;
	}


	PathOffsetFunction *GetPathFun(){return pathFun;}

	virtual AObject::Type GetType(){return AObject::PathOffsetPointType;}
	virtual QString ClassName(){return "PathOffsetPoint";}
	virtual QString GetDesc(){return "Offset Mark";}

	virtual SceneObject *GetParent();
	virtual DAG GetDAG();
	int GetIdx();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	bool IsOffsetAuto();
	bool IsOffsetDisabled();
	double GetRowValue();
	void SetVal(double value);
	double GetValue();
	double GetPercSpace(){return space;}
	double GetSpace();
	double GetPathSpace();
	double GetLinkSpace(ModelInstance *mi);
	bool IsUserEnabled(){return userEnabled; }
	bool IsEnabled(int passNumber=0);
	bool EvalAuto();
};

class ANIMA_CORE_API PathOffsetFunction: public IBuildElement, public BObject {
friend class ContextIO;
friend class Animation;
friend class ModelInstance;
private:

	ModelInstance *modelInstance;
	AList<PathOffsetFunction_PathEntry> *entryList;
	AList<PathOffsetFunction_Mark *> markList;
	double distanceOffset;

	double totalSpace;

private:
	void _BuildFromMark();
	void __BuildFromMark(int pass);

public:
	PathOffsetFunction(ModelInstance *modelInstance);
	~PathOffsetFunction();

	ModelInstance *GetModelInstance(){return modelInstance;}
	AList<PathOffsetFunction_Mark *> &GetMarkList(){ return markList;}
	void GetMarkList_GUI(MovePath *path,AList<PathOffsetFunction_Mark *> &list);
	void GetMarkList_Enabled(MovePath *path,AList<PathOffsetFunction_Mark *> &list);
	int GetMarkPartIndex(PathOffsetFunction_Mark &mark);
	int GetMarkIndex(PathOffsetFunction_Mark &mark);
	PathOffsetFunction_PathEntry *GetEntry(int idx);
	int GetEntryCount(){return entryList->count();}
	int FindEntry(MovePath *path);
	double GetDistanceOffset(){return distanceOffset;}

	void Build();
//	void OnAnimPathsChanged();

	void Clear();
	bool HaveOffset();

	void SetDistanceOffset(double distanceOffset);
	void DeleteMark(int generalIdx,bool rebuild);
	int AddMark(MovePath *path,double spacePerc,double val,bool rebuild=true);
	void MoveMark(int markIdx,double val);
	void MoveMark(int markIdx,double spacePerc,double val);
	void SetBound(double distanceOffset,double dir_offset90);

	double GetFromGlobal(MovePath *path,double globalPathSpace);
	Ogre::Vector2 Get(MovePath *path,double pathSpace);

	//void Copy(PathOffsetFunction *fun);
	void Dump();
	//void Update();

	QString Serialize();
	void ResumeFromSerial(QString &ser);

	virtual void InvalidateBuild(int type=0);

};
#include "Anima_nsend.h"