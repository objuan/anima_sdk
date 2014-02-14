#pragma once

//#include "Core/IA/ModelIAActor.h"

#include "Anima_nsbegin.h"

class LibClip;
class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;
class ModelIAThread;
class ModelIABuilderPath;
class Animation;
class ModelIAActor;
class ModelIAFrame;
class ModelIACollisionManager;
class CollisionActor;
class ModelIAActorDecisionSpeedFunc;
class QRandom;
class ModelIAActorDecisionBrainNodeDecision;
struct ModelIAActorDecisionBrainNodeDecisionTarget;
class ModelIAActorDecision;
class MovePathInfoClip;
class ModelIAPathFrameBuild;
class ModelIAActorCollision;
class MovePathSlice;
class LibClip;
class ModelIAActorCollisionEval;
class ModelIACollisionReport;

class ModelIAActorDecisionPtr : public Ogre::SharedPtr<ModelIAActorDecision> 
 {
 public:
     ModelIAActorDecisionPtr() : Ogre::SharedPtr<ModelIAActorDecision>() {}
     explicit ModelIAActorDecisionPtr(ModelIAActorDecision* rep) : Ogre::SharedPtr<ModelIAActorDecision>(rep) {}
     ModelIAActorDecisionPtr(const ModelIAActorDecisionPtr& r) : Ogre::SharedPtr<ModelIAActorDecision>(r) {} 
     //BodyModderPtr(const ResourcePtr& r);
     //BodyModderPtr& operator=(const ResourcePtr& r);
 protected:
     virtual void destroy(void);
 };

enum ModelIAActorDecisionStorageSpeedFunInfoType
{
	ModelIAActorDecisionStorageSpeedFunInfoType_DISTANCE=0,
	ModelIAActorDecisionStorageSpeedFunInfoType_FOLLOW=1,
};

class ModelIAActorDecisionEntry
{
public:
	ModelIAActorDecisionBrainNodeDecisionTarget *target;
	ModelIAActorDecisionBrainNodeDecision *node;

	bool useSpeed;
	ModelIAActorDecisionStorageSpeedFunInfoType speedType;
	float follow_distance;

	ModelIAActor *actor;
	LibClip *_clip;
	MovePathSlice *_slice;
	float _sliceSpace;
	float hit_space;
	float hit_space_offset;
	float hit_space_last;

	float restartSpace;
	float hit_clip_time;
	int hit_frame;
	QString lastResultClip;
	MovePathSlice *newSlice;

	ModelIAActorDecisionEntry(){useSpeed=false;}
};

class ModelIAActorDecisionAttempt: public BObject
{
private:
	ModelIAActorDecision *decision;
	ModelIAActor *actor;
	ModelIAActor *otherActor;
	ModelIAActorCollision *coll;

	ModelIAActorDecisionEntry *actorDecision;
	ModelIAActorDecisionEntry *otherActorDecision;
	int priority;

	//double targetSpace;
	int targetFrame;
	int collisionFrameCount;
	float firstSpace;
	float lastSpace;

	int firstFrame;
	int lastFrame;

public:
	void CollisionDecisionLog(const QString &str);
	bool DoBuild(ModelIAActorDecisionEntry *entry,ModelIAPathFrameBuild *pos);
	bool CanBuild(ModelIAActorDecisionEntry *entry,ModelIAPathFrameBuild *pos,ModelIAActor *otherActor);

public:
	ModelIAActorDecisionAttempt(ModelIAActorDecision *decision,ModelIAActor *actor,ModelIAActor *otherActor,ModelIAActorCollision *coll,int Priority);
	virtual ~ModelIAActorDecisionAttempt(void);

	void AddDecision(ModelIAActorDecisionBrainNodeDecisionTarget *target,ModelIAActorDecisionBrainNodeDecision *node);

	ModelIAActor *GetActor(){return actor;}
	ModelIAActor *GetOtherActor(){return otherActor;}

	ModelIAActorDecisionEntry *GetActorDecision(){return actorDecision;}
	ModelIAActorDecisionEntry *GetOtherActorDecision(){return otherActorDecision;}

	bool HaveOtherActorDecision(){return otherActorDecision!=NULL; }
	float GetFirstSpace(){return firstSpace;}
	float GetLastSpace(){return lastSpace;}
	int GetCollisionFrameCount(){return collisionFrameCount;}
	bool IsClipBuilded(ModelInstance *mi,const QString &clipID);
	int ApplyRewind();
	inline int GetPriority(){return priority;}

	bool Build(int idx);

	QString ToString();
};

class ANIMA_CORE_API ModelIAActorDecision: public BObject
{
	enum ModelIAActorDecisionState
	{
		Init,
		BuildOnce,
		RetryMode,
		Closed
	};
protected:

	ModelIAActorDecisionPtr ptr;
	ModelIAActorCollision *coll;
	ModelIACollisionReport *report;

	ModelIAActorDecisionState state;
	bool used;

	float restartSpace;
	float _restartSpace;
	//MovePathInfoClip *retryClipInfo;

	ModelIAActorDecisionAttempt *buildAttempt;
	QList<ModelIAActorDecisionAttempt *> attemptList; 

public:
	QString logDecision;
private:
//	double AddSpeedFunction(ModelIAActorDecisionStorageSpeedFunInfo &speedFun,double targetSpace,bool checkLimit,bool checkAdd);
public:
	ModelIAActorDecision(ModelIAActorCollision *coll);
	virtual ~ModelIAActorDecision(void);

	void AddAttempt(ModelIAActorDecisionAttempt *attempt);
	int GetAttemptCount(){return attemptList.count();}

	bool IsUsed(){return used;}
	ModelIAActorDecisionPtr &GetPtr(){return ptr;}
	void CollisionDecisionLog(const QString &str);

	ModelIAActorCollision *GetCollision(){return coll;}

        bool IsBuilded(){return state != Init; }
	ModelIAActor *GetActor(){return buildAttempt->GetActor();}
	ModelIAActor *GetOtherActor(){return buildAttempt->GetOtherActor();}
	int GetCollisionFrameCount(){return buildAttempt->GetCollisionFrameCount();}
	ModelIAActorDecisionAttempt *GetDecisionDetail(){return buildAttempt;}

	bool HaveRestartSpace(){return buildAttempt!=NULL;}
	ModelIACollisionReport *GetReport(){return  report;}

	void Build(ModelIAActorCollisionEval *eval);

	static ModelIAActorDecision *Load(QDomElement &ele);
	void Save(QDomDocument &doc,QDomElement &ele);

	QString LogDecisionHinner();

	QString ToString();
};

#include "Anima_nsend.h"
