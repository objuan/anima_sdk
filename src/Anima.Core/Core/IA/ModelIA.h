#pragma once

#include "ModelIAActorDecision.h"
#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
class ModelInstanceIAPath;
class ModelIAThread;
class ModelIABuilder;
class MovePathInfoClip;
class ModelIAActorDecision;
class ModelIAActorDecisionBrainNode;
class ModelIAActor;
class MovePathInfoClipGfx;
class OverlayClip;

//class ModelIAFrame_Overlay 
//{
//public:
//	OverlayClip *overClip;
//	float time;
//	float fade;
//	ModelIAFrame_Overlay():overClip(NULL),time(0),fade(0){};
//};
class ModelIAFrame: public BObject
{
public:
	//bool visible;
	Ogre::Vector3 Pos;
	Ogre::Vector3 Dir;
	float xzAngle;
	float h;
	// idx
	MovePathInfoClip *clipInfo;
	float animClipModelTime;
	//ModelIAActorDecision* clipDecision;
	
	// overlay
	//ModelIAFrame_Overlay overlay;

	// render
	float pathNoClipSpace; // distanza sul path del clip espluso il clip
	float pathNoClipHeight; 
	float gfxClipAnimTime;
	MovePathInfoClipGfx *assignedGfxClipAnim;

	ModelIAFrame():clipInfo(NULL){}
};

class ANIMA_CORE_API ModelIA : public BObject
{
private:

	bool valid;
	int currentIdx;
	int phase;
	int endFrame;
	bool changed;

//	ModelIAThread *thread;
	ModelIABuilder *modelIABuilder;

	bool collisionEnable;
	bool collisionViewMode;
	bool cutCollision;
	int cutCollisionPercent;

private:
	void BuildCollision();
	void BuildFrame(double frame);

public:
	ModelIA();
	virtual ~ModelIA(void);

	bool IsValid(){return valid;}

	void RebuildAll();

	bool GetCollisionEnable(){return collisionEnable;}
	bool GetCollisionViewMode(){return collisionViewMode;}
	void SetCollisionEnable(bool enable);
	void SetCollisionViewMode(bool mode);
	bool GetCutCollision(){return cutCollision;}
	void SetCutCollision(bool cutCollision);
	int GetCutCollisionPercent(){return cutCollisionPercent;}
	void SetCutCollisionPercent(int cutCollisionPercent);
	
	ModelIABuilder *GetBuilder(){return modelIABuilder;}
//	ModelIAThread *GetThread(){return thread;}

	//bool IsChanged();

	//void Abort();

	//void Begin();
	//bool Step();

	//void CollectInfo();
};

#include "Anima_nsend.h"