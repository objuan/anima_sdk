#pragma once

#include "TransformFun.h"
#include "Anima_nsbegin.h"
class Bone;
class Marker;

class MarkerTransformFun : public TransformFun
{
private:
	Marker *marker;
public:
	MarkerTransformFun(Marker *marker,int keyCount);
};

class MarkerInfo: public BObject
{
public:
	Ogre::Vector3 startPos;
	Ogre::Vector3 startXZPos;
	Ogre::Vector3 endPos;
	Ogre::Vector3 endXZPos;
	Ogre::Vector3 direction;
	Ogre::Vector3 directionXZ;
	double distance;
	double distanceXZ;
	double height;
};


class Marker : public BObject {
private:

	bool enable;
	QString name;
	Bone *bone;
	Ogre::Vector3 relPosition;

	MarkerTransformFun trxFun;

public:
	Marker(Bone *bone,const QString name,Ogre::Vector3 &relPosition,int keyCount);
	~Marker();

	Bone *GetBone(){return bone;}

	MarkerTransformFun &GetTransformFun(){return trxFun;}

	MarkerInfo GetInfo();

	void SetBoneFun();

	void SetEnable(bool enable){this->enable=enable;}
	bool IsEnabled(){return enable;}

	void SetMarkerPosition(Ogre::Vector3 &p);
//	void Set(TransformFun &trxFun);

	QString GetName(){return name;}

	static double GetDist(MarkerInfo &markerInfo,Ogre::Vector3 &posXZ);
};

#include "Anima_nsend.h"