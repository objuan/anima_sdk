#pragma once

//#include "ModelIA.h"
#include "ModelIAActorDecision.h"
#include "Physic/BodyModder.h"
#include "Gfx/GfxClipAnim.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;
class ModelIAThread;
class Animation;
class GfxClip;
class ModelClip;
class GfxModel;
class ModelIAFramePos;
class MovePathSlice;
class ModelIAActorDecision;
class LibClip;
class MovePathInfo;
class ModelIABuilderPath;
class ModelIAActor;
class ModelIABuilder;
class ModelOverlayFunction_AnimEntry;
class LibClip;

class ModelIABuilderStackFrame: public BObject
{
public:
	int frame;
	float *dValList;
	int *iValList;
	ModelIABuilderStackFrame(int frame,int ds,int is);
	virtual ~ModelIABuilderStackFrame();
};

class ModelIABuilderClip: public BObject
{
public:
	AnimClipModel *animClipModel;
	ModelClip *clip;
	ModelIABuilderClip();
	ModelIABuilderClip(AnimClipModel *animClipModel,ModelClip *clip):animClipModel(animClipModel),clip(clip){}
};

struct MovePathStepPosition
{
	float scanPathSpace;
	float renderPathSpace;
	float renderPathHeight;
	float modelPathSpace;
	int startPathIdx;
	void Clear(){scanPathSpace=0;renderPathSpace=0;renderPathHeight=0;modelPathSpace=0;startPathIdx=0;}
};
class ModelIAPathFrameBuild: public BObject
{
public:
	bool assigned;
	//float Speed;
	int Frame;
	ModelIAFrame *ModelFrame;
	// idx

	float pathClipSpace; // distanza sul path del clip incluso
	float totalModelSpace;// distanza toale del clip incluso
	float SpeedModificator;
	QList<ModelIAActor*> collide;

	// STACK

	// PATH
	unsigned short path_currentPathIdx;
	//unsigned short path_loopCount;
	float path_lastPathSpace;
	float path_lastModelSpace;
	float path_speedModificator;

	// PATHINFO
	/*float pathinfo_renderPathSpace;
	float pathinfo_modelPathSpace;
	unsigned short pathinfo_startPathIdx;
	float pathinfo_scanPathSpace;
	float pathinfo_renderPathHeight;*/
	MovePathStepPosition pathinfo_pathPos;
	float pathinfo_currentPathTime;
	float pathinfo_currentAnimClipModelTime;
	unsigned short pathinfo_currentClipIdx;
	//SLICE
	unsigned short slice_currentClipIdx;
	//CLIP INFO
	float clip_currentClipLocalTime;
	float clip_currentClipTotalTime;
	float clip_currentTime;
	unsigned short clip_currentClipIdx;
	float clip_currentClipSpace;
	unsigned short clip_currentClipCount;

	ModelIAPathFrameBuild():assigned(false){}
};


class ANIMA_CORE_API MovePathInfoClipGfx: public BObject
{
public:
	BodyModderPtr bodyModder;
	ModelClip *modelClip;
	AnimClipModel *animClipModel;
	Ogre::Vector2 startOffset;
	GfxClip *clipGfx;
	int subClipIdx;
	int addType;
	QString clipID;
	int clipCount;
	float clipSpace;
	float clipHeight;
	float clipTime;
	float cutClipTime;
	float cutClipSpace;
	float spaceFactor;
	float modder_cutClipTime;

	float totalTime;
	float totalSpace;
	inline float GetTotalHeight(){return clipHeight * clipCount;};

	MovePathInfoClipGfx *prevClip;
	MovePathInfoClipGfx *nextClip;

	GfxClipAnimPtr gfxClipAnim;
public:

	MovePathInfoClipGfx(){prevClip=nextClip=NULL;modder_cutClipTime=-1;}
	MovePathInfoClipGfx(AnimClipModel * animClipModel,ModelClip *clip,BodyModderPtr _bodyModder,int subClipIdx,float clipCount,float cutClipTime,bool doResize);
	MovePathInfoClipGfx(AnimClipModel * animClipModel,ModelClip *clip,float space,float height,float clipCount);
	MovePathInfoClipGfx *Clone();

	//MovePathInfoClipGfx();
	virtual ~MovePathInfoClipGfx();

	QString ID();
	void ClearClipGfx();
	GfxClip *GetClipGfx();
	GfxClipAnim *GetClipAnimGfx(bool ensureInit=true);
	void EnsureInit();
	QString ToString();
};

class MovePathInfoClip: public BObject
{
public:
	QList<MovePathInfoClipGfx *> assignedClip;
	MovePathSlice *slice;
	float forceClipTime; // params
//	ModelOverlayFunction_AnimEntry *overlayFun;
//	float overlayStartClipTime; 

	float currentTime;
	float currentClipLocalTime;
	float currentClipTotalTime;
	float currentClipSpace;
	int currentClipIdx;
	int currentClipCount;
	int firstFrame;

	float totalTime;
	float totalSpace;

	ModelClip *modelClip;
	AnimClipModel *animClipModel;
	MovePath *path;
	ModelInstance *mi;
	MovePathInfoClip(MovePathSlice *parentSlice,ModelClip *modelClip,AnimClipModel *animClipModel,ModelInstance *mi,MovePath *path);
	virtual ~MovePathInfoClip();
	MovePathInfoClip *Clone(MovePathSlice *slice);

	MovePathInfoClipGfx &AddEntry(MovePathInfoClipGfx *gfx);
	void SetRenderPath(MovePath *path,MovePathStepPosition &pathPos,MovePathInfoClipGfx *gfx,MovePathInfoClipGfx *nextGfx);

	void Init();
	void Clear();
	void OnBuildEnd();
	void PushStack(ModelIAPathFrameBuild *frame);
	void Resume(ModelIAPathFrameBuild *frame);
	bool Step(float &elapsed_time,MovePathStepPosition &pathPos,float &animClipModelTime,ModelIAPathFrameBuild *frame);

};


class ANIMA_CORE_API MovePathSlice: public BObject
{
public:
	bool toDispose;
	float _currentClipIdx;
	ModelIAActorDecisionPtr decision; 
	MovePathSlice *prevSlice;
	MovePathInfo *pathInfo;
	//ModelIABuilderStack stack;

	bool isFirstSlice;
	int firstFrame;
	float firstTotalSpace;
	bool isFixed;
	QList<MovePathInfoClip *> clipList;

	float totalSpace;
	float totalHeight;
	float totalTime;
	bool valid;
	ModelInstance *mi;

	MovePathSlice(MovePathInfo *pathInfo);
	virtual ~MovePathSlice();
	void Init(ModelInstance *mi,MovePathSlice *prevSlice,bool fixed,float &space);
	void Clear();
	MovePathSlice *Clone();

	AnimClipModel *GetAnimClipModel();

	ModelClip *GetWorkModelClip(ModelClip *workModelClip,AnimClipModel *animClipModel,int *userClipMult);
	float TestClipSize(AnimClipModel *animClipModel,ModelClip *modelClip,ModelClip *prevModelCLip,float spaceToFit);

	void Init();
	void OnBuildEnd();
	void PushStack(ModelIAPathFrameBuild *frame);
	void Resume(ModelIAPathFrameBuild *frame);
	bool Step(float &elapsed_time,MovePathStepPosition &pathPos,float &animClipModelTime,ModelIAPathFrameBuild *frame,bool recordInfo);
	void ProjectToAnimClipModel();
	void RebuldTotal();

	QString Dump();
};

class MovePathInfo_Assign
{
public:
	QList<MovePathSlice *> sliceList;
	float startTime;
	float totalSpace;
	float totalTime;
	// escalator
	float startPathClipTime0;
	float startPathClipTime1;
	MovePathInfo_Assign(){startPathClipTime0=startPathClipTime1=0;}
};

class ANIMA_CORE_API MovePathInfo: public BObject
{
public:
	ModelIABuilderPath *builderPath;
	GfxModel *gfxModel;
	MovePath *path;
	ModelInstance *mi;

	MovePathInfo_Assign *assignInfo;
	QList<MovePathSlice *> sliceList;
	
	//bool changed;
	// trx
	Ogre::Matrix4 pos_trx;
	Ogre::Matrix4 dir_trx;
	float xzAngle_trx;

	float totalSpace;
	float totalTime;

	int currentClipIdx;
	float _currentAnimClipModelTime;
	MovePathStepPosition pathPos;
	float _currentPathTime;

	float collisionStartTime;
	float stairFitScale;
	float startSpace;
	float startTime;
	// escalator
	float startPathClipTime0;
	float startPathClipTime1;
	bool assignScanMode;
	bool isLoopPart;

	bool resume_invalid;

private:
	void PushStack(ModelIAPathFrameBuild *frame);
	void InitializeTrx();

public:

	MovePathInfo(ModelIABuilderPath *builderPath,MovePath *path,ModelInstance *mi);
	virtual ~MovePathInfo();

	// API
	inline float GetTotalTime(bool valid){ return valid ? totalTime : assignInfo->totalTime ;}
	inline float GetTotalSpace(bool valid){ return valid ? totalSpace : assignInfo->totalSpace ;}
	inline float GetStartTime(bool valid){ return valid ? startTime : assignInfo->startTime ;}
	inline QList<MovePathSlice *> &GetSliceList(bool valid){ return valid ? sliceList : assignInfo->sliceList ;}

	void InvalidateResume(){resume_invalid=true;}
	inline bool IsResumeInvalid(){return resume_invalid;}
	// 
	bool SetToSpace(float startSpace,bool assignMode,bool isFirst);
	void Startup(bool isFirst);
	void Reset();
	void Resume(ModelIAPathFrameBuild *frame);
	bool PopFrame(float &elapsed_time,ModelIAPathFrameBuild *frame,bool recordInfo);

	MovePathSlice *GetPrevSlice(MovePathSlice *slice);
	MovePathInfo *GetPrevPath();

	void RebuldTotal();
	void OnBuildEnd();

	bool CanInsertClip(LibClip *clip,float clipTime,MovePathSlice *inSlice,float insertSpace);
	MovePathSlice *InsertClip(LibClip *clip,float clipTime,MovePathSlice *inSlice,float insertSpace,ModelIAActorDecision* decision);
	MovePathSlice *InsertClip_old(LibClip *clip,float clipTime,MovePathSlice *inSlice,float insertSpace,ModelIAActorDecision* decision);
	MovePathSlice *ChangeClip(LibClip *clip,float clipTime,MovePathSlice *inSlice,ModelIAActorDecision* decision);

	void Load(QDomElement &e);
	MovePathInfoClipGfx *New_MovePathInfoClipGfx(AnimClipModel *animClipModel,ModelClip *modelClip,QDomElement &ele);
	void Save(QDomDocument &doc,QDomElement &ele);
};


// ====================================================

class ModelIABuilderPathStorage: public BObject
{
public:
	QDomDocument storeInfo;
};

class ANIMA_CORE_API ModelIABuilderPath : public BObject
{
private:
	int maxLoopCount;
	int maxFrameCount;
public:
	ModelIAActor *actor;
	ModelInstance *mi;
	Animation *anim;	
	QList<MovePathInfo *> pathList; // cresce con il tempo

	// startup
	int _startAnimationPartIdx;
	float _startAnimationPartSpace; 
	int _currentAnimationPartIdx;

	int currentPathIdx;
	float lastPathSpace; // ultimo spazio
	float lastModelSpace;

	int assignedFrameCount;
	int collisionFrameCount;
	int buildFrameCount;

	bool valid; // startup
	float _timeStep;
	bool forcedEnded;

	//render
	QList<MovePathInfoClip *> assignedClipList;
	
	// IO
	ModelIABuilderPathStorage *storage;
	QList<double> speedInfo;
	int storageTotFrames;
private:
	void Back(int frame);
	bool PopFrame(float &elapsed_time,ModelIAPathFrameBuild *frame,bool recordInfo);
	void DoDebug(const QString &txt);

	void PushStack(ModelIAPathFrameBuild *frame);
	void PopStack(ModelIAPathFrameBuild *frame);

public:

	ModelIABuilderPath(ModelIAActor *actor,ModelInstance *mi);
	virtual ~ModelIABuilderPath();

	ModelInstance *GetModel(){ return mi;}

	void SetMaxFrameCount(int maxFrameCount);
	void StartAssign();
	void StartKeep();
	void StartBuild();
	void FitToTime(double time);

	void SetStorage(QDomElement &ele);
	bool GetIsLive();

	QList<MovePathInfo *> &GetPathList(){return pathList;}
	QList<MovePathInfoClip *> &GetAssignedClipList(){return assignedClipList;}

	void Init();
	bool Step(ModelIAPathFrameBuild *frame);
	
	int GetFrameCount();
	bool FindClipByTime(float globalTime,int autoLoopCount,MovePathInfoClip **clipInfo,MovePathInfoClipGfx **gfxClip,float &localTime,float &startPathSpace,float &startPathHeight);

	bool HaveStorage();

	void RewindFrame(int frame); 
	int RewindFullFrame(int frame); 
	void RewindSpace(float space); 

	void OnEndAssign();
	void OnEndBuild();
	void PushFrame();

	bool Load(QDomElement &e);
	void Save(QDomDocument &doc,QDomElement &ele);

	QString Dump();
};


#include "Anima_nsend.h"