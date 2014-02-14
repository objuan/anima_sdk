#pragma once

#include "BaseModel.h"
#include "Clip.h"

#include "Anima_nsbegin.h"
class Model;
class ClipNode;
class ClipMoverSet;
class ClipMover;
class AnimModelClipFunction;
class OgreSpline;
class BaseModelClip;


//class ModelClipPath
//{
//public:
//
//	Ogre::Vector3 StartDir;
//	Ogre::Vector3 EndDir;
//	Ogre::Vector3 StartXZDir;
//	Ogre::Vector3 EndXZDir;
//	QList<Ogre::Vector3> posList;
//	QList<Ogre::Vector3> posXZList;
//
//	ModelClipPath(){}
//};

class ANIMA_CORE_API ModelClipFoot : public BObject 
{
public:
	double StartTime;
	double EndTime;
	double GroundOffset;
	ModelClipFoot(){StartTime=0;EndTime=0;GroundOffset=0;}

	void FromSerial(QString &ser);
	QString Serialize();
};

class ANIMA_CORE_API ModelClipFootInfo : public BObject 
{
public:
	QList<ModelClipFoot *> LeftFoot;
	QList<ModelClipFoot *> RightFoot;

	QList<ModelClipFoot *> &GetFoot(bool isLeft);

	ModelClipFootInfo(){}
	~ModelClipFootInfo();

	void FromSerial(QString &ser);
	QString Serialize();
};

class ANIMA_CORE_API ModelClip  : public BObject 
{
	//friend class ImportFBX;
	friend class GfxImport;
	//friend class GfxModel;
	friend class GfxClip;

private:

	//double clipTime; // seconds
	//double clipDistance; 
	//double clipHeight;
	Model *model;
	Clip *clip;
	BaseModelClip *baseModelClip;
//	ModelClipPath referencePath;
	
	// proprieta' del clip - sul modello
	// dati preimpostati dagli autori
	double XZAngleOffset; 
	double DirectionOffset;
	double YOffset; 
	double DOffset; 
	double Escalator_DOffset;
	double GroundOffset;
	ModelClipFootInfo footInfo;
	QMap<QString,double> nextClipStartTimeMap;
	double minResizePerc;
	double maxResizePerc;
	double start_rot;
	double end_rot;
	//bool clip_blend; 
	bool path_blend; 
//	ClipFadeInfo clip_blend;
	// path link for fixed
	FixedPathLink enterInfo;
	FixedPathLink exitInfo;

	bool cutDOffset;
	bool dynamicDirection;
	OgreSpline *clipPath;

public:

	//ModelClipPath &GetReferencePath(){return referencePath;}
	double GetClipTime();
	double GetClipDistance();
	double GetClipHeight();

	Model &GetModel(){return *model;}
	Clip &GetClip(){return *clip;}
	QString &ID(){return clip->ID;}

	ModelClip(Model *model,Clip *clip);
	~ModelClip();

	void CopyFrom(BaseModelClip *clip);

	// base clip properties
//	SelectedActiveFootType GetStartFootType(){return clip->GetStartFootType();}
//	SelectedActiveFootSideType GetStartFootSideType(){return clip->GetStartFootSideType();}

	QList<Clip *> &GetSubClipList(){return clip->GetSubClipList();}
//	AnimModelClipFunction *GetHDeltaFun(){return clip->GetHDeltaFun();}
	AnimClipModelFunction *GetConditionFun(){return clip->GetConditionFun();}
        Clip_ResizeType GetResizeType(){return clip->GetResizeType();}
	Clip_ComposeType GetComposeType(){return clip->GetComposeType();}
	Clip *GetLastClip(){return clip->GetLastClip();}
	double GetPathOffset(){return clip->GetPathOffset();}
	ModelClipFootInfo &GetFootInfo(){return footInfo;}
	double GetNextClipStartTime(const QString &clipID);
	double GetMinResizePerc(){return minResizePerc;}
	double GetMaxResizePerc(){return maxResizePerc;}

	// model specific properties
	double GetGroundOffset(){return GroundOffset;}
	void SetGroundOffset(double offset){GroundOffset=offset;}
	double GetXZAngleOffset(){return XZAngleOffset;}
	double GetDirectionOffset(){return DirectionOffset;}
	void SetXZAngleOffset(double offset){XZAngleOffset=offset;}
//	double GetYOffset(){return YOffset;}
	double GetDOffset(){return DOffset;}
	bool GetCutDOffset(){return cutDOffset;}
//	void SetYOffset(double offset){YOffset=offset;}
	void SetDOffset(double offset){DOffset=offset;}
	void SetCutDOffset(bool cut){cutDOffset=cut;}
	bool GetDynamicDirection(){return dynamicDirection;}
	OgreSpline *GetClipPath(){return clipPath;}
	//bool GetClipBlend(){return clip_blend; }

//	ClipBlend *GetClipBlend();
	bool GetPathBlend(){return path_blend; }

	FixedPathLink &GetFixedEnterInfo(){return enterInfo;}
	FixedPathLink &GetFixedExitInfo(){return exitInfo;}

	// IO
	void Load(QDomElement &parentEle);
};
#include "Anima_nsend.h"

