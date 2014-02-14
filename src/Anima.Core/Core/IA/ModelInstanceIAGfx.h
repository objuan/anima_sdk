#pragma once

#include "Core/SceneObjectGfx.h"
#include "Core/SceneObjectMaterial.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;

class ModelCollisionGfx: public SubSceneObjectGfx
{
protected:
		
public:

	ModelCollisionGfx(SceneObjectGfx *obj);
	virtual ~ModelCollisionGfx(){}

	virtual QString GetDesc(){return "ModelCollisionGfx";}
	virtual void OnPreRender();
	//virtual AObject::Type GetType(){return AObject::ModelInstanceMoveGfxType;}
};


class ModelCollisionGfxMesh : public SceneObjectGfxMesh  
{
private:
	Ogre::ManualObject *mo_base;
	Ogre::ManualObject *mo_sel;
	bool lastCollMode;

public:
	ModelCollisionGfxMesh();
	virtual ~ModelCollisionGfxMesh();

	void Create(Ogre::String &name);
	void Update();
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};


#include "Anima_nsend.h"