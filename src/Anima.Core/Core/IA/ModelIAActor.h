#pragma once

#include "Core/IA/ModelIA.h"
#include "Core/IA/ModelIABuilderPath.h"
#include "OpenSteer/Proximity.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;
class ModelIAThread;
class ModelIABuilderPath;
class Animation;
class ModelIAActor;
//class ModelIAFrame;
class ModelIACollisionManager;
class CollisionActor;
class ModelIABuilder;
class ModelIAFramePos;
class ModelIAActorDecision;
class ModelIAActorSpeedFunc;
class ModelIAActorDecisionHistory;
class CollisionActor;
class ModelIAActorQuality;

class ModelIAActorSpeedCollisionInfo;
void CollisionLog(const QString &str);

enum ModelIAActorState
{
	NotAssigned = 0,
	InvalidBuild,
	InvalidKeep,
	Valid
};
//
//enum ModelIAActorScanCollisionType
//{
//	COLLISION_TYPE_FOR_CLIP = 0,
//	COLLISION_TYPE_FOR_SPEED = 1,
//};


class ANIMA_CORE_API ModelIAActor 
{
private:
	ModelInstance *mi;
	ModelIABuilderPath *path;
	CollisionActor *collisionActor;
	ModelIAActorQuality *quality;
	
	ModelIAActorSpeedFunc *speedFun;

	// collision
	QList<ModelIAPathFrameBuild *> buildFrameList;
	QList<ModelIAFrame *> frameList;

	ModelIAActorState state;
	bool buildEnable;
	ModelIAFrame renderAssignPos;

	QMap<ModelIAActor  *,ModelIAActorCollision *>  collMap;
	//ModelIAActorScanCollisionType collisionScanType;
	float collisionScanDistance;
	ModelIAActorSpeedCollisionInfo *collisionInfo;
	
	// runtime
	bool isBuilding;

public:
	int builderIdx;
	bool buildActive;

public:

	ModelIAActor(ModelInstance *mi);
	~ModelIAActor();

	ModelIAActorState GetState(){return state;}
	void SetState(ModelIAActorState state);
	bool IsBuilding(){return isBuilding;}
	void SetIsBuilding(bool _isBuilding){isBuilding=_isBuilding;}

	bool IsBuildEnable(){return buildEnable;}
	void SetBuildEnable(bool enable){buildEnable=enable;}
	bool NeedBuild();

	void StartAssign(int maxFrameCount);
	void StartKeep();
	void StartBuild(ModelIACollisionManager *collManager);
	void EndCollision();
	void EndCollisionFromStorage();
	void EndAssign();
	void EndAssignStorage();

	ModelIAActorSpeedFunc *GetSpeedFun(){ return speedFun;}
	ModelInstance *GetModel(){return mi;}
	ModelIABuilderPath *GetPath(){return path;}
	inline ModelIAActorQuality *GetQuality(){return quality;}

	inline bool IsLive(){return path->GetIsLive();}
	ModelIAFrame *GetFrameSafe(int frame);
	ModelIAFrame *GetFrame(int frame);
	ModelIAPathFrameBuild *GetFrameBuild(int frame);
	int GetFrameCount();

	CollisionActor *GetCollisionActor(){return collisionActor;}

	//ModelIAActorScanCollisionType GetCollisionScanType(){return collisionScanType;}
	void SetCollisionScanSpeed(float distance);
	void SetCollisionScanClip();
	inline ModelIAActorSpeedCollisionInfo *GetCollisionInfo(){return collisionInfo;}

	bool StepFrame (int frame,ModelIABuilder *builder,ModelIACollisionManager *collManager,bool useStorageSpee);
	int GetFrameAtSpace(int startFrame,int maxFrame,double space);

	void RenderAssign(float globalTime,int autoLoopCount);

	void OnRemove(ModelIAActorCollision *coll);

};


#include "Anima_nsend.h"
