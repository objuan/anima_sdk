#pragma once

#include "Core/AnimationPart.h"
#include "Anima_nsbegin.h"

class PointMovePath;
class AnimClip;
class CoreFunction;
class ModelInstance;
class MeshEditable;
class LinkPointMovePath;

class ANIMA_CORE_API AnimationPartClipPoint : public AnimationPart
{
friend class ContextIO;
friend class PointMovePath;
friend class AnimationPart;
friend class LinkPointMovePath;

private:

	PointMovePath *movePath;
	QMap<ModelInstance *,LinkPointMovePath *> modelPathMap;

public:
	AnimationPartClipPoint(Animation *animation);
	virtual ~AnimationPartClipPoint();

	virtual AObject::Type GetType(){return AObject::AnimationPartClipPointType;}
	virtual QString ClassName(){return "AnimationPartClipPoint";}
	virtual QImage Image(){return ResourceManager::Singleton.item_fixed;}
	virtual QString GetDesc(){return "Fixed Animation";}
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	virtual void OnLoad();
	//virtual void Detach();

	AnimClip *SetPose(LibClip *clip);
	//MeshEditable *GetPoseMesh();

	// interfaccia
	virtual void OnPreRenderTargetUpdate();
	virtual void OnAddModel(ModelInstance *mi);
	virtual void OnRemoveModel(ModelInstance *mi);

	// path
	virtual MovePath *GetPath(ModelInstance *modelInstance=NULL);
	virtual bool HavePath();

	// servizio
	void _InvalidateLinks();

	//virtual void NotifyEvent(AnimationEvent &evt );

	//void Build();

protected:
	virtual void UpdateGfx(ModelInstance *modelInstance,int subPart,double startSpace,double frameOffset);
};
#include "Anima_nsend.h"