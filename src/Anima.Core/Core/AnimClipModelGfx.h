#pragma once

#include "Clip.h"
#include "BaseModel.h"
#include "Physic/BodyModder.h"
#include "gfx/GfxClipAnim.h"

#include "Anima_nsbegin.h"
class Model;
class AnimationPartStairs;
class GfxBaseModel;
class GfxClipAnimSet;
class MovePath;
class AnimClip;
class AnimClipModel;
class GfxClipAnim;
class CoreFunction;
class GfxClip;
class ModelClip;
//class BodyModder;
class ModelClipPath;
class AnimClipModelGfx;
//class ModelInstanceIAPath;

class AnimModelClipEntry : public BObject 
{
private:
	BodyModderPtr bodyModder;

public:
	enum AnimModelClipEntryAddType
	{
		None = 0,
		Distance = 1,
		Height =2,
		All_Size = 3
	};
	double clipSpace;
	double clipHeight;
	double clipTime;
	double totalSpace;
	double totalHeight;
	double totalTime; // totali
	Ogre::Vector2 startOffset;
	//bool addSize;;
	double clipCount;
	ModelClip *clip;
	QString clipID;
	
	int subClipIdx;
	AnimClipModelGfx *animClipModelGfx;
	AnimModelClipEntryAddType addType;
	AnimModelClipEntry(AnimClipModelGfx *animClipModelGfx,BodyModderPtr bodyModder,int subClipIdx,double clipCount);
	AnimModelClipEntry(AnimClipModelGfx *animClipModelGfx,ModelClip *clip,double space,double height,double clipCount);
	~AnimModelClipEntry();

	GfxClipAnimPtr clipGfx;
	QString ID();

	void ClearClipGfx();
	GfxClip *GetClipGfx();
	GfxClipAnim *GetClipAnimGfx(bool ensureInit);
	void EnsureInit();
//	void AddPathInfo(MovePath *path,AnimClipModel *modelClip,ModelInstanceIAPath *ia_path,Ogre::Vector2 &clipOffset);
};

class AnimModelClipFunction  : public BObject 
{
private:
	QString fun;
public:
	AnimModelClipFunction(QString fun);

	bool EvalBool(int clipIdx,int userClipIdx,AnimClipModel *prevAnim,AnimClipModel *anim,AnimClipModel *nextAnim);
	double EvalDouble(AnimClipModel *prevAnim,AnimClipModel *anim,AnimClipModel *nextAnim);
};


class ANIMA_CORE_API AnimClipModelGfx : public BObject 
{
public:
	AnimClipModel *animClipModel;
	ModelClip *_modelClip;
	GfxClip *gfxClip;
	MovePath *path;

	ModelClip *workModelClip;
	QList<AnimModelClipEntry *> animSliceList;
	double forceClipTime; // IN
	
	double space;
	double height;
	double clipTime;

	double baseClipCount;
	bool valid;
//	double heightDelta;

	bool update;

	// select

	int selectedPartIdx;
	double currentInnerClipTime;
	AnimModelClipEntry *currentEntry; 
	int currentClipMult;

private:
//	void LoadWork();
	void AddEntry(BodyModderPtr bodyModder,int subClipIdx,double clipCount=-1);
	void AddEntry(ModelClip *clip,double space,double height,double clipCount);
	void AddEntry(ModelClip *clip,double clipCount);

public:
	double escalatorStartTime;

	AnimClipModelGfx(AnimClipModel *animClipModel,ModelClip *modelClip);
	virtual ~AnimClipModelGfx();
	void SetSpace(double space);

	ModelClipPath &GetReferencePath();
	QString ID();
	bool IsFixed();
	bool IsValid(){return valid;}
//	double GetHeightDelta();
	double GetBaseClipCount();
	double GetXZAngleOffset();
	double GetNextClipStartTime(AnimClipModelGfx *clip);
	FixedPathLink &GetFixedEnterInfo();
	FixedPathLink &GetFixedExitInfo();

	ModelClip *GetBaseModelClip(){return _modelClip;}
	ModelClip *GetWorkModelClip(){return workModelClip;}

	GfxClip *GetClipGfx(){return gfxClip;}

	GfxClipAnim *GetFirstClipAnim(bool ensureInit);
	GfxClipAnim *GetLastClipAnim(bool ensureInit);

	GfxClip *GetFirstClipGfx();
	GfxClip *GetLastClipGfx();

	AnimModelClipEntry *GetPrevClipAnim(int idx);
	AnimModelClipEntry *GetNextClipAnim(int idx);

	void GetPreprocessClipList(QList<Clip *> &list);

	double GetUserMultiply();
//	void SetUserMultiply(int mult);
	int GetUserClipIndex();
//	void SetUserClipIndex(int userClipIndex);

	GfxClipAnim *GetSelected();

	// genera troppi task
	void GetSelected(GfxClipAnim **clip,GfxClipAnim **prev,GfxClipAnim **next,MovePath **prevPath,MovePath **nextPath);

	void Build(bool force);
//	void AddPathInfo(ModelInstanceIAPath *ia_path);

	bool OnPreRender(double clipTime,double &startSpace,double &startHeight,double &innerClipTime,bool &relativeSpace);
	virtual QString ToString();
};
#include "Anima_nsend.h"
