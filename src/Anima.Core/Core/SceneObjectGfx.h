#pragma once

#include "./SceneObject.h"
#include "./SceneObjectGfxMesh.h"
#include "Anima_nsbegin.h"

class SceneObjectGfx;
class SubSceneObjectGfx;


class ANIMA_CORE_API SceneObjectGfx : public SceneObject
{
friend class SubSceneObjectGfx;

typedef QList<SubSceneObjectGfx *> SubSceneObjectGfxList;

private:

	int visible;
	SubSceneObjectGfx *subGfx;
	int groupCount;
	SubSceneObjectGfxList *subGfxMap;
	bool isLive;

protected:
	Ogre::SceneNode *rootNode;
	//Ogre::SceneNode *rootNode_sub;

protected:
	
	void SetMesh(SceneObjectGfxMesh *soGfxMesh); 

	virtual void OnEnabledChanged();
	virtual void OnPreRender(){}
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj){}

public:
	SceneObjectGfx(SceneObject *parent,int childTypeCount,int groupCount);
	virtual ~SceneObjectGfx(void);

	virtual AObject::Type GetType(){return AObject::SceneObjectGfxType;}
	virtual QString ClassName(){return "SceneObjectGfx";}

	Ogre::SceneNode *GetContentNode();

	//virtual void Detach();
	//virtual void Attach();

	void OverChanged(bool over,int subObjectIdx=-1);
//	void SetEditMode(SceneObjectGfxMesh::SceneObjectGfxMeshEditMode mode,int subObjectIdx=-1);

	SubSceneObjectGfx *GetGfx(){return subGfx;}
	void AddSubGfx(int groupIdx,SubSceneObjectGfx *sub,int insertIdx=-1);
	void RemoveSubGfx(int groupIdx,int idx);
	void RemoveSubGfx(int groupIdx,SubSceneObjectGfx *gfx);
	SubSceneObjectGfx *GetSubGfx(int groupIdx,int idx);
	int GetSubGfxCount(int groupIdx);
	SubSceneObjectGfx *GetSubGfxByObjectID(int objectID);
	int GetSubGfxIndex(SubSceneObjectGfx *obj);
	void ClearSubGfx();
	void ClearSubGfx(int groupIdx);

	bool IsBBoxMode();
	virtual bool IsGfxVisible(){return visible;}
	virtual void SetGfxVisible(bool visible);

	virtual void InvalidateGfx(bool fromIK=false);
	virtual void OnPreRenderTargetUpdate();

	virtual Ogre::Vector3 GetLabelGUIPosition();
};

// -------------------------------

struct SubSceneObjectGfxEntry
{
	Ogre::MovableObject* movableObject;
	Ogre::SceneNode *rootNode;
//	Ogre::SceneNode *rootNode_sub;
};

class ANIMA_CORE_API SubSceneObjectGfx : public SceneObject
{
friend class SceneObjectGfx;
friend class SceneObjectGfxMesh;
protected:
	SceneObjectGfx *sceneObjectGfx;
	SceneObjectGfxMesh *soGfxMesh;

	int visible;
	// ogre
	Ogre::String meshName;
	Ogre::SceneNode *rootNode_main;
	Ogre::SceneNode *rootNode_sub;

	bool autoZoom;
	double autoZoomScale;
	bool topMost;

	QString labelText;
	bool labelEnabled;	

	static int _objectID;
	int objectID;

protected:
	void ChangeObject(SceneObjectGfx *sceneObjectGfx);
	Ogre::MovableObject *getMainMovableObject(Ogre::SceneNode *node);
	//virtual void OnPreRenderTargetUpdate();

public:
	SubSceneObjectGfx(SceneObjectGfx *sceneObjectGfx);
	virtual ~SubSceneObjectGfx(void);
	//void Detach();

	virtual AObject::Type GetType(){return AObject::SubSceneObjectGfxType;}
	virtual QString ClassName(){return "SubSceneObjectGfx";}
	virtual QString GetDesc(){return "SubSceneObjectGfx";}

	virtual QImage Image(){return ResourceManager::Singleton.item_color;}
	//virtual QColor GetColor(){return Qt::white;}
	virtual QString GetName(){return "";}

	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

public:

	void SetMesh(SceneObjectGfxMesh *soGfxMesh); 
	SceneObjectGfxMesh *GetMesh(){return soGfxMesh;}
//	void SetEditMode(SceneObjectGfxMesh::SceneObjectGfxMeshEditMode mode,int subObjectIdx=-1);
	void OverChanged(bool over,int subObjectIdx=-1);

	SceneObjectGfx *GetSceneObjectGfx(){return sceneObjectGfx;}

	void InvalidateGfx(bool fromIK=false);
	bool IsVisible();
	void SetVisible(bool visible);

	bool IsBBoxMode();

	virtual void EnsureInit(Ogre::SceneNode *node);
	virtual void FirePreRenderTargetUpdate();


	//Ogre::MovableObject *getMainMovableObject();

	// label

	virtual Ogre::Vector3 GetLabelGUIPosition();
	void SetLabel(bool enabled,const QString &text="",const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0),const Ogre::Vector3 &point = Ogre::Vector3::ZERO );
};


#include "Anima_nsend.h"

