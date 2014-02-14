#pragma once


#include "Core/Clip.h"
//#include "Core/AnimClip.h"
//#include "Core/ModelClip.h"
//#include "Physic/BodyModder.h"
//#include "Gfx/GfxClipAnim.h"
#include "Anima_nsbegin.h"

class GfxBaseModel;
//class ClipNode;
class GfxClipAnim;
class OgreSpline;
class GfxPathFinder;
//class BodyModder;
class Bone;
class Body;
class ClipBlendAssign;

class ANIMA_CORE_API GfxOverlayClip : public BObject
{

private:

	GfxBaseModel *model;
	OverlayClip *clip;
	Ogre::Animation *animation;

public:

	QString GetID(){return clip->ID();}

	GfxOverlayClip(GfxBaseModel *model,OverlayClip *clip);
	~GfxOverlayClip(void);

	double GetWeight(){return clip->GetWeight();}
	OverlayClip_ComposeMode GetComposeMode(){return clip->GetComposeMode();}

	void Load();
	
	void Evaluate(ClipBlendAssign &assign,double w);

	Ogre::Animation *GetAnimation(){return animation;}
	
};

#include "Anima_nsend.h"