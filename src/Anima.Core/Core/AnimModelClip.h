#pragma once

#include "Clip.h"

class Model;
class AnimationPartStairs;
class GfxModel;
class AnimClipGfxSet;
class MovePath;
class AnimationPart;
class AnimModelClip;
class AnimClipGfx;
class CoreFunction;

struct AnimModelClipEntry
{
public:
	double space;
	double height;
	double time;
	Clip *clip;

	AnimModelClipEntry(double space,double height,double time,Clip *clip)
	{
		this->space=space;
		this->height=height;
		this->time=time;
		this->clip=clip;
	}
};


class AnimModelClipFunction
{
private:
	QString fun;
public:
	AnimModelClipFunction(QString fun);

	bool EvalBool(AnimModelClip *anim,AnimModelClip *nextAnim);
	double EvalDouble(AnimModelClip *prevAnim,AnimModelClip *anim,AnimModelClip *nextAnim);
};


class ANIMA_CORE_API AnimModelClip 
{
protected:

	//bool toUpdate;

	double clipTime;
	double path_start;
	double path_end;
	MovePath *path;

	AnimationPart *animPart;
	Model *model;
	Clip *clip;

	bool changed;
	QString prevAnimHashCode;
	QString nextAnimHashCode;

	AnimClipGfxSet *currentAnimClipGfxSet;
	QMap<QString,AnimClipGfxSet *> *AnimClipGfxMap;
	QList<AnimModelClipEntry> animSliceList;

	// cross info

	double baseClipCount;
	double heightDelta;

	//void SetCurrentTakeNode(KFbxNode *meshNode,char *name);

public:

	AnimModelClip(AnimationPart *animPart,Model *model,Clip *clip);
	virtual ~AnimModelClip();

	double GetPathStart(){return path_start;}
	double GetPathEnd(){return path_end;}

	bool IsDynamic(){return clip->IsDynamic();}
	bool IsPose(){return clip->IsPose();}

	QList<AnimModelClipEntry> &GetAnimSliceList(){return animSliceList;}

	QString GetHashCode();

	Clip &GetClip(){return *clip;}
	QString ID(){return clip->ID;}
	double GetClipTime(){return clipTime;}
	double GetClipTime(CoreFunction *speedFun);
	double GetClipSpace(){return path_end-path_start;}
	double GetBaseClipSpace();
	double GetBaseClipTime();

	void GetClipSequence(AnimModelClip *toClip,QList<LinkClip *> &linkList);

	QString &GetName(){return clip->Name;}
	QString &GetDesc(){return clip->Desc;}
	QColor &GetColor(){return clip->Color;}

	// changed
	void MoveSpace(double path_start);
	void SetTime(double time);
	void Build();
	void NotifyPathChanged(MovePath *path,double path_start,double path_end);
	
	// render
	void CopyToModelAndSelect(GfxModel *model,double clipTime,double &startSpace,double &startHeight,double &innerClipTime);
	
	// info
	double GetSelectedXZAngleOffset();
	double GetSelectedYOffset();
	double GetSelectedDOffset();
	SelectedActiveFootType GetSelectedActiveFoot();
	double GetSelectedBaseTime();
	bool GetSelectedDynamicDir();
	AnimClipGfx *GetSelectedClip();
	//KFbxVector4 GetOffset(double clipTime,int clipCount);

	// cross info

	double GetBaseClipCount(){return baseClipCount;}
	double GetHeightDelta(){return heightDelta;}
	//void SetHeightDelta(double heightDelta){this->heightDelta=heightDelta;}

	// draw

	void Draw(KTime time);
};
