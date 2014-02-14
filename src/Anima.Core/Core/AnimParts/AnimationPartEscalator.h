#pragma once

#include "Core/AnimationPart.h"
#include "Anima_nsbegin.h"

class EscalatorMovePath;
class AnimClip;
class CoreFunction;
class ModelInstance;
class MeshEditable;

class ANIMA_CORE_API AnimationPartEscalator : public AnimationPart
{
friend class ContextIO;

private:

	EscalatorMovePath *movePath;
	double stepSpeed;
	bool customSpeed;
	double width;
	bool customWidth;
	double enterW;
	double exitW;
	double inclinationDeg; // 0 = free
	bool customDeg;

public:
	AnimationPartEscalator(Animation *animation);
	virtual ~AnimationPartEscalator();

	virtual AObject::Type GetType(){return AObject::AnimationPartEscalatorType;}
	virtual QString ClassName(){return "AnimationPartEscalator";}
	virtual QImage Image(){return ResourceManager::Singleton.item_escalator;}
	virtual QString GetDesc();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	virtual void OnLoad();
	//virtual void Detach();

	//AnimClip *SetPose(LibClip *clip);

	// interfaccia

	// path
	virtual MovePath *GetPath(ModelInstance *modelInstance=NULL);
	virtual bool HavePath();

	virtual void OnStartTimeChanged();

	void SetStepSpeed(double stepSpeed,bool customSpeed);
	double GetStepSpeed(){return stepSpeed;}
	bool GetCustomSpeed(){return customSpeed;}

	void SetWidth(double width,bool customWidth);
	double GetWidth(){return width;}
	bool GetCustomWidth(){return customWidth;}

	void SetInclinationDeg(double inclinationDeg,bool customDeg);
	double GetInclinationDeg(){return inclinationDeg;}
	double GetRealInclinationDeg();
	bool GetCustomDeg(){return customDeg;}

	void SetEnterExit(double enterW,double exitW);
	void GetEnterExit(double &enterW,double &exitW);

	//virtual void OnPathChanged(bool notify);
	//virtual void OnClipChanged(bool notify);
	virtual void OnAddModel(ModelInstance *mi);

//	virtual void OnAnimClipsBeforeChanged(AnimationPart *part);

	//virtual bool NotifyNearClipChanged(AnimationPart *nearPart,bool leftPart);
	//virtual void NotifyNearPathChanged(AnimationPart *nearPart,bool leftPart);
	//virtual void NotifyEvent(AnimationEvent &evt );

	virtual void OnPreRenderTargetUpdate();

	void InnerBuild();

protected:
	virtual void UpdateGfx(ModelInstance *modelInstance,int subPart,double startSpace,double frameOffset);
};
#include "Anima_nsend.h"