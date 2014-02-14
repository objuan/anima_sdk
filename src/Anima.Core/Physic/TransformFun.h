#pragma once
#include "Anima_nsbegin.h"
struct TransformFunKey
{
public:
	double Time;
	Ogre::Vector3 Pos;
	Ogre::Quaternion Rot;
};

class TransformFun  : public BObject{
private:

	int size;
	int count;
	TransformFunKey *keyList;
	bool disposed;

public:
	TransformFun(int keyCount);
	TransformFun(TransformFun &from);
	~TransformFun();

	void CopyFrom(TransformFun &from);
	void Clear();

	int Size(){return size;}
	int KeyCount(){return count;}
	//TransformFunKey *AddKey(double time,Ogre::Matrix4 &m);
	TransformFunKey *AddKey(double time,const Ogre::Vector3 &Pos,const Ogre::Quaternion &Rot);
	TransformFunKey *GetKey(int idx){return &keyList[idx];}
	int GetKeyIdxByTime(double time);

	//void TransformMatrix(TransformFun &trx);
	void Transform(TransformFun &trx);
};


#include "Anima_nsend.h"
