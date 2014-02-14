#pragma once
#include "Anima_nsbegin.h"
class GfxModel;
class GfxClip;
class Bone;
class Body;
class Marker;
class World;
class BodyPose;
class MarkerFootTranform;
class TransformFun;

class BodyKeyFun
{
private:
	Ogre::Vector2 from;
	Ogre::Vector2 dir;
	double len;
	int keyCount;
public:

	BodyKeyFun(Ogre::Vector2 &from,Ogre::Vector2 &to,int keyCount);
	Ogre::Vector2 Get(int keyIdx);
};

class BodyKey
{
public:
	int key;
	bool haveLeft;
	bool haveRight;
	bool isLastClipKey;
	Ogre::Vector2 leftPos;
	Ogre::Vector2 rightPos;
	Ogre::Vector2 reference;

	BodyKey(){}
	BodyKey(int key) : key(key)
	{
		isLastClipKey=haveLeft=haveRight=false;
	}
	BodyKey(int key,Ogre::Vector2 &leftPos,Ogre::Vector2 &rightPos,Ogre::Vector2 &reference) : key(key),leftPos(leftPos),rightPos(rightPos),reference(reference)
	{
		isLastClipKey=false;
		haveLeft=haveRight=true;
	}
	void SetIsLastClipKey(double isLastClipKey)
	{
		this->isLastClipKey=isLastClipKey;
	}
	void SetLeft(Ogre::Vector2 &v)
	{
		leftPos= v;
		haveLeft=true;
	}
	void SetRight(Ogre::Vector2 &v)
	{
		rightPos= v;
		haveRight=true;
	}
};

class BodyAI {
private:
	Body *body;
	GfxClip *gfxClip;
	QString animSlotName;

	MarkerFootTranform *loadFootTrx[2]; // left, right

	int currentKey;
	TransformFun *refFun;
	TransformFun *footTrx[2];
	Ogre::Vector2 oldPosition;
	bool upDirection;

	Marker *refMarker;
	Marker *leftFoot;
	Marker *rightFoot;

	int clipCount;
	QList<BodyKey> keyList;
	QList<BodyKey> fullKeyList;

public:
	bool beginLeft;
	Ogre::Vector2 bodyReference;

private:
	int GetLastFootIdx(int footIdx);

public:
	BodyAI(Body *body);
	~BodyAI();

	void Begin(GfxClip *clip,const QString &animSlotName);
	void Save();

	void MoveFoot(int footIdx,int keyIdx,Ogre::Vector2 position);
	void EndClip(double refD,double refH);

	void End();

	QString ToString();
};
#include "Anima_nsend.h"