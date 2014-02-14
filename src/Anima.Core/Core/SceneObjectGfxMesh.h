#pragma once

#include "./SceneObject.h"
#include "Anima_nsbegin.h"

class SceneObjectGfx;
class SubSceneObjectGfx;
class OBBoxRenderableEntry;
class MeshExtGeomExt;
class CollisionObject;
class MeshExporter;

#define RENDER_QUEUE_ANIMA_BACKGROUND  Ogre::RENDER_QUEUE_MAIN +1
#define RENDER_QUEUE_ANIMA_BASE_GEOMETRY  Ogre::RENDER_QUEUE_MAIN  + 0
#define RENDER_QUEUE_ANIMA_PATH Ogre::RENDER_QUEUE_MAIN +2
#define RENDER_QUEUE_ANIMA_MODEL  Ogre::RENDER_QUEUE_MAIN + 3
#define RENDER_QUEUE_ANIMA_EDIT  Ogre::RENDER_QUEUE_MAIN + 4
#define RENDER_QUEUE_ANIMA_HELP  Ogre::RENDER_QUEUE_MAIN + 5

class SceneObjectGfxMeshInfo : public BObject
{
public:
	SceneObject *obj;
	int subIdx;
	bool skeletonBuilded;
	CollisionObject *collObj;
public:
	
	SceneObjectGfxMeshInfo(SceneObject *obj) : obj(obj)
	{
		skeletonBuilded=false;
		subIdx=0;
		collObj=NULL;
	}
};

enum SceneObjectGfxMeshCollide
{
	None = 0,
	Ogre = 1,
	Full
};
class SceneObjectGfxMesh : public BObject
{
//friend class SceneObjectGfx;
//friend class SubSceneObjectGfx;
public:
	//enum SceneObjectGfxMeshEditMode
	//{
	//	Normal = 0,
	////	Selected = 1,
	//	Move=1,
	//	Rotate=2,
	//};


private:
	bool changed;
	SceneObjectGfxMeshCollide collidable;
	bool collisionEnable;
	bool lastEffect;

protected:
	OBBoxRenderableEntry *box;

protected:
	Ogre::SceneNode *mainNode;

	QList<Ogre::MovableObject *> normalList;
	QList<Ogre::MovableObject *> glowList;

	// pick state

//	SceneObjectGfxMeshEditMode editMode;
	//int subSelectedIdx;
	bool bboxMode;
	bool visible;
	bool isOver;
	bool isFocus;
	bool isOutOfPath;
	int subSelectedIdx;
	int oldSubSelectedIdx;

	bool haveEffectGlow;

	int _non_render_visible;

public:
	SubSceneObjectGfx *subSceneObjectGfx;

protected:
	virtual void OnPreRender(bool &meshChanged){}

	void AddNode(Ogre::MovableObject *obj);
	void AddNode(Ogre::SceneNode *node);

	virtual void Create(Ogre::String &name)=0;
	virtual void GetEffectMovableObject(Ogre::SceneNode *node,QList<Ogre::MovableObject *> &list);
	void GetMovableObject(Ogre::SceneNode *node,QList<Ogre::MovableObject *> &list);
	void SetupRenderQueue();

	QString GetNextName();

public:
        SceneObjectGfxMesh(SceneObjectGfxMeshCollide collidable=Ogre);
	virtual ~SceneObjectGfxMesh();

	bool IsCreated(){return mainNode!=NULL;}
	//void Free();

	void CollisionEnable(bool enable);
	bool IsCollidable(){return collidable;}

	virtual QString GetName(){return "SceneObjectGfxMesh";}
	Ogre::SceneNode *GetMainNode(){return mainNode;}

	virtual int GetRenderQueueGroup()=0;

	virtual void OverChanged(bool over,int subSelectedIdx);
	virtual void SetUserFocus(bool isFocus);
	virtual void SetOutOfPath(bool isOutOfPath);

	bool IsSelected();
//	int GetSelectionIdx(){return subSelectedIdx;}
	void CreateMesh(Ogre::SceneNode *node,Ogre::String &name);

	Ogre::Vector3 GetPosition();
	void setPosition(Ogre::Vector3 &pos);
	void setOrientation(Ogre::Quaternion &ro);

	virtual bool IsVisible();
	virtual void SetVisible(bool visible);
	virtual bool IsPreviewRenderVisible(){return false;}

	virtual bool IsBBoxMode();
	virtual bool SetBBoxMode(bool bboxMode);
	virtual void OnBBoxModeChaged();


	bool IsChanged(){return changed;}
	virtual void Invalidate();

	virtual void PreRender(bool &meshChanged);

	void ClearEffect();
	void ApplyEffect(bool normalEffect,int selected=-1);

	virtual void UpdateEffect(){}
	virtual void OnAddEffect(){}
	virtual void OnDelEffect(){}

	virtual bool HaveEffectGlow(){return haveEffectGlow;}
	void SetHaveEffectGlow(bool haveEffectGlow){this->haveEffectGlow=haveEffectGlow;}

	virtual MeshExtGeomExt *ExportGeometry(MeshExporter *exporter){return NULL;}
};


#include "Anima_nsend.h"

