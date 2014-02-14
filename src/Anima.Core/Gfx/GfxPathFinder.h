#pragma once
#include "Anima_nsbegin.h"

class GfxModel;
class GfxClip;
class AnimClipGfx;

class ANIMA_CORE_API GfxPathFinder
{
private:

	GfxClip *clip;
	GfxModel *model;

	Ogre::Vector3 boundMin,boundMax;

	QList<Ogre::Vector3> hipPoints;
	QList<Ogre::Vector3> hipfeetPoints[2];

private:

//	void ComputeFeetPath(KFbxNode *node ,QList<Ogre::Vector3> &points);

public:

	GfxPathFinder(GfxClip *clip);
	~GfxPathFinder(void);

	Ogre::Vector3 &GetBoundMin(){return boundMin;}
	Ogre::Vector3 &GetBoundMax(){return boundMax;}

	QList<Ogre::Vector3> &GetHipPoints(){return hipPoints; }
	QList<Ogre::Vector3> &GetHipfeetPoints(bool left){return hipfeetPoints[(left)? 0 : 1]; }

	void Process();

	
};
#include "Anima_nsend.h"