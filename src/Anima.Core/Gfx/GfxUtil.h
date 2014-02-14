#pragma once
#include "Anima_nsbegin.h"
class GfxModel;
class GfxBaseModel;
class BaseModel;

class ANIMA_CORE_API GfxUtil
{
private:
//	static bool SelectAnim(char *clipName,KFbxNode *skeletonNode);
public:

	static double Angle2DDeg(double dx,double dy);
	static double Angle2DRad(double dx,double dy);
	static double DegToRad(double);
	static double RadToDeg(double);
	static Ogre::Vector3 GetDir90(Ogre::Vector3 direction);

	// ogre

	static Ogre::NodeAnimationTrack *GetHipTrack(GfxBaseModel *model,Ogre::Animation *anim);
	static Ogre::NodeAnimationTrack *GetHipTrack(GfxModel *model,Ogre::Animation *anim);
	//static void MoveHip(Ogre::Animation *anim,Ogre::Vector3 &trx);
//	static Ogre::Vector3 GetStartPos(Ogre::Animation *anim);

	static void FromToRotation(double *from, double *to, double *mtx9);
};
#include "Anima_nsend.h"