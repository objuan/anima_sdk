#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"

class EntityMesh : public SceneObjectGfxMesh  
{
private:

	SceneObject *sceneObject;
	QString entityName;
	Ogre::Entity *entity;
	QList<Ogre::String> materialList;
	void AssignMaterials();
	bool haveEffect;

protected:
		
public:

	EntityMesh(SceneObject *so,const QString &entityName,SceneObjectGfxMeshCollide collidable=Ogre);
	~EntityMesh();

	Ogre::Entity *GetEntity(){return entity;}

	void Create(Ogre::String &name);
	void SetHaveEffect(bool haveEffect,bool haveEffectGlow);
	void SetMaterial(int subEntityIdx,const Ogre::String &matName);

	virtual void OnPreRender(bool &meshChanged);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};

#include "Anima_nsend.h"
