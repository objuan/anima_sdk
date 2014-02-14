#pragma once


#include "Core/Clip.h"
#include "Core/AnimClip.h"
#include "Core/ModelClip.h"
#include "Physic/BodyModder.h"
#include "Gfx/GfxClipAnim.h"
#include "Anima_nsbegin.h"

class GfxBaseModel;
//class ClipNode;
class GfxClipAnim;
class OgreSpline;
class GfxPathFinder;
//class BodyModder;
class Bone;
class Body;

class GfxClipPos: public BObject
{
public:
	double time;
	double distXZ;
	double xzAngle;
	Ogre::Vector3 pos;

	GfxClipPos(){}
	GfxClipPos(double time,double distXZ,double xzAngle,const Ogre::Vector3 &pos)
	{
		this->time=time;
		this->distXZ=distXZ;
		this->pos=pos;
		this->xzAngle=xzAngle;
	}
};

class ANIMA_CORE_API GfxClipPathBuilder: public BObject
{
private:
	Ogre::Vector3 minPos;
	Ogre::Vector3 maxPos;
	QList<GfxClipPos *> hipPos;
	double distXZ;
	bool clearPosition;
	double enter90Offset;
	double exit90Offset;

private:
	void FindDirection(int count);
public:
	GfxClipPathBuilder(GfxBaseModel *model,Ogre::Animation *animation,bool clearPosition=true);
	~GfxClipPathBuilder();
	double GetDistXZ(){return distXZ;}
	int Count(){return hipPos.count();}
	GfxClipPos &GetPos(int idx);

//	void GetPosByTime(double time,GfxClipPos &pos,int &idx,float &blend);
	void GetPosByTime(float time,GfxClipPos &pos);

	GfxClipPos &GetPosBySpace(double space,int &idx);
	//int GetIdx(double time);

	double GetEnter90Offset(){return enter90Offset;}
	double GetExit90Offset(){return exit90Offset;}
	int GetUsedMemory(); 
};

class ANIMA_CORE_API GfxBaseModelClip : public BObject
{
	friend class ImportFBX;
	friend class GfxImport;
	//friend class GfxModel;
	friend class Body;
	friend class Bone;
private:

	bool valid;
	GfxBaseModel *model;
	Clip *clip;
	BaseModelClip *baseModelClip;
	//ModelClip *modelClip;
	Ogre::Animation *animation;

	// dal clip
	Ogre::Vector3 direction; 
	double clipTime; // seconds
	double loadedClipDistance;
	double clipDistance; // metri, sul piano orizzontale
	double clipHeight; // metri, in verticale
	double haveCloseCycle;

	//GfxPathFinder *pathFinder;
	bool loaded;  // flag per sapere quando sono presente a disco i movimenti
	GfxClipPathBuilder *pathBuilder;
	//ModelClipPath referencePath;
	
	// runtime
	//QList<GfxClipAnimPtr> subClipList;
	QList<double> keyTimes;

	double pre_clip_inAngle;
	double pre_clip_outAngle;

	// fixed

	Ogre::Vector3 StartDir;
	Ogre::Vector3 EndDir;
	Ogre::Vector3 StartXZDir;
	Ogre::Vector3 EndXZDir;

	// cache
	bool haveCacheData;
	Ogre::Vector3 *keyLocalPos;
	Ogre::Quaternion *keyLocalRot;
	Ogre::Vector3 *keyWPos;
	Ogre::Quaternion *keyWRot;

private:

	void GetClipInfo_Normal(double time,double &space,Ogre::Vector3 &translate,double& xzAngle);
	void GetClipInfo_Dynamic(double time,double &space,Ogre::Vector3 &translate,double& xzAngle);

public:

	GfxBaseModelClip(GfxBaseModel *model,Clip *clip);
	~GfxBaseModelClip(void);

	void Load();
	QList<double> &GetKeyTimes() { return keyTimes;}
	bool IsValid(){return valid;}

	GfxBaseModel *GetBaseModel(){return model;}
	Ogre::Animation *GetAnimation(){return animation;}
	GfxClipPathBuilder *GetPathBuilder(){return pathBuilder;}
	BaseModelClip *GetModelClip(){return baseModelClip;}

	//ModelClipPath &GetReferencePath(){return referencePath;}

	//GfxClipAnimPtr GetAnimClip(BodyModderPtr bodyModder,int subClipIdx);
	//void FreeAnimClip(GfxClipAnim *animClip); 

	//void Clear();

	double GetClipTime(){return clipTime;}
	double GetClipDistance(){return clipDistance;}
	double GetClipHeight(){return clipHeight;}
	bool HaveCloseCycle(){return haveCloseCycle;}

        Clip &GetClip(){return *clip;}
	QString &GetID(){return clip->ID;}
        ClipType GetClipType(){return clip->Type();}
	virtual QString &GetName(){return clip->Name;}

	double GetGroundOffset(){return baseModelClip->GroundOffset;}

	double GetPreClipInAngle(){return pre_clip_inAngle;}
	double GetPreClipOutAngle(){return pre_clip_outAngle;}
	Ogre::Vector3 &GetDirection(){return direction;}
//	GfxPathFinder *GetPathFinder(){return pathFinder;}

	double GetXZAngleOffset(){return baseModelClip->XZAngleOffset;}
	double GetDirectionOffset(){return baseModelClip->DirectionOffset;}
	double GetDOffset(){return baseModelClip->DOffset;}
	double GetEscalatorDOffset(){return baseModelClip->Escalator_DOffset;}
	double GetPost_DOffset(){return baseModelClip->Post_DOffset;}
	bool GetCutDOffset(){return baseModelClip->cutDOffset;}
	bool GetDynamicDirection(){return baseModelClip->dynamicDirection;}
	OgreSpline *GetClipPath(){return NULL; } // SOLO UI EDIT
	bool GetPathBlend(){return baseModelClip->path_blend;}

	FixedPathLink *GetEnterInfo(){return &baseModelClip->enterInfo; }
	FixedPathLink *GetExitInfo(){return &baseModelClip->exitInfo; }

	ModelClipFootInfo &GetFootInfo(){return *(baseModelClip->footInfo);}

	void GetClipFixedInfo(double time,Ogre::Vector3 &start,Ogre::Vector3 &pos,double& xzAngle);
	void GetClipInfo(double time,double &space,Ogre::Vector3 &translate,double& xzAngle);

	void GetClipFixedInfo(float time,Ogre::Vector3 &start,Ogre::Vector3 &pos,float& xzAngle);
	void GetClipInfo(float time,float &space,Ogre::Vector3 &translate,float& xzAngle);
};

#include "Anima_nsend.h"
