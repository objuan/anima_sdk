#pragma once


#include "Gfx/GfxBaseClip.h"
#include "Anima_nsbegin.h"

class GfxBaseModel;
class GfxClipAnim;
class GfxPathFinder;
class GfxModel;
class Body;

class ANIMA_CORE_API GfxClip: public BObject
{
private:

	Ogre::Animation *animation;

	bool mergeMode;
	GfxModel *model;
	GfxBaseModelClip *gfxBaseModelClip;

	QList<GfxClipAnimPtr> subClipList;

public:

	GfxClip(GfxModel *model,GfxBaseModelClip *gfxBaseModelClip);
	~GfxClip();

	QString GetID();
	Clip &GetClip();

	GfxModel *GetModel(){return model;}
	GfxBaseModel *GetBaseModel();

	virtual GfxClipPathBuilder *GetPathBuilder();

	virtual Ogre::Animation *GetAnimation();
	Ogre::Skeleton *GetSkeleton();

	virtual QList<double> &GetKeyTimes();

	// model clip properties

	double GetClipTime();
	double GetClipDistance();
	double GetClipHeight();

	GfxClipAnimPtr GetAnimClip(BodyModderPtr bodyModder,int subClipIdx);
	// 

	Body *GetInfoBody();
	Body *GetTaskBody();

	double GetPreClipInAngle(){return gfxBaseModelClip->GetPreClipInAngle();}
	double GetPreClipOutAngle(){return gfxBaseModelClip->GetPreClipOutAngle();}

	double GetGroundOffset(){return gfxBaseModelClip->GetGroundOffset();}
	double GetXZAngleOffset(){return gfxBaseModelClip->GetXZAngleOffset();}
	double GetDirectionOffset(){return gfxBaseModelClip->GetDirectionOffset();}
	double GetDOffset(){return gfxBaseModelClip->GetDOffset();}
	bool GetCutDOffset(){return gfxBaseModelClip->GetCutDOffset();}
	bool GetDynamicDirection(){return gfxBaseModelClip->GetDynamicDirection();}
	OgreSpline *GetClipPath(){return NULL; } // SOLO UI EDIT
	bool GetPathBlend(){return gfxBaseModelClip->GetPathBlend();}
	FixedPathLink *GetEnterInfo(){return gfxBaseModelClip->GetEnterInfo(); }
	FixedPathLink *GetExitInfo(){return gfxBaseModelClip->GetExitInfo(); }

	GfxBaseModelClip *GetBaseModelClip(){return gfxBaseModelClip;}

	ModelClipFootInfo &GetFootInfo();

	// 
	void GetClipFixedInfo(double time,Ogre::Vector3 &start,Ogre::Vector3 &pos,double& xzAngle);
	void GetClipInfo(double time,double &space,Ogre::Vector3 &translate,double& xzAngle);

	void GetClipFixedInfo(float time,Ogre::Vector3 &start,Ogre::Vector3 &pos,float& xzAngle);
	void GetClipInfo(float time,float &space,Ogre::Vector3 &translate,float& xzAngle);

};

#include "Anima_nsend.h"