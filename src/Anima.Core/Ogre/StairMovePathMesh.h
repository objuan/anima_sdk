#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"
class StairMovePath;
class ModelInstance;

class StairMovePathMesh : public SceneObjectGfxMesh  
{
private:

	Ogre::String name;
//	OgreOpcode::PtrCollisionShape *collisionShape;
	Ogre::ManualObject *mo;
	Ogre::ManualObject *mo_effect;
	StairMovePath *stairMovePath;
	ModelInstance *modelInstance;

public:

	StairMovePathMesh(StairMovePath *stairMovePath,ModelInstance *mi);
	~StairMovePathMesh();

	void Create(Ogre::String &name);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_PATH;}

	virtual void OnPreRender(bool &meshChanged);
};

#include "Anima_nsend.h"
