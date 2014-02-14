#pragma once

//#include "Clip.h"
#include "BuildDispatcher.h"

#include "Anima_nsbegin.h"
class ModelInstance;
class AnimClip;
class CoreFunction;
class GfxClip;
class GfxClipAnim;
class Clip;
class MovePathInfoClipGfx;
class ModelClip;
//
//struct AnimClipModelEditClip
//{
//	ModelClip *clip;
//	double size;
//	double escalatorStartTime;
//	double forceClipTime;
//};

enum AnimClipModelAddType
{
	AnimClipModelAddType_None = 0,
	AnimClipModelAddType_Distance = 1,
	AnimClipModelAddType_Height =2,
	AnimClipModelAddType_All_Size = 3
};

class AnimClipModelFunction  : public BObject 
{
private:
	QString fun;
public:
	AnimClipModelFunction(QString fun);

	bool EvalBool(int clipIdx,int userClipIdx,AnimClipModel *prevAnim,AnimClipModel *anim,AnimClipModel *nextAnim);
	double EvalDouble(AnimClipModel *prevAnim,AnimClipModel *anim,AnimClipModel *nextAnim);
};

class ANIMA_CORE_API AnimClipModel : public BObject // public IBuildElement,
{
private:
	AnimClip *animClip;
	ModelInstance *model;

	//QList<MovePathInfoClipGfx *> clipList;
	double time;
	double space;
	//bool isValid;

public:
	// UTILITA'
	//double _startPathClipTime0;
	//double _startPathClipTime1;

private:
	Clip *ProcessClip(Clip *clip,const QString &userClipIdxPropertyName,MovePath *path);

public:
	AnimClipModel(AnimClip *animClip,ModelInstance *model);
	virtual ~AnimClipModel();

	AnimClip *GetAnimClip(){return animClip;}
	ModelInstance *GetModel(){ return model;}
	GfxClip *GetGfxClip();
	MovePath *GetPath();

	// fuori dalla syncro
	AnimClipModel *GetPrevClip();
	AnimClipModel *GetNextClip();

	QString ID();
	bool IsValid();//{return isValid;}
	bool IsPose();
	//double GetClipSpace();
	//double GetClipTime();
	//double GetClipTime(CoreFunction *speedFun);

	// non calcola , da solo i glip coinvolti
	GfxClip *PreviewFixed_NearClip(GfxClip *fixedClip,bool leftSide);
	void PreviewFixed_ClipList(GfxClip *fixedClip,QList<GfxClip *> &list,MovePath *path);

	// IBuildElement
	void Build();

	void GetClipList(QList<ModelClip *> &clipList);

	// render

	static void GetPreprocessClipList(Clip *clip,QList<Clip *> &list);

	//void BeginAssign();
	//void AddClip(MovePathInfoClipGfx *clip);

	QString ToString();
};
#include "Anima_nsend.h"
