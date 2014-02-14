#pragma once

#include "Core/AnimationPart.h"
#include "Anima_nsbegin.h"

class MovePath;
class LinkSplineMovePath;

class ANIMA_CORE_API AnimationPartClipSpline : public AnimationPart//,public IAnimationPartClipDynamic
{
friend class ContextIO;

protected:
	SplineMovePath *curvePath;
	QMap<ModelInstance *,LinkSplineMovePath *> modelPathMap;

private:

	int GetLastLibClip(int idx);
	//void TestLinkIntegrity(bool &changed);
	void DeleteAnimClip(int idx);
	//bool MoveClipEnd(int idx,double offset);

	// ritorna lo spazio ricavato, aggiunto
	//double ResizeClip(double sizeOffset); // resize ultimo clip
	//double ResizeClip(AnimClip *resizeClip,double sizeOffset); 

public:		
	AnimationPartClipSpline(Animation *animation);
	virtual ~AnimationPartClipSpline();

	virtual AObject::Type GetType(){return AObject::AnimationPartClipSplineType;}
	virtual QString ClassName(){return "AnimationPartClipSpline";}
	virtual QImage Image(){return ResourceManager::Singleton.item_spline;}
	virtual QString GetDesc();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);
	virtual void OnLoad();
	//virtual void Detach();
	

	// interfaccia

	// path

	virtual MovePath *GetPath(ModelInstance *modelInstance=NULL);
	virtual bool HavePath();
	//virtual void OnPathChanged(bool notify);
	//virtual void OnClipChanged(bool notify);

	// gestione interna spline

	AnimClip *ReplaceClip(LibClip *clip,int idx);
	virtual void InsertAnimClip(int idx,AnimClip *clip);
	virtual AnimClip *InsertClip(LibClip *clip,int idx);
	virtual AnimClip *InsertClip(LibClip *clip,int idx,double size);
	virtual AnimClip *AddClip(LibClip *clip);
	virtual AnimClip *AddClip(LibClip *clip,double size);
	virtual void DeleteClip(int idx);
//	virtual bool MoveClip(int idx,double startOffset,double endOffset);
	virtual void MoveClipPos(int idx,int toIdx);

	virtual bool CanMerge(int idx,int idxOffset); // idxOffset>0
	virtual bool CanSplit(int idx);
	//virtual void MergeClip(int idx,int idxOffset);
	//virtual void SplitClip(int idx);

	//virtual void NotifyEvent(AnimationEvent &evt );
	
	virtual void OnAddModel(ModelInstance *mi);
	virtual void OnRemoveModel(ModelInstance *mi);
	//virtual void OnChangedModel(ModelInstance *mi);

	virtual void OnPreRenderTargetUpdate();

	// events

	virtual void OnFPSChanged();

	/*virtual void InvalidateBuild(int type=0);
	virtual void Build();*/

//	virtual double AddPathInfo(ModelInstance *modelInstance,double startTime,ModelInstanceIAPath *path);

protected:
	
	virtual void UpdateGfx(ModelInstance *modelInstance,int subPart,double startSpace,double frameOffset);
};
#include "Anima_nsend.h"
