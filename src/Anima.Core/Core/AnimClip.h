#pragma once

#include "Clip.h"
#include "SceneObject.h"
#include "SceneObjectMaterial.h"
#include "BuildDispatcher.h"
//#include "Property.h"

#include "Anima_nsbegin.h"
class Model;
class AnimationPartStairs;
//class GfxModel;
class AnimClipGfxSet;
class MovePath;
class AnimationPart;
class AnimClip;
class AnimClipGfx;
class CoreFunction;
class AnimClipModel;
class ModelInstance;
class AnimClipCollection;

//class AnimClipProperty
//{
//public:
//	int userCurrentClipIndex;
//	int userMultiply;
//	AnimClipProperty()
//	{
//		userMultiply=1;
//		userCurrentClipIndex=0; // il primo di default
//	}
//};


#define ANIM_CLIP_PREV_CHANGED 1
#define ANIM_CLIP_NEXT_CHANGED 2
#define ANIM_CLIP_SIZE_CHANGED 4
#define ANIM_CLIP_VALID_CHANGED 8

class ANIMA_CORE_API AnimClip : public SceneObject, public SceneObjectMaterial//, public IBuildElement
{
private:

	// spazio

	double size;
	MovePath *path;
	AnimClipCollection *animClips;
	LibClip *clip;

	QString _prevClipID;
	QString _nextClipID;
	AnimClip *prevClip;
	AnimClip *nextClip;

	int changed;
	QHash<ModelInstance *,AnimClipModel *> modelClipList;

//	PropertyCollection propertyList; // max
	//bool isValid;

private:
	static bool  GetCondition(LinkClipTargetInfo *info,LibClip *target,MovePath *path,bool sideFromPrev);

public:

	AnimClip(AnimClipCollection *animClips);
	AnimClip(AnimClipCollection *animClips,LibClip *clip);
	virtual ~AnimClip();

	MovePath *GetPath(ModelInstance *modelInstance=NULL);

	AnimClip *GetPrevClip(ModelInstance *modelInstance);
	AnimClip *GetNextClip(ModelInstance *modelInstance);

	//bool IsChanged(){return changed!=0;}
	//bool IsChanged(int flag){return (changed & flag) != 0;}
	//void ClearChanged(){changed=0;}


	AnimationPart *GetAnimationPart();
	virtual AObject::Type GetType(){return AObject::AnimClipType;}
	virtual QString ClassName(){return "AnimClip";}
	//virtual SceneObject *GetParent();

	void EvaluateValid();

	virtual QImage Image(){return ResourceManager::Singleton.item_color;}
	virtual QString GetName(){return clip->GetName();}
	virtual QString GetDesc(){return clip->GetDesc();}
	//virtual QColor GetColor(){return clip->GetColor();}
	//virtual QString GetMaterial(){return clip->Material;}
	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	int GetIdx();
	int GetAnimationIdx();
	//PropertyCollection &GetPropertyList(){return propertyList;}

	// cross
	bool IsDynamic(){return clip->IsDynamic();}
	bool IsPose(){return clip->IsPose();}
	LibClip *GetClip(ModelInstance *model=NULL);
	void SetClip(LibClip *clip);
	QString ID(){return clip->ID;}
	ClipType GetClipType(){return clip->Type();}
	double GetDefaultSize(){return clip->GetDefaultSize();}
	double GetMinimumSize(){return clip->GetMinimumSize();}
	
	// model clip

	void OnAddModel(ModelInstance *model);
	void OnRemoveModel(ModelInstance *model);
	//void OnChangedModel(ModelInstance *model);

	AnimClipModel *GetModelClip(ModelInstance *model);
	AnimClipModel *GetFirstModelClip();

	// path
	// NULL == size del path di base
	//double GetSize(ModelInstance *modelInstance=NULL);
	//bool SetSize(double size);
	//void Invalidate();
	//bool IsValid(){return isValid;}

	void SetBounds(AnimClip *prevClip,AnimClip *nextClip);

	static bool GetClipSequence(LibClip *from,LibClip *target,QList<LinkClip *> *linkList,MovePath *fromPath,bool sideFromPrev);//,bool &leftSide); // da la lista della sequenza dei link per i clip

	//LibClip *GetClip(MovePath *path);

	// properties
	void SetUserClipIndex(int userClipIndex);
	// render
	//void OnRender();

	QString ToString();
	QString ToString(ModelInstance *modelInstance);

	//void Update();
};
#include "Anima_nsend.h"
