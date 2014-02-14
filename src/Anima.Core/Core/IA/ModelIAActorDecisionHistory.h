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

class ModelIAActorDecisionStorageSpeedFunInfo: public BObject
{
public:
	bool enabled;
	double hit_space_offset;
	ModelIAActorDecisionStorageSpeedFunInfoType type;
	double follow_distance;

	ModelIAActorDecisionStorageSpeedFunInfo(){enabled=false;}
};

class ModelIAActorDecisionStorageReplaceInfo: public BObject
{
public:
	LibClip *clip;
	double hit_space_offset;
	double hit_clip_time;
	//ModelIAActorDecisionStorageSpeedFunInfo clipSpeedFun;
};


class ModelIAActorDecisionStorage: public BObject
{
private:
	static QList<ModelIAActorDecisionStorage *> storageList;
public:
	QString Name;
	QList<ModelIAActorDecisionStorageReplaceInfo> replaceList;

	ModelIAActorDecisionStorageSpeedFunInfo speedDecision;

	ModelIAActorDecisionStorageReplaceInfo *GetReplace();
	ModelIAActorDecisionStorageReplaceInfo *GetReplace(QRandom &rnd);

	static void Clear();
	static void LoadAll(QDomElement e);
	virtual void Load(QDomElement e);
	static ModelIAActorDecisionStorage *Find(const QString &name);
};

// =============

class ModelIAActorDecisionHistoryDecision: public BObject
{
public:
	double startSpace;
	double endSpace;
	MovePathInfoClip *clipInfo;
	ModelIAActorDecisionPtr decision;

	ModelIAActorDecisionHistoryDecision(ModelIAActorDecision* decision,bool storePtr);
	virtual ~ModelIAActorDecisionHistoryDecision();
	QString ToString();
};


class ModelIAActorDecisionHistoryDecisionList: public BObject
{
public:
	QList<ModelIAActorDecisionHistoryDecision *> List;
public:
	void Add(ModelIAActorDecisionHistoryDecision *dec){List.append(dec);}
	bool HaveDecisions(){return List.count() >0;}
	void SortByAttempt();
};

class ModelIAActorDecisionHistory: public BObject
{
private:
	QMap<ModelIAActorDecision *,ModelIAActorDecisionHistoryDecision *> decMap;
public:
	ModelIAActorDecisionHistory(){}
	~ModelIAActorDecisionHistory();

	void Clear();
	void Notify(int frame,ModelIAPathFrameBuild *pos);

	void Get(double spaceStart,double spaceEnd,ModelIAActorDecisionHistoryDecisionList &list);
	QString ToString();
};

#include "Anima_nsend.h"