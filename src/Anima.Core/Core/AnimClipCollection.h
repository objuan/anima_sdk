#pragma once

#include "Clip.h"
#include "SceneObject.h"

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

class AnimClipCollection : public SceneObject
{
private:
	AnimationPart *animPart;
	QList<AnimClip *> clipList;

	AnimClip *prevClip;
	AnimClip *nextClip;

	QList<ModelInstance *> modelList;

private:

public:

	AnimClipCollection(AnimationPart *animPart);
	virtual ~AnimClipCollection();

	virtual AObject::Type GetType(){return AObject::AnimClipCollectionType;}
	//virtual SceneObject *GetParent();

	virtual AObject::Type GetChildType(int idx){return AObject::AnimClipType;}
	virtual int GetChildCount(AObject::Type type);
	virtual SceneObject *GetChild(AObject::Type type,int idx);
	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	virtual QImage Image(){return ResourceManager::Singleton.item_color;}
	//virtual QColor GetColor(){return Qt::white;}
	virtual QString GetName(){return "";}
	virtual QString ClassDesc(){return "Parts";}
	virtual QString ClassName(){return "AnimClipCollection";}

	AnimationPart *GetAnimationPart(){return animPart;}

	void clear();
	void append(AnimClip *animClip);
	void insert(int idx,AnimClip *animClip);
	void replace(int idx,AnimClip *animClip);
	void removeAt(int idx);
	AnimClip* operator[]( int i ) {	return clipList[i]; }
	AnimClip* Get( int i );
	int count(){return clipList.count();}
	int indexOf(AnimClip *animClip);
	//void Invalidate();

	void UpdateDependences(AnimClip *prevClip,AnimClip *nextClip);

	// clip
	//bool NotifyNearClipChanged(AnimClip *prevClip,AnimClip *nextClip);
	//void OnChangedModel(ModelInstance *mi);

	// models
	void OnAddModel(ModelInstance *model);
	void OnRemoveModel(ModelInstance *model);

	// IO
	virtual void Load(QDomElement &ele);
	virtual void Save(QDomDocument &doc,QDomElement &ele );
};
#include "Anima_nsend.h"