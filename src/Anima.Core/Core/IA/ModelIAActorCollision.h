#pragma once

#include "Core/IA/ModelIABuilderPath.h"
#include "OpenSteer/Proximity.h"

#include "Anima_nsbegin.h"


class CollisionActor;
typedef OpenSteer::AbstractProximityDatabase<CollisionActor*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<CollisionActor*> ProximityToken;
typedef OpenSteer::LQProximityDatabase<CollisionActor*> LQPDAV;

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
void CollisionLog(const QString &str);

enum ModelIAActorCollisionPathSide
{
	Left_Front=0,
	Left_Front_90 = 1,
	Left_Behind_90 = 2,
	Left_Behind = 3,
	Right_Front=4,
	Right_Front_90 = 5,
	Right_Behind_90 = 6,
	Right_Behind = 7,
};

class ModelIAActorCollisionInfo: public BObject
{
public:
	float HitPathDistance;
	float HitActorAngle; // deg
	int HitActorPartSide; // ModelIAActorCollisionPathSide
};

class ModelIAActorCollisionFrame: public BObject
{
public:
	int frame;
	bool collide;
	float ActorDistance;
	float HitPathAngle; // deg
	bool HitChangeSide;
	float HitFramesLeft;
	ModelIAActorCollisionInfo ActorInfo;
	ModelIAActorCollisionInfo OtherInfo;
	ModelIAActorCollisionFrame(int frame):frame(frame),collide(false){}

	QString ToString();
};

struct ModelIAActorCollisionActor: public BObject
{
	float HitPathAngle; // deg
	bool HitChangeSide;
	float SpeedFactor;
	float EnterSideAngle;
	float ExitSideAngle;
	ModelIAActorCollisionPathSide EnterSide;
	ModelIAActorCollisionPathSide ProgressSide;
	ModelIAActorCollisionPathSide ExitSide;
};

class ModelIAActorCollisionEval
{
public:
	 bool isDecision;
	 ModelIAActorDecision *decision;
	 ModelIAActorCollision *collision;
	 int backFrame;

	 ModelIAActorCollisionEval():decision(NULL),backFrame(-1),isDecision(true){}

	 int ElapseActor(ModelIAActorCollision *coll,ModelIAActor *actor,int __backFrame);

	 void SetDecision(ModelIAActorDecision *dec);
	// void SetActorBack_SpeedMode(ModelIAActorCollision *coll);

};

class ModelIACollisionReportActor;

class ModelIAActorCollision: public BObject
{
private:
	QList<ModelIAActorCollisionFrame *> frameList;
public:
	int startFrame;
	int endFrame;
	ModelIAActor *actor;
	ModelIAActor *other;
	bool closed;
	bool toDispose;
	bool invalid;
	bool resolved;

//	QMap<ModelIAActor *,ModelIACollisionReportActor *> *collisionPoints;
	
	ModelIAActorCollisionActor actorColl;
	ModelIAActorCollisionActor otherColl;

	int _collisionCount;
	float _collisionTime;
	int startCollisionIdx;
	ModelIAActorDecisionHistory *decisionHistoryActor;
	ModelIAActorDecisionHistory *decisionHistoryOther;
	QString collisionLog;

private:
	float EvalHitAngle(int fromIdx,int toIdx,bool actor,int weightType);
	ModelIAActorCollisionPathSide EvalSide(int fromIdx,int toIdx,bool actor,int weightType);
	bool EvalSideChange(int fromIdx,int toIdx,bool actor,int weightType);
public:
	ModelIAActorCollision(int frame,ModelIAActor *actor,ModelIAActor *other);
	virtual ~ModelIAActorCollision();

	inline int FrameCount(){return frameList.count();}

	void NotifyColl(int frame,ModelIAActor *actor,bool collide);
	void NotifyEndFrame(int frame,QList<ModelIAActorCollisionEval *> &collisionEvalList,bool doEvaluate);
	void Close();

	bool Is(ModelIAActor *actor,ModelIAActor *other);
	inline bool IsClosed(){return closed;}
	inline bool IsDisposed(){return toDispose;}
	inline bool IsInvalid(){return invalid;}
	void OnRemove();

	inline bool IsResolved(){return resolved;}
	void SetResolved(){resolved=true;}

	// info
	inline ModelIAActor *GetFirstActor(){ return actor;}
	inline ModelIAActor *GetOtherActor(){ return other;}
	inline ModelIAActorCollisionActor &GetFirstActorColl(){return actorColl;}
	inline ModelIAActorCollisionActor &GetOtherActorColl(){return otherColl;}
	inline int GetCollisionFrameCount(){return _collisionCount;}
	inline float GetCollisionTime(){return _collisionTime;}
	int GetCollisionFrame();

	//void NotifyExtraCollision(ModelIAActor *extraActor,ModelIAFrame *frame);
	//void ElapseExtraCollision();
	//void DetachExtraCollision();
	//QMap<ModelIAActor *,ModelIACollisionReportActor *> *GetCollisionPoints(){return collisionPoints;}
	

	int GetFirstFrame();
	int GetLastFrame();

	QString ToString();
};


#include "Anima_nsend.h"
