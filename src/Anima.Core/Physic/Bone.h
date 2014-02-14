#pragma once

class Link;

#include "Anima_nsbegin.h"

class Body;
class TransformFun;
class Marker;
class World;
class BodyPose;

//#include "ode/ode.h"

class Bone : public BObject
{
friend class Body;
friend class BodyPose;
friend class Skeleton;
friend class IKSkeleton;
friend class IKMuscle;
friend class Marker;
friend class BodyModderSubClip;
friend class IKLeg;
friend class IKBrain;

public:

private:
	bool visible;
	bool isReference;
	
	QString name;
	Body *body;
	Ogre::Bone *ogreBone;
	int channelIdx;
	Link *link;

	Bone *parent;
	QList<Bone *> childs;

	TransformFun *keyTrx;
	TransformFun *clipTrx; // local coordinates
	TransformFun *outClipTrx; // world coordinate
	Marker *marker;

	Ogre::Quaternion keyRot;
	Ogre::Vector3 localPos;
	Ogre::Quaternion localRot;
	Ogre::Vector3 worldPos;
	Ogre::Quaternion worldRot;

	//TransformFun *keyTrx;

	// debug

	QList<Ogre::Vector3> markerPos;

public:
	// out
//	Ogre::Matrix4 localTrx;
	//Ogre::Matrix4 worldTrx;
	//Ogre::Vector3 worldPos;

private:
	void GetOutLocalTrx(int key,Ogre::Vector3 &localPos,Ogre::Quaternion &localRot);
	void CalcolateWorldTrx();

public:
	Bone(Body *body,Ogre::Bone *ogreBone,Bone *parent);
	~Bone();
	void Clear();

	QString GetName(){return name;}
	TransformFun *GetClipTrx(){return clipTrx;}
	TransformFun *GetOutTrx(){return outClipTrx;}
	Body *GetBody(){return body;}
	Bone *GetParent(){return parent;}
	QList<Bone *> &GetChilds(){return childs;}
	Bone * GetChild(const QString &name);

//	Ogre::Matrix4 GetLocalTrx(int keyIdx);
//	Ogre::Matrix4 GetLocalTrx(int keyIdx,Ogre::Matrix4 &preTrx);
	void GetMarkerLocalTrx(int keyIdx,Ogre::Vector3 &pos,Ogre::Quaternion &rot);
	void TransformOut(TransformFun &trx);
	//void Transform(int key,Ogre::Matrix4 &trx);

	// take

	void LoadBoneInfo(double time,int keyIdx,Ogre::Animation *anim);

	void InitAnim(int keyCount);
	//void LoadCurrentTake(double time);
	void SaveToAnimation(Ogre::Animation *anim,int keyIdx,double timeOffset=0);
	void SaveToAnimation(Ogre::Animation *anim,int keyIdx,Ogre::Matrix4 &preTrx,double timeOffset=0);
	//void ClearCurrentTake();

	// manipolators

	void TransformLocal(Ogre::Quaternion rot);
	
	//void UpdatePoseFromLocal(Ogre::Matrix4 &parentTrx);
	void LoadPose(int keyIdx);//,Ogre::Vector3 &parentO);

	// IO
	void Load(QDomElement &parentEle);

	// render
	void Draw(int keyIdx);
	void DrawPhysic();

	QString ToString();
};
#include "Anima_nsend.h"