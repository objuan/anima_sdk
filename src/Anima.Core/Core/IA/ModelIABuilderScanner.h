#pragma once

#include "OpenSteer/Proximity.h"
#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
class ModelIAThread;
class ModelIAActor;
class ModelIAFrame;
class ModelIAActorCollision;
class ModelIABuilderPathStorage;
class Animation;
class ModelIAActorSpeedCollisionMap;
class MovePathInfo;
class ModelIABuilderCollisionScanner;

class ANIMA_CORE_API ModelIABuilderScanner
{
private:
	ModelIABuilder *builder;

	float globalHitTime;
	int globalHitFrame;
	ModelIAActor *actor;
	ModelIAActor *otherActor;
	ModelIAActorDecisionAttempt *attempt;
	ModelIAActorDecisionEntry *decisionEntry;

	float fpsFactor;
	int firstFrame;
	MovePathInfo *pathInfo;
	int new_slice_idx;
	int speedMode; // 0 inalterata, 1 solo speed , 2 fade
	ModelIAActorDecisionStorageSpeedFunInfoType speedType;
	float follow_distance;

	float _COLLISION_SPEED_RAMPA_UP_FACTOR;
	float _COLLISION_SPEED_RAMPA_DOWN_FACTOR;
	float _COLLISION_SPEED_RAMPA_MIN_FACTOR;
	float _COLLISION_SPEED_RAMPA_MAX_FACTOR;;
	float _COLLISION_SPEED_RAMPA_FOLLOW_DOWN_FACTOR;

	ModelIAActorCollision *coll;

	// COLLISION SCAN
	ModelIABuilderCollisionScanner *collScanner;
	//bool collisionMode;

protected:

public:

	ModelIABuilderScanner(ModelIABuilder *builder);
	virtual ~ModelIABuilderScanner(void);

	float GetSpeed(ModelIAActor *actor,ModelIAPathFrameBuild *precFrame,float sourceSpeed);
	// processi
	//int ActorScanBySpace(ModelIAActor *actor ,ModelIAActorDecisionEntry *decisionEntry ,int startFrame,int targetFrame);
	int ActorScanByClipTime(ModelIAActor *actor ,ModelIAActorDecisionEntry *decisionEntry ,int startFrame,int targetFrame);
	int ActorScanByGlobalTime(ModelIAActor *actor ,ModelIAActorDecisionEntry *decisionEntry ,int startFrame,int targetFrame,float globalHitTime);
	int ActorScanSpeedMode(ModelIAActor *actor ,ModelIAActorDecisionEntry *decisionEntry ,int startFrame,int targetFrame,ModelIAActor *otherActor);

	int Process(ModelIAActorCollisionEval *eval,int currentFrame);

};

class ANIMA_CORE_API ModelIABuilderCollisionScanner
{
private:
	ModelIAActor *actor;
	ModelIAActor *otherActor;

	ModelIACollisionReport *report;

	bool excludeOther;
	float _collRay;
	ModelIACollisionManager *collManager;
	QList<ModelIAActor *> collisionActors;

public:

	ModelIABuilderCollisionScanner(ModelIAActor *actor,ModelIAActor *otherActor);

	void InitCollisionScan(ModelIACollisionReport *report,bool excludeOther);
	void EndCollisionScan();
	void StepCollisionScan(int frame);

};

#include "Anima_nsend.h"
