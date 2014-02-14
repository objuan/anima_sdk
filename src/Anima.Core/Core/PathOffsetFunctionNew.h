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

class PathOffsetFunctionNew_PathEntry :  public BObject
{
public:
	MovePath *path;

	PathOffsetFunctionNew_PathEntry(MovePath *path);
};

class ANIMA_CORE_API PathOffsetFunctionNew: public IBuildElement, public BObject {
friend class ContextIO;
friend class Animation;
friend class ModelInstance;
private:

	ModelInstance *modelInstance;
	AList<PathOffsetFunctionNew_PathEntry> *entryList;
	
public:
	PathOffsetFunctionNew(ModelInstance *modelInstance);
	~PathOffsetFunctionNew();

	ModelInstance *GetModelInstance(){return modelInstance;}
	
	PathOffsetFunctionNew_PathEntry *GetEntry(int idx);
	int GetEntryCount(){return entryList->count();}
	int FindEntry(MovePath *path);
	
	void Build();
	void Clear();
	bool HaveOffset();

	void Dump();

	QString Serialize();
	void ResumeFromSerial(QString &ser);

	virtual void InvalidateBuild(int type=0);

};
#include "Anima_nsend.h"