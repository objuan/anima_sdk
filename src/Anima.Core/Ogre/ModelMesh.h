#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"

class ModelInstance;

//class ModelMeshBBOX
//{
//public:
//
//	static ModelMeshBBOX *Create();
//};

class ModelMesh : public SceneObjectGfxMesh  
{
private:

	ModelInstance *mi;
	QString entityName;
	Ogre::Entity *entity;
	Ogre::Entity *eff_entity;
//	OgreOpcode::EntityCollisionShape *collisionShape;
	QList<Ogre::String> materialList;

	ModelMesh *baseMesh;

private:
	void AssignMaterials();
//	void SetOutline();

public:

	ModelMesh(ModelInstance *mi);
	~ModelMesh();

	virtual void Create(Ogre::String &name);

	virtual void OverChanged(bool over,int subSelectedIdx);
	
	virtual bool IsVisible();
	virtual void SetVisible(bool visible);
	virtual bool IsPreviewRenderVisible(){return true;}

	virtual bool SetBBoxMode(bool bboxMode);
	virtual void OnBBoxModeChaged();

	virtual void OnAddEffect();
	virtual void OnDelEffect();
	virtual void UpdateEffect();

	void Dump();
	virtual void OnPreRender(bool &meshChanged);
	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_MODEL;}
};

class ModelMeshAdmin : public SceneObjectGfxMesh  
{
	ModelInstance *mi;
	Ogre::ManualObject *mo;

public:
	ModelMeshAdmin(ModelInstance *mi);
	~ModelMeshAdmin();

	virtual void Create(Ogre::String &name);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_MODEL;}

	virtual void OnPreRender(bool &meshChanged);
};

#include "Anima_nsend.h"
