#pragma once

#include "BodyCoord.h"
//#include "Task/TaskObject.h"
#include "MarkerModder.h"
#include "Anima_nsbegin.h"
class Body;
class GfxClip;
class MovePath;
class Model;
class Clip;
class ModelClip;
class BodyPose;
class GfxClipAnim;

class BodyModderDelta: public BObject
{
public:
	PhysicCoord start;
	PhysicCoord end;
};

class BodyModderFootDeltaW: public BObject
{
public:
	Ogre::Vector3 start;
	Ogre::Vector3 end;
	bool startAbsolute;
	bool endAbsolute;
public:

	BodyModderFootDeltaW(Ogre::Vector3 start,Ogre::Vector3 end,bool startAbsolute=false,bool endAbsolute=false):start(start),end(end),startAbsolute(startAbsolute),endAbsolute(endAbsolute)
	{}

	Ogre::Vector3 Start(Ogre::Vector3 &footOffset)
	{
		if (startAbsolute) return start;
		else return start + footOffset;
	}
	
	Ogre::Vector3 End(Ogre::Vector3 &footOffset)
	{
		if (endAbsolute) return end;
		else return end + footOffset;
	}

};

class BodyModder;
class BodyModderSubClip: public BObject
{
friend class BodyModder;
friend class BodyModderDistance;
friend class BodyModderStair;
friend class BodyModderCutFrame;
friend class BodyModderStairFirstStep;
friend class BodyModderStairLastStep;

public:
	enum HipFromFoot
	{
		START_LEFT_END_LEFT = 0,
		START_LEFT_END_RIGHT = 1,
		START_RIGHT_END_LEFT = 2,
		START_RIGHT_END_RIGHT = 3,
		H_DISABLED = 8,
	};

private:

	int subClipIdx;
	GfxClip *gfxClip;
	BodyModder *bodyModder;
	//Body *infoBody;
	//Body *taskBody;
	
	//Body *taskBody;

	MovePath *movePath;

	int initValue; // 0 , 1 info 2 full
	CoordOrigin coordOrigin;
	// va verso z, fisso
	QList<PhysicCoord> hipMarker;
	QList<PhysicCoord> leftMarker;
	QList<PhysicCoord> rightMarker;

	QList<PhysicCoord> hipMarkerEnd;
	QList<PhysicCoord> leftMarkerEnd;
	QList<PhysicCoord> rightMarkerEnd;

	QList<PhysicCoord> leftMarkerIK;
	QList<PhysicCoord> rightMarkerIK;

	BodyModderDelta left;
	BodyModderDelta right;
	BodyModderDelta hip;

	Ogre::Vector3 footOffsetL;
	Ogre::Vector3 footOffsetR;

	MarkerModderPos::MarkerModderPosPolicy hipBuildPolicy;
	MarkerModderPos::MarkerModderPosPolicy leftBuildPolicy;
	MarkerModderPos::MarkerModderPosPolicy rightBuildPolicy;

	// out
	double space;
	double height;
	Ogre::Vector2 startOffset; // rimpiazza il size del clip precedente
	Ogre::Vector2 clipOffset;
	int clipCount;
	double time;
	int addType;
	//double startAnimTimeOffset;

public:
	int lastKey;
	Body *body;

	BodyModderSubClip(BodyModder *bodyModder,GfxClip *clip,int subClipIdx);

	//double GetStartAnimTimeOffset(){return startAnimTimeOffset;}

	void SetStartPosD(BodyModderFootDeltaW &left,BodyModderFootDeltaW &right,bool isLeft);
	void SetEndPosD(BodyModderFootDeltaW &left,BodyModderFootDeltaW &right,bool isLeft);

	// OUT
	inline GfxClip *GetGfxClip(){return gfxClip;}
	inline int GetAddType(){ return addType;}
	void GetInfo(QString &clipID,double &space,double &height,double &time,int &clipCount,Ogre::Vector2 &startOffset);
	void GetInfo(QString &clipID,float &space,float &height,float &time,int &clipCount,Ogre::Vector2 &startOffset);

	/// -------
	void Init(bool infoPhase,Ogre::Vector2 &clipOffset);
	void InitForBuild(Ogre::Vector2 &clipOffset);
	void InitForLoad(Ogre::Vector2 &clipOffset);

	void End();
	void LoadSmall(BodyModderFootDeltaW &leftFoot,BodyModderFootDeltaW &rightFoot,HipFromFoot hipFromFoot);
	void Load(BodyModderFootDeltaW &leftFoot,BodyModderFootDeltaW &rightFoot,HipFromFoot hipFromFoot);
	void Load(bool infoPhase,BodyModderFootDeltaW &leftFoot,BodyModderFootDeltaW &rightFoot,HipFromFoot hipFromFoot);

	void Build(const QString &animSlotName);

	ClipCoord GetLeftToe(int keyIdx);
	ClipCoord GetRightToe(int keyIdx);

	void SetFootOffset(double D_L,double D_R,double H);

	void SetBuildPolicy(MarkerModderPos::MarkerModderPosPolicy policy);

	virtual void Draw(double time);
};

class BodyModder : public BObject//: public TaskObject
{
friend class BodyModderSubClip;

protected:
	GfxClip *gfxClip;
	Body *infoBody;
	Body *taskBody;
	MovePath *movePath;
	QList<BodyModderSubClip *> bodyModderSubClipList;

	bool loaded;
	
protected:

	void Init();
	virtual void OnLoad(){}

	BodyModderSubClip *Add(GfxClip *clip);
	BodyModderSubClip *GetBuilder(int subClipIdx);

public:
	BodyModder(MovePath *movePath,GfxClip *clip);
	~BodyModder();

	GfxClip *GetGfxClip(){return gfxClip;}
	MovePath *GetMovePath(){return movePath;}

	virtual QString Type()=0;
	// interfaccia esterna
	virtual void GetPlayInfo(double &baseTimeOffset){baseTimeOffset=0;}
	virtual int GetSubClipCount();
	virtual BodyModderSubClip *GetSubClip(int subClipIdx);
//	virtual void GetClipInfo(int subClipIdx,double &space,double &height,double &time,int &clipCount,Ogre::Vector2 &offset,bool &addSize)=0;

	virtual void Build(int subClipIdx,const QString animSlotName); // ALTRO THREAD

	virtual void Draw(GfxClipAnim *anim,double time);
};

class BodyModderPtr : public Ogre::SharedPtr<BodyModder> 
 {
 public:
     BodyModderPtr() : Ogre::SharedPtr<BodyModder>() {}
     explicit BodyModderPtr(BodyModder* rep) : Ogre::SharedPtr<BodyModder>(rep) {}
     BodyModderPtr(const BodyModderPtr& r) : Ogre::SharedPtr<BodyModder>(r) {} 
     //BodyModderPtr(const ResourcePtr& r);
     //BodyModderPtr& operator=(const ResourcePtr& r);
 protected:
     void destroy(void);
 };


class BodyModderDistance : public BodyModder
{
private:
	double space;
	double startClipTime;
	double endClipTime;
	double startTime;
	double endTime;
public:

	BodyModderDistance(MovePath *movePath,GfxClip *clip,double space,double startClipTime,double endClipTime);

	virtual QString Type(){return "Distance";};

	virtual void GetPlayInfo(double &baseTimeOffset){baseTimeOffset=startTime;}
	virtual BodyModderSubClip *GetSubClip(int subClipIdx);
	//virtual void GetClipInfo(int subClipIdx,double &space,double &height,double &time,int &clipCount,Ogre::Vector2 &offset,bool &addSize);
	virtual void Build(int subClipIdx,const QString animSlotName);
};

class BodyModderCutFrame : public BodyModder
{
private:
	double startClipTime;
	double endClipTime;

	double startTime;
	double endTime;

public:

	virtual void GetPlayInfo(double &baseTimeOffset){baseTimeOffset=startTime;}
	BodyModderCutFrame(MovePath *movePath,GfxClip *clip,double startClipTime,double endClipTime);

	virtual QString Type(){return "CutFrame";};

	virtual BodyModderSubClip *GetSubClip(int subClipIdx);
	//virtual void GetClipInfo(int subClipIdx,double &space,double &height,double &time,int &clipCount,Ogre::Vector2 &offset,bool &addSize);
	virtual void Build(int subClipIdx,const QString animSlotName);
};

class BodyModderEscalator : public BodyModder
{
private:

public:

	BodyModderEscalator(MovePath *movePath,GfxClip *clip);

	virtual QString Type(){return "Escalator";};

	virtual BodyModderSubClip *GetSubClip(int subClipIdx);
	virtual void Build(int subClipIdx,const QString animSlotName);
};

#include "Anima_nsend.h"