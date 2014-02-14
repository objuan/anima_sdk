#pragma once

#include "Anima_nsbegin.h"

class Body;
class GfxModel;
class GfxClip;
class ModelInstance;
class IKSkeleton;

class AnimPhysicController
{
private:

	Body *body;
	GfxModel *model;
	Ogre::Skeleton *skeleton;
	Ogre::Animation *anim;
	GfxClip *clip;
	ModelInstance *mi;
	double localTime;
	bool editMode;
	int keyIdx;
	Ogre::Vector3 pos;
	IKSkeleton *ikSkeleton;

	Ogre::Vector3 leftMarkerPos;

public:
	AnimPhysicController(GfxModel *model,double maxTime);
	~AnimPhysicController();

	void Load(GfxClip *clip);
	void Select(ModelInstance *mi,Ogre::Vector3 &pos,double localTime);
	void KeyPressEvent(class QKeyEvent *e);

	void Draw();
};

#include "Anima_nsend.h"