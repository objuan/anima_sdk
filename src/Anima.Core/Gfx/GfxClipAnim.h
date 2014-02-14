#pragma once

#include "Task/Task.h"
#include "Core/AnimClip.h"
#include "Core/ModelLibrary.h"
#include "Physic/BodyModder.h"
#include "Anima_nsbegin.h"

class GfxModel;
class GfxClip;
//class BodyModder;
class GfxClipAnimTask;
class GfxClipPathBuilder;
class ClipBlend;

class ANIMA_CORE_API GfxClipAnim  : public BObject
{
friend class GfxClip;
friend class GfxBaseModel;
friend class GfxClipAnimTask;
friend class MovePathInfoClipGfx;

public:
	//
	//enum ClipFadeMask
	//{
	//	ClipFade_No = 0,
	//	ClipFade_LeftLeg = 1,
	//	ClipFade_LeftRight = 2,
	//	ClipFade_Body = 4
	//};

protected:
	int idx;
	int node_idx;

	bool isBaseAnim;
	QString slotName;
	double distance;
	double height;
	double clipTime;
	Ogre::Vector2 startOffset;
	int clipCount;
	BodyModderPtr bodyModder; 
	int subClipIdx;
	GfxClip *gfxClip;
	GfxClipPathBuilder *pathBuilder;
	double baseTimeOffset;
	Ogre::Skeleton *skeleton;

	bool needBuild;
	bool queuePresent;
	bool ready;

	GfxClipAnimTask *task;

private:

	//void InitAnimQ(KFbxNode *node,double time);

protected:

	void TaskRun();
	void TaskAbort();
	//void TaskError();

	GfxClipAnim();

public:
	GfxClipAnim(BodyModderPtr bodyModder,int subClipIdx);
	GfxClipAnim(GfxClip *gfxClip,int subClipIdx);
	~GfxClipAnim(void);

	BodyModderPtr GetBodyModder(){return bodyModder;}

	double GetBaseTimeOffset(){return baseTimeOffset;}
	bool QueuePresent(){return queuePresent;}
	void SetQueuePresent(bool queuePresent){this->queuePresent=queuePresent;}
	bool NeedBuild(){return needBuild;}
	bool IsReady(){return ready;}

	//virtual QString TaskName(){return "GfxClipAnim";}

	//double GetStartAnimTimeOffset(){return startAnimTimeOffset;}
	bool IsBaseAnim(){return isBaseAnim;}
	GfxClip *GetGfxClip();

	QString &SlotName(){return slotName; }
	double GetDistance(){return distance;}
	double GetHeight(){return height;}
	double GetTime(){return clipTime;}
	int GetSubClipIdx(){return subClipIdx;}

	QString ID();
	double GetXZAngleOffset();
	double GetDirectionOffset();
	double GetDOffset();
	double GetYOffset();
	void GetClipInfo(double time,double &space,Ogre::Vector3 &hipTranslate,double& xzAngle);
	void GetClipFixedInfo(double time,Ogre::Vector3 &start,Ogre::Vector3 &pos,double& xzAngle);

	// life cycle
	void Build();
	void UnBuild();
	int GetUsedMemory(); // memory usege

	void SelectBegin();
	void SelectEnd();

	ClipBlend *GetClipFade(GfxClipAnim *toAnim);
	bool CanPathFade(GfxClipAnim *toAnim);
	bool CanFade(GfxClipAnim *toAnim);

//	virtual void Draw(double time);
};

class GfxClipAnimPtr : public Ogre::SharedPtr<GfxClipAnim> 
 {
 public:
     GfxClipAnimPtr() : Ogre::SharedPtr<GfxClipAnim>() {}
     explicit GfxClipAnimPtr(GfxClipAnim* rep) : Ogre::SharedPtr<GfxClipAnim>(rep) {}
     GfxClipAnimPtr(const GfxClipAnimPtr& r) : Ogre::SharedPtr<GfxClipAnim>(r) {} 
     //BodyModderPtr(const ResourcePtr& r);
     //BodyModderPtr& operator=(const ResourcePtr& r);
 protected:
     void destroy(void);
 };

// TASK LIST

//class ANIMA_CORE_API DeleteGfxClipAnimTask : public Task
//{
//private:
//	GfxClipAnimPtr gfxClipAnim;
//
//public:
//
//	DeleteGfxClipAnimTask(GfxClipAnimPtr gfxClipAnim);
//	virtual QString TaskName(){return "Delete";}
//	virtual void TaskRun();
//
//};

class ANIMA_CORE_API GfxClipAnimTask: public Task
{
private:
	GfxClipAnimPtr gfxClipAnim;
public:

	GfxClipAnimTask(GfxClipAnimPtr _gfxClipAnim)
	{
		this->gfxClipAnim=_gfxClipAnim;
		_gfxClipAnim->task = this;
	}
	virtual ~GfxClipAnimTask(void);
	virtual QString TaskName(){return "GfxClipAnim";}
	virtual void TaskRun();
};

#include "Anima_nsend.h"
