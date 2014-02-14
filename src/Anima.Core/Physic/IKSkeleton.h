#pragma once

class Link;
class IKChain;
class ArticulatedObject;
struct NewtonWorld;
struct NewtonRagDoll;

#include "Anima_nsbegin.h"

//class GfxModel;
class Body;
class Bone;
class IKLeg;
class RagDoll;

class IKMuscle: public BObject
{
public:
	Bone *bone;
	int degree;
	Ogre::Vector3 target;
	QList<Bone * > dep_bone_list;

	IKMuscle()
	{
		target = Ogre::Vector3::ZERO;
	}

	void Set(Bone *bone,int degree);

	void SetTarget(const Ogre::Vector3 &target)
	{
		this->target=target;
	}

	void Save(int key);
};

class IKSkeleton : public BObject {

private:
	Body *body;
	QList<IKMuscle *> leftLeg;
	QList<IKMuscle *> rightLeg;

public:
	IKSkeleton(Body *body);
	~IKSkeleton();

	void LoadFromBody();
	void MoveAndSave(int keyIdx,bool save=true);

	void Draw(double time);
};


class IKBrain: public BObject
{
private:
	double Test(QList<IKMuscle *> &ikList,Ogre::Quaternion &rot);
	double Get_I1_Angle(QList<IKMuscle *> &ikList,int i0,int i1,int i2);
public:
	IKBrain(){}
	void Process(QList<IKMuscle *> &ikList,bool debugInfo);
};

#include "Anima_nsend.h"