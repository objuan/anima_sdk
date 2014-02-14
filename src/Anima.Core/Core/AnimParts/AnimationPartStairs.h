#pragma once

#include "Core/AnimationPart.h"
#include "./LinkMovePath.h"
#include "Anima_nsbegin.h"

class StairMovePath;
class AnimClip;
class CoreFunction;
class ModelInstance;

class ANIMA_CORE_API AnimationPartStairs : public AnimationPart
{
friend class ContextIO;

private:
//	AnimClip *animClip;
	StairMovePath *movePath;
	QMap<ModelInstance *,LinkStairMovePath *> modelPathMap;

	bool directionUp;
	int stepCount;
	double stepWidth;

public:
	AnimationPartStairs(Animation *animation);
	virtual ~AnimationPartStairs();

	virtual AObject::Type GetType(){return AObject::AnimationPartStairsType;}
	virtual QString ClassName(){return "AnimationPartStairs";}
	virtual QImage Image(){return ResourceManager::Singleton.item_stairs;}
	virtual QString GetDesc();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	virtual void OnLoad();
	//virtual void Detach();
	
	// properties

	void SetStepCount(int stepCount);
	int GetStepCount(){return stepCount;}
	double GetStepWidth(){return stepWidth;}
	void SetStepWidth(double stepWidth);

	bool IsDirectionUp(){return directionUp;}
	void ComputeAutoStep();

	// interfaccia

	void GetSelectedLink(QList<LinkStairMovePath *> &list);
	
	// path
	virtual MovePath *GetPath(ModelInstance *modelInstance=NULL);
	virtual bool HavePath();

	virtual void OnAddModel(ModelInstance *mi);
	virtual void OnRemoveModel(ModelInstance *mi);

	virtual void OnPreRenderTargetUpdate();
	// notify

	void InnerBuild();

protected:
	virtual void UpdateGfx(ModelInstance *modelInstance,int subPart,double startSpace,double frameOffset);
};
#include "Anima_nsend.h"
