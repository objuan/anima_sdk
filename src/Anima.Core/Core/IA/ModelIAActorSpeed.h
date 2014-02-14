#pragma once

#include "Core/IA/ModelIAActor.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
class ModelIAThread;
class ModelIABuilderPath;
class Animation;
class ModelIAActor;
class ModelIAFrame;
class ModelIACollisionManager;
class CollisionActor;
class ModelIAActorDecisionSpeedFunc;
class ModelIAActorDecisionHistoryDecisionList;
class ModelIAActorDecisionHistoryDecision;
class ModelIABuilderScanner;

class ModelIAActorSpeedCollisionInfoLink
{
public:

	ModelIAActorSpeedCollisionInfo *from;
	ModelIAActorSpeedCollisionInfo *to;
	float dist;

	void Use();
};

class ModelIAActorSpeedCollisionInfo: public BObject
{
public:
	float lineTime;
	float fps;
	ModelIAActor *actor;
	int lastFrameLoaded;

	// -- node --	
	QList<ModelIAActorSpeedCollisionInfoLink *> linkList;

public:
	Ogre::Vector3 *posArray;
	int posArrayCount;
	float _spaceStep ;

	bool changeUsed;
	//ModelIAActorSpeedCollisionInfoLink *collTargetLink;

	// decision
	float currentDecisionDistance;
	ModelIAActorSpeedCollisionInfo *currentDecision;
	int currentNullDecision;

public:
	ModelIAActorSpeedCollisionInfo(ModelIAActor *actor);
	~ModelIAActorSpeedCollisionInfo();

	void SetCollisionScanSpeed();
	void SetCollisionScanClip();

	void LoadFrame(int frame,ModelIAFrame *actorFrame );
	// node
	void BeginHit();
	ModelIAActorSpeedCollisionInfoLink *AddHit(ModelIAActor *actor,float dist);
};

class ModelIAActorSpeedCollisionMap: public BObject
{
private:
	QList<ModelIAActorSpeedCollisionInfoLink *> entryList;

public:
	void Start();
	void End();

	void Begin(ModelIAActor *actor);
	void AddHit(ModelIAActor *fromActor,ModelIAActor *toActor,float dist);
};

class ModelIAActorSpeedFunc: public BObject
{
private:
	ModelIAActor *actor;
	
	float baseValue;
	
	float _actualSpeed;
	float _actualAcc;
	ModelIABuilderScanner *scanner;

	float _COLLISION_SPEED_BLEND_UP_FACTOR;
	float _COLLISION_SPEED_BLEND_DOWN_FACTOR;
//	float spaceFrom;
//	float spaceTo;

public:
	// collision
	//bool haveCollision;


public:
	ModelIAActorSpeedFunc(ModelIAActor *actor,double baseValue);
	virtual ~ModelIAActorSpeedFunc();

	void Restart(float speed);

	inline float GetBaseSpeed(){return baseValue;}
	inline float GetActualSpeed(){return _actualSpeed;}
//	void SetSpaceLimit(float spaceFrom,float spaceTo);
	void SetScanner(ModelIABuilderScanner *scanner){this->scanner=scanner;}

	void Build(ModelIAActorSpeedCollisionMap *collMap,ModelIACollisionManager *collManager,int lastFrame);
	float Compute(ModelIAPathFrameBuild *,float sourceSpeed);

	void Load(QDomElement &e);
	void Save(QDomDocument &doc,QDomElement &ele);
};


#include "Anima_nsend.h"