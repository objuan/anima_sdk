#pragma once

#include "Core/ModelLibrary.h"

#include "Anima_nsbegin.h"

class Model;
class MeshEditable;
class Clip;
class GfxClip;
class MovePath;
class AnimClipModel;
class AnimClipModelGfx;
class ModelInstance;
class IMesh;
class GfxTexture;
class GfxClipAnim;
class Body;
class ClipBlend;
class GfxOverlayClip;
class ModelInstanceIAPathPos;
class ModelIAFrame;
//class AnimPhysicController;

class ANIMA_CORE_API GfxModelInstanceRender
{
public:

	static ModelInstance *lastModelInstance;

private:

	static void BuildAnkle(Ogre::Entity *mBaseModel);
//	static void RemoveState(QList<Ogre::AnimationState *> &activeStateList,Ogre::AnimationState *state1,Ogre::AnimationState *state2,Ogre::AnimationState *state3);

	static void ClearOverlay(ModelInstance *mi);
	static void AddOverlay(ModelInstance *mi,GfxOverlayClip *clipAnim,double time,double fadeFactor);
	static void AddModelPose(ModelInstance *mi,double time);

public:

	static void SelectClipTime(ModelInstance *mi,GfxClip *clip,double time);
	static void SelectClipTime(ModelInstance *mi,GfxClipAnim *clip,double time);
	static void SelectClipMorph(ModelInstance *mi,GfxClipAnim *prevClipAnim,GfxClipAnim *actualClipAnim,GfxClipAnim *nextClipAnim,double actualTime,ClipBlend *clipBlendPrev,ClipBlend *clipBlendNext);

//	static void Render(ModelInstance *modelInstance,MovePath *path,double startSpace,AnimClipModel *animClip,double animClipTime);
	//static void Render(ModelInstance *modelInstance,ModelInstanceIAPathPos &pos);
	static void Render(ModelInstance *mi,bool assign,ModelIAFrame *posPrev,ModelIAFrame *posNext ,double blurTime );
	//static void Render(ModelInstance *modelInstance,GfxClipAnim * selectedClip);
};
#include "Anima_nsend.h"