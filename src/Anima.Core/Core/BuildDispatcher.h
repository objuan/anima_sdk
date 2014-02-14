#pragma once

#include "Anima_nsbegin.h"

class MovePath;
class AnimClipModel;
class ModelInstance;
class Animation;

enum NearChanged
{
	NearChanged_PREV_PATH = 1,
	NearChanged_NEXT_PATH = 2,
	NearChanged_PREV_CLIP = 4,
	NearChanged_NEXT_CLIP = 8,
};


class ANIMA_CORE_API IMovePath_BuildElement 
{
friend class MovePath_BuildDispatcher;

private:
	bool isBuilding;
	bool invalid;
	int depChanged;
	int childDepChanged;
	bool isBuilded;
	bool isPreBuilded;
	bool isSubBuilded;
	bool firstTime;

	int nearChanged;
	MovePath *prevPart;
	MovePath *nextPart;
//	Ogre::Vector3 old_prevNormal;
//	Ogre::Vector3 old_nextNormal;

protected:
	bool HaveDepChanged(int type=0);
public:

	IMovePath_BuildElement();
	virtual void InvalidateBuild(int type=0);

	bool EnsureValidBuild();
	bool IsBuilded(){return isBuilded;}
	void ForceValid();
	void ChildDepChanged(){childDepChanged=true;}

	void BuildPending(int depChanged);
	bool HavePending(int depChanged);
	bool IsSubBuilding();
	bool IsBuilding();

	virtual void Build() {};
	virtual void PostBuild() {};
};

class ANIMA_CORE_API MovePath_BuildDispatcher : public BObject
{
friend class IMovePath_BuildElement;
friend class BuildDispatcher;

//	QMap<AnimClipModel *,bool> invalidMap;
private:
	bool isBuilding;
	bool path_invalid;
	bool afterLoad;

	bool build_invalid;
	bool last_path_invalid;
public:
	MovePath_BuildDispatcher();
	
	void BuildPending(bool afterLoad);
	void BuildPending_ModelPath(Animation *anim);

	bool IsPathInvalid(){return path_invalid;}
	void InvalidatePath(){path_invalid=true;}
};

class ANIMA_CORE_API BuildDispatcher: public BObject
{
private:
	bool model_invalid;
	MovePath_BuildDispatcher *pathDispatcher;
public:
	BuildDispatcher();
	~BuildDispatcher();

	void BuildPending(bool afterLoad=false);

	bool IsModelInvalid(){return model_invalid;}
	void InvalidateModel(){model_invalid=true;}
	void InvalidatePath(){pathDispatcher->InvalidatePath();}
};

#include "Anima_nsend.h"