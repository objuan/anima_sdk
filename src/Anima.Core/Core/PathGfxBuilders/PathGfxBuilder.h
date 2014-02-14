#pragma once

#include "../Clip.h"
#include "Physic/BodyModder.h"
#include "Anima_nsbegin.h"

class ModelClip;
class AnimClip;
class AnimClipModel;
class MovePath;
class GfxClip;
class ModelInstance;

class PathGfxBuilder : public BObject 
{
private:
//	AnimClipModel *animClipModel;

protected:
	//MovePath *path;

public:

	static BodyModderPtr GetBodyModder(GfxClip *gfxClip,MovePath *basePath,ModelInstance *mi);
};
#include "Anima_nsend.h"