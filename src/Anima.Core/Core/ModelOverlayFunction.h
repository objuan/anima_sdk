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
class ModelOverlayFunction;
class OverlayClip;
class OverlayGroupClip;
class GfxClip;
class QRandom;
class ModelIABuilderClip;
class ModelIAFrame;
class MovePathInfoClip;
//

class ModelIAFrame_Overlay 
{
public:
	OverlayClip *overClip;
	float time;
	float fade;
	ModelIAFrame_Overlay():overClip(NULL),time(0),fade(0){};
};


class ANIMA_CORE_API ModelOverlayFunction_AnimEntry 
	:public BObject
{
public:
 
	ModelOverlayFunction *overlayFun;
	OverlayClip *overClip;
	//MovePathInfoClip *firstInfoClip;
	float firstGlobalTime;
	float lastGlobalTime;
	
	ModelOverlayFunction_AnimEntry(ModelOverlayFunction *speedFun,OverlayClip *overClip);

	void Set(OverlayClip *overClip);
	void CopyFrom(ModelOverlayFunction_AnimEntry &fromEntry);
	void Build();
};


class ANIMA_CORE_API ModelOverlayFunction: public BObject {// public IBuildElement, public BObject {
friend class ContextIO;
public:
	
	enum ModelOverlayFunctionEditMode
	{
		ModelOverlayFunctionEditMode_Constant,
		ModelOverlayFunctionEditMode_User
	};

private:

	int seed;
	//QRandom *rnd;
	float fps;
	Animation *anim;
	ModelInstance *modelInstance;

	bool haveOverlay;
	ModelIAFrame_Overlay currentOverlay;
	QList<ModelIAFrame_Overlay> overlayFrames;
	QList<ModelOverlayFunction_AnimEntry *> entryList;
	bool cleared;
	OverlayGroupClip *clipGroup;
	QString serialFromLoad;
	
private:

	void BuildOld();
	void EnsureInit();

	void AddOverClip(int frame,OverlayClip *overClip,float timeDelta,float globalTime);

public:
	ModelOverlayFunction(ModelInstance *modelInstance);
	~ModelOverlayFunction();

	ModelInstance *GetModelInstance(){return modelInstance;}
	
	//void SetEditMode( ModelOverlayFunctionEditMode mode);
	//ModelOverlayFunctionEditMode GetEditMode(){return editMode;}
	OverlayGroupClip *GetClipGroup(){return  clipGroup;}
	QString GetClipGroupID();

	//void GetEntryList(AnimationPart *part,QList<ModelOverlayFunction_AnimEntry> &list);
	//OverlayClip *GetOverlay(AnimClipModel *model);

	ModelOverlayFunction_AnimEntry *GetEntry(int idx);
	int GetEntryCount(){return entryList.count();}
	//int FindEntry(AnimClipModel *animClip);

	void Build();
	void Clear();
	void Dump();

//	void Set(AnimClipModel *modelClip,OverlayClip *groupClip);

	void _SetAll(const QString &gropID);
	void SetAll(OverlayGroupClip *group);
	void Randomize();

	bool HaveOverlay();

	bool Evaluate(double animationGlobalTime, OverlayClip **clip,double &innerTime,double &fadeFactor);
	
	QString Serialize();
	void ResumeFromSerial(QString &ser);

	void BuildFromStorage();
	//virtual void InvalidateBuild();

};
#include "Anima_nsend.h"