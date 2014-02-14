#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"

class OgreGridPlane : public SceneObjectGfxMesh  
{
private:

public:

	OgreGridPlane();
	~OgreGridPlane();

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_BACKGROUND;}

protected:
	void Create(Ogre::String &name);
};

#include "Anima_nsend.h"
