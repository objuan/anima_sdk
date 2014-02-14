#pragma once
#include "Anima_nsbegin.h"
class GfxModel;
class GfxClip;
class Bone;
class Marker;
//class World;
class BodyPose;
class IKSkeleton;
class TransformFun;
class GfxClipPathBuilder;

class Body : public BObject{
friend class Bone;
friend class World;
private:

	GfxModel *model;
	GfxClip *last_clip;
	Bone *root;

	Ogre::Bone *hipsBone;
	QString hipsBoneName;

	int idx;
	int currentPoseIdx;
	//TransformFun *parentClipTrx;
	Bone *referenceBone;
	QHash<QString,Marker *> markerMap;

	QList<Bone *> boneList;

	Ogre::Skeleton *skeleton;
	IKSkeleton *ikSkeleton;

	// aus
	int phase;
	bool loading;

	bool useBodyTrx;

private:

	Bone *Initialize(Bone *rootBone,Ogre::Bone *b);

public:
	Body(GfxModel *model);
	~Body();

	//Ogre::Skeleton *GetSkeleton(){return skeleton;}
//osxport
	Marker *AddMarker(Bone *bone,const QString &name,Ogre::Vector3 relPosition = Ogre::Vector3(0,0,0));
	QList<QString> GetMarkerNameList(){return markerMap.keys();}

	bool GetUseBodyTrx(){return useBodyTrx;}

	void LoadClip(GfxClip *clip,int key);
	void LoadClip(GfxClip *clip);
//	void UnlockClip();

//osxport
	void SaveToAnimation(Ogre::Animation *anim,int startKey,Ogre::Vector3 posOffset=Ogre::Vector3(0,0,0),double timeOffset=0);
	//void SaveToCurrentTake(int startKey,Ogre::Vector3 &posOffset=Ogre::Vector3(0,0,0),double timeOffset=0);
	
	//GfxModel *GetGfxModel(){return model; }
	GfxClip *GetLastClip(){return last_clip;}
	Marker *GetMarker(const QString &name);
	Marker *GetReferenceMarker();
	Bone *GetBone(const QString &name,Bone *bone=NULL);
	Bone *GetRoot(){return root;}

	void LoadPose(int keyIdx,Ogre::Vector3 &worldOffset);

	void Rebuild();

	void CopyAnim(Ogre::Animation *fromAnim,double startTime,double endTime, Ogre::Animation *toAnim,bool isLoop,GfxClipPathBuilder *hipBuilder);

	// 

	void SavePoseToCurrentToTake(double time);

	void MoveToMarker(double force,double &distanceError);

	void Draw(double time);

	void KeyPressEvent(class QKeyEvent *e);

	QString ToString();
};
#include "Anima_nsend.h"
