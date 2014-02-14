#pragma once

#include "Anima_nsbegin.h"

class ModelInstance;
class ModelIAActor;
class ModelIAActorCollision;
class ModelIAFrame;

class ANIMA_CORE_API ModelIACollisionReportActor
{
public:
	ModelIAActor *actor;
	QList<Ogre::Vector3> actorCollisionLine;
};

class ANIMA_CORE_API ModelIACollisionReport: public BObject
{
public:
	int startFrame;
	int endFrame;

	QList<ModelIACollisionReportActor *> actorList;

	QString engineLog;

	ModelIACollisionReport();
	virtual ~ModelIACollisionReport(void);

	bool IsVisible();
	void SetCollision(ModelIAActorCollision *coll,int currentFrame);

	void Notify(ModelIAActor *actor,int iframe,ModelIAFrame *frame);
	bool End();
};

class ModelIAActorQualityEntry
{
public:
	int TotalCollisionFrames;
};

class ANIMA_CORE_API ModelIAActorQuality : public BObject
{
public:
	QMap<ModelIAActor *,ModelIAActor *> base_CollisionActorMap;
	//QMap<ModelIAActor *,ModelIAActorQualityEntry *> actual_CollisionActorMap;

	int Idx;
	bool HaveCollision;

	int TotalCollisionFrames;
	int TotalActorCount;
	float CollisionWeight;

	void Clear();
	void Compute(float CUT_FRAMES_FACTOR,float CUT_ACTOR_FACTOR,int totalFrame,int totalActors);
	QString ToString();
};

typedef QList<ModelIACollisionReport *> ModelIACollisionReportList;
typedef ModelIACollisionReport *pModelIACollisionReport;

class ModelIAQualityManager
{
protected:

	int totalFrames;
	QMap<ModelIAActor *,ModelIAActor *> actorMap;
	QList<ModelIAActor *> collisionActorList;

	QList<ModelIACollisionReport *> buildCollisionList;

	QList<ModelIACollisionReport *> collisionReportStorage;
	QList<ModelIACollisionReportList *> collisionReportByFrame;

	//QMap<ModelIAActor *,ModelIAQualityManagerEntry *> actorHits;

public:

	ModelIAQualityManager();
	virtual ~ModelIAQualityManager();

	void Clear();

	void BeginBuild();
	void EndBuild(int totalFrames);
	ModelIACollisionReport *NewReport();

	QList<ModelIACollisionReport *> &GetReports(){return collisionReportStorage;}
	int GetReportFrameCount(){return collisionReportByFrame.count();}
	ModelIACollisionReportList *GetCollisionAtFrame(int frame);

	void Process();
	//QList<ModelIAActor *> &GetCollisionActorList(){return collisionActorList;}

	void Load(QDomElement &e);
	void Save(QDomDocument &doc,QDomElement &ele);
};


#include "Anima_nsend.h"