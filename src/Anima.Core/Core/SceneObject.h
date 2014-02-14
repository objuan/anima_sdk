#pragma once

#include "./ResourceManager.h"
#include "./DAG.h"
#include "Anima.Core.h"
#include "SceneLibrary.h"
#include "Anima_nsbegin.h"

enum SceneLibraryElementAccess;

class MeshSceneObject;
class Animation;
class AnimationPart;

class ANIMA_CORE_API SceneObjectUI
{
private:
	bool open;

public:
	SceneObjectUI(){open=true;}
	bool GetOpen(){return open;}
	void SetOpen(bool _open){open=_open;}
};

class ANIMA_CORE_API SceneObject : public AObject
{
	friend class ContextIO;
private:
	bool enabled;	
	bool visible;
	int childTypeCount;
	bool disposed;

protected:
	bool notifyDisposed;
	
protected:
    SceneObject *parent;
	virtual void OnPreRenderTargetUpdate(){}
	virtual void OnPostRenderTargetUpdate(){}
	virtual void OnEnabledChanged(){}

public:
	SceneObject(SceneObject *parent,int childTypeCount);
	virtual ~SceneObject(void);

	bool IsDisposed(){return disposed;};

	//class
	virtual bool IsSceneObject(){return true;}
	virtual AObject::Type GetType(){return AObject::SceneObjectType;}
	virtual QString ClassName(){return "SceneObject";}

	virtual QImage Image()=0;
	virtual QString GetName() = 0;
	virtual QString GetDesc(){return "";}

	// tree
	SceneObject *GetParent(){return parent;}
	SceneObject *GetParent(AObject::Type type);
	int GetChildTypeCount(){return childTypeCount;}

	virtual QColor GetMaterialColor();

	// virtuals
	virtual DAG GetDAG()=0;
        virtual AObject::Type GetChildType(int idx){return AObject::SceneObjectType;}
	virtual int GetChildCount(AObject::Type type){return 0;}
	virtual SceneObject *GetChild(AObject::Type type,int idx){return NULL;}
	
	int GetChildCount();
	SceneObject *GetChild(int idx);
	
	int Idx();
	int IdxByType();

	virtual void Copy(SceneObject *fromObj)=0;
	virtual SceneObject *Clone(SceneObject *newParent)=0;
	
	// BUILDING
	bool IsEnabled(){return enabled;}
	bool IsEnabledFromRoot();
	void SetEnabled(bool enabled);

	// RENDER + PLUGIN
	bool IsRenderVisible(){return visible;}
	virtual bool IsRenderVisibleFromRoot();
	void SetRenderVisible(bool visible);

	// EDIT
	bool IsSelected();
	virtual bool HasAccess(SceneLibraryElementAccess access){return true;}

	// TRX
	virtual Ogre::Vector3 GUIToWorld(const Ogre::Vector3 &p);
	virtual Ogre::Vector3 WorldToGUI(const Ogre::Vector3 &p);
	Animation *GetRootAnimation();
	AnimationPart *GetRootAnimationPart();

	virtual void InvalidateGfx(bool fromIK=false);
	void InvalidateGfxFromRoot();

	void PreRenderTargetUpdate();
	void PostRenderTargetUpdate();

};

class ANIMA_CORE_API ISceneObjectMovable 
{
public:
	//SceneObject *GetParent(){return NULL;}
	virtual Ogre::Vector3 &GetPos()=0;
	virtual void GetRot(float &_rot_xz)=0;
	virtual void SetPos(const Ogre::Vector3 &pos)=0;
	virtual void SetRot(float rot_xz)=0;
	virtual void GetLocalTrx(Ogre::Matrix4 &mat)=0;
	//virtual void InvalidateBuild(bool )=0;
};

class ANIMA_CORE_API SceneObjectMovable : public SceneObject , public ISceneObjectMovable
{
private:

	float rot_xz;
	Ogre::Vector3 pos;
	//Ogre::Quaternion rot;
	Ogre::Matrix4 trx;
	Ogre::Matrix4 inv_trx;
	float w_rot_xz;
	
	Ogre::SceneNode *main_node;
	Ogre::SceneNode *sub_node;

	bool boundChanged;
	bool trxChanged;
	bool firstTime;

protected:
	// bound
	Ogre::Vector3 pivot_center;

	Ogre::Vector3 bound_center;
	Ogre::Vector3 bound_min;
	Ogre::Vector3 bound_max;

protected:
	virtual void ComputeBound()=0;
	void EnsureBound();
	void EnsureTrx();
	//void BuildTrx();
	void OnGfxChanged();

	virtual void OnEnsureTrx(){}
	virtual void OnPreRenderTargetUpdate();

public:

	SceneObjectMovable(SceneObject *parent,int childTypeCount);
	//virtual ~SceneObjectMovable(void);
	Ogre::Vector3 &GetPos(){return pos;}

	Ogre::AxisAlignedBox GetBox(){return Ogre::AxisAlignedBox(bound_min,bound_max);}

	void GetRot(float &_rot_xz){ _rot_xz=rot_xz;}
	//Ogre::Quaternion GetRotMatrix();
	void GetLocalTrx(Ogre::Matrix4 &mat);
	void GetLocalInvTrx(Ogre::Matrix4 &mat);
	void GetTrx(Ogre::Matrix4 &mat);
	void GetInvTrx(Ogre::Matrix4 &inv_trx);
	void GetTrxRot(float &_rot_xz);
	void GetInvTrxRot(float &_rot_xz);

	void Copy(SceneObjectMovable *fromObj);

	Ogre::SceneNode *GetMainNode(){return sub_node ; }


	void _Set(const Ogre::Vector3 &pos,float rot_xz);
	void SetPos(const Ogre::Vector3 &pos);
	void SetRot(float rot_xz);

	void GetBound(Ogre::Vector3 &min,Ogre::Vector3 &max);
	Ogre::Vector3 GetBoundCenter();
	virtual void OnPathChanged();
	void GetBounds(Ogre::Vector3 bounds[]);

	virtual void InvalidateBuild(bool pathChanged)=0;
};

class ANIMA_CORE_API BulkSceneObject : public SceneObject
{
public:

	BulkSceneObject() : SceneObject(NULL,0){}
	virtual QImage Image();
//	virtual QColor GetColor();
	virtual QString GetName();
	virtual QString GetDesc(){return "";}
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);
};
#include "Anima_nsend.h"

