#pragma once

#include "OpenSteer/Proximity.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
class ModelIABuilder;
struct ModelIABuilderPathFrame;
class ModelIAPathFrameBuild;
class ModelIAActor;
class ModelIAActorCollision;

class CollisionActor;

typedef OpenSteer::AbstractProximityDatabase<CollisionActor*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<CollisionActor*> ProximityToken;
typedef OpenSteer::LQProximityDatabase<CollisionActor*> LQPDAV;

class CollisionActor: public BObject
{
public:
	//int modelIdx;
	ProximityToken *token;
	//ModelIAFrame Pos;
	ModelIAActor *Actor;
	Ogre::Vector3 Pos;
	bool IsLive;

	CollisionActor(ModelIAActor *Actor);
	virtual ~CollisionActor(void);

	void StartBuild(ProximityDatabase *coll_db);

	void Move(ModelIAFrame *pos);
	void Hide();
};

class ANIMA_CORE_API ModelIACollisionManager: public BObject
{
private:
	//Ogre::AxisAlignedBox area;
	ProximityDatabase *coll_db;
	std::vector<CollisionActor *> _coll_result;

	Ogre::Vector3 bound_min;
	Ogre::Vector3 bound_max;
	float min_x;
	float min_z;
	int dx;
	int dz;

public:

	ModelIACollisionManager();
	virtual ~ModelIACollisionManager();

	void Notify(ModelIAActor *actor,int frame);//,QList<ModelIAActorCollision *> &activeCollList);

	ProximityDatabase *getProximityDatabase(){return coll_db;}

	CollisionActor *FindNeighbors (ModelIAActor *actor,const float radius,QList<CollisionActor *>& results);

	//Ogre::AxisAlignedBox &GetArea(){return area;}
};

#include "Anima_nsend.h"
