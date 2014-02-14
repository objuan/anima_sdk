#pragma once

#include "Context.h"
#include "ClipBlend.h"
#include "Anima_nsbegin.h"

class Model;
class Clip;
class ClipLink;
class ModelClipFootInfo;
class GfxBaseModel;
class BaseModel;
class ClipBlend;
class OverlayGroupClip;
class OverlayClip;
class QRandom;

class ANIMA_CORE_API BaseModelClip : public BObject 
{
private:
	Clip *clip;
	BaseModel *baseModel;

public:
	
	double XZAngleOffset; 
	double DirectionOffset;

	bool path_blend; 
	double YOffset; 
	double DOffset; 
	double Escalator_DOffset;
	double Post_DOffset; 
	double GroundOffset;
	bool cutDOffset;
	bool dynamicDirection;
	double minResizePerc;
	double maxResizePerc;
	// path link for fixed
	FixedPathLink enterInfo;
	FixedPathLink exitInfo;

	// loaded
	double clipTime; // seconds
	double clipDistance; 
	double clipHeight;

	ModelClipFootInfo *footInfo;
	QMap<QString,double> NextClipStartTimeMap;

	BaseModelClip(BaseModel *baseModel,Clip *clip);
	virtual ~BaseModelClip();
	QString ID();

	// IO
	void Save();
};

class ANIMA_CORE_API BaseModel: public BObject 
{
friend class ModelLibrary;
friend class Model;
friend class ClipBlend;
friend class BaseModelClip;
private:
	QString config_filePath;
	Ogre::String rootBone;
	ClipBlend *defaultClipBlend;
	QList<ClipBlend *> clipBlendList;
	QMap<QString,ClipBlendChannel *> clipChannelMap;
	QHash<QString,BaseModelClip *> clipMapName;
	QList<Model *> activeModelList;
	QMap<QString,int> boneInfo;

	// overlay
	QHash<QString,OverlayGroupClip *> clipOverlayGroupMapID;
	QList<OverlayGroupClip *> overlayGroupClipList;
	QList<OverlayGroupClip *> overlayDefaultGroupClipList;
	GfxBaseModel *gfxModel;

public:
	bool loaded;
	bool initialized;
	QString name;
	QString scale;
	QString meshPath; 
	//Ogre::MeshPtr mesh;
	Ogre::SkeletonPtr  skeleton;
	QString defineFilePath;
	float baseSpeed;

	BaseModel(const QString &defineFilePath);
	virtual ~BaseModel();

	QString GetName(){return name;}

	static void InitBones(Ogre::Bone *bone,bool ikBone);

	//void CopyAnim(Ogre::Animation *fromAnim,Ogre::Skeleton::BoneHandleMap &map,double startTime,double endTime,Ogre::SkeletonPtr toSkeleton, Ogre::Animation *toAnim,bool isLoop);
	void Load();
	void UnLoad();
	Ogre::String GetRootBone(){return rootBone;}

	ClipBlendChannel *GetChannel(const QString &id);
	void AddChannel(ClipBlendChannel *channel);
	ClipBlend *GetClipBlend(const QString &fromID,const QString &toID);
	BaseModelClip *GetClip(const QString &fromID);

	GfxBaseModel *GetGfxModel();

	QString GetImportPath();
	QString GetDefinePath(){return defineFilePath;}

	void NotifyModel_AnimationDelete(const Ogre::String &id);
	int GetBoneID(const QString &name);

	// overlay
	QList<OverlayGroupClip *> &OverlayGroupClipList(){return overlayGroupClipList;}
	OverlayClip *FindOverlayByID(const QString &id);
	OverlayGroupClip *FindOverlayGroupByID(const QString &id);
	OverlayGroupClip *GetDefaultOverlayGroup(QRandom &rnd);
};


#include "Anima_nsend.h"
