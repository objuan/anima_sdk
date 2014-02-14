#pragma once

#include "OpenSteer/Proximity.h"
#include "ModelIAThread.h"
#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;
class ModelIAThread;
class ModelIAActor;
class ModelIAFrame;
class ModelIAActorCollision;
class ModelIABuilderPathStorage;
class Animation;
class ModelIAActorSpeedCollisionMap;
class ModelIABuilderScanner;
class ModelIACollisionManager;
class ModelIAQualityManager;

class ANIMA_CORE_API ModelIABuilder: public BObject, public ModelIAThread
{
private:

	int currentFrame;
	QList<ModelIAActor *> storageActorList;
	QList<ModelIAActor *> actorList;
	QList<ModelIAActor *> invalidActorList;
	QList<ModelIAActor *> inactiveActorList;
	QList<ModelIAActor *> toBuildActorList;
	
	// report
	QList<ModelIAActorCollision *> activeCollList;

	ModelIAActorSpeedCollisionMap *collisionMap;
	ModelIABuilderScanner *builderScanner;
	ModelIACollisionManager *collManager;
	ModelIAQualityManager *qualityManager;

	int currentReportFrame;
	bool mustGfxRebuild;

private:
	//void Clear(ModelIACollisionReportList &list,int frame);
//	void BackCollision(int frame);
	void AddCollision(ModelIAActorCollision *collision,int currentFrame);
	bool CheckAbort();
public:
	ModelIABuilder();
	virtual ~ModelIABuilder(void);

	// processi
	void Build_Assign();
	// collision thread
	virtual void Process(bool checkAbort);

	ModelIAActorCollision *GetActiveCollision(int frame ,ModelIAActor *a1,ModelIAActor *a2);

	inline ModelIACollisionManager *GetCollisionManager(){return collManager;}
	inline ModelIAQualityManager *GetQualityManager(){return qualityManager;}

	void UpdateActorsCut();

	bool MustGfxRebuild();
	void OnCollisionViewModeChanged(bool);

};

#include "Anima_nsend.h"
