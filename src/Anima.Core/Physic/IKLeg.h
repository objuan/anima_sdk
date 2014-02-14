#pragma once

class GfxModel;

class Body;
class Bone;
class IKLeg;

//#include "IK/ik.h"

class IKChain;
class ArticulatedObject;

class IKLeg {

private:

	Body *body;
	Bone *rootBone;
	Bone *target1;
	Bone *target2;

	int level;

	bool isLeft;

	ArticulatedObject *articulatedLeg;
	IKChain * ikChain;
	//IK::Skeleton *skeleton;

private:

	//VectorR3 GetFeetOffset(KTime time);
	//void BuildRot(KTime &time);

public:
	IKLeg(Body *body,Bone *rootBone,bool isLeftt);
	~IKLeg();

	void LoadFromBody();
	void SaveToBody();

	void Draw(KTime time);
};

