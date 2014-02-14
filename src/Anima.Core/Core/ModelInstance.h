#pragma once

#include "SceneObjectGfx.h"
#include "SceneObjectMaterial.h"
#include "BuildDispatcher.h"

#include "Anima_nsbegin.h"

class Model;
class Animation;
//class CoreFunction;
class AnimationPart;
class MovePath;
class ModelOverlayFunction;
class GfxModel;
class ClipBlendAssign;
class ModelMesh;
class ModelInstanceEdit;
//class KeySpeedFunction;
class SpeedFunction;
class ModelIAActor;

class ModelInstanceBuildInfo: public BObject
{
private:
	ModelInstance *mi;
	bool path_invalid;
	bool clip_invalid;
public:
	ModelInstanceBuildInfo(ModelInstance *mi):mi(mi),path_invalid(true),clip_invalid(true)
	{
	}
	bool NeedBuildPath();
	bool NeedBuildClip();
	void InvalidatePath(MovePath *path);
	void InvalidateClip();
	void InvalidateAll();
	void ValidatePath();
	void ValidateClip();
};

class ANIMA_CORE_API ModelInstance : public SceneObjectGfx , public SceneObjectMaterial //, public IBuildElement
{
friend class ContextIO;
friend class RenameCommand;
friend class GfxModelInstanceRender;
friend class ClonerModder;
friend class BuildDispatcher;
friend class ModelMesh;
friend class Animation;
friend class ModelIAActor;
//friend class AnimPhysicController;

private:

	Animation *anim;
	ModelIAActor *actor;
	QString name;
	Model *model;
	ModelInstanceEdit *edit;
	static int _id;
	int id;

	//QColor Color;
	QList<Ogre::AnimationState *> activeStateList;
	QList<Ogre::AnimationState *> activeOverlayStateList;
	ModelInstance *masterModel;
	bool autoLocked;
	bool isGhost; // usato per il defautl model

	SpeedFunction *speedFunction;
	ModelOverlayFunction *overlayFunction;
	double phaseTime;
	bool inverseDirection;
	//int loopCount;
	double startSpace;
	double startDelay;
	//bool playLoop;
	double startTime;
	int playTimeAutoLoopCount;

	// gfx

	Ogre::Entity *entity;
	Ogre::Vector3 position;
	Ogre::Quaternion orientation;
	Ogre::Vector3 direction;
	double pathSpace;
	MovePath *lastModelPath;
	MovePath *lastBasePath;
	AnimationPart *lastPart;
	ClipBlendAssign *lastAssign;
	double lastTime;
	bool isReady;
	ModelMesh *modelMesh;
//	bool isLastFixed;
	
	ModelInstanceBuildInfo *build;
	QString storageOverlaySerial;

public:
	Ogre::Vector2 offsetPosition;
private:
	
public:
	ModelInstance(Animation *anim,Model *model,ModelInstance *masterModel=NULL);
	//ModelInstance(Animation *anim,QStringList &ser);
	~ModelInstance(void);
	int GetID(){return id;}
	void EnsureUnicity();

	ModelInstanceBuildInfo *GetBuild(){return build;}

	void Clear();
	virtual AObject::Type GetType(){return AObject::ModelInstanceType;}
	virtual QString ClassName(){return "ModelInstance";}
	ModelIAActor *GetActor(){return actor;}

	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);

	static ModelInstance *Load(Animation *anim,QDomElement &ele,QString &importMessage,bool fromImport,int pos=-1);
	void Load(QDomElement &ele,bool getID);
	void Save(QDomDocument &doc,QDomElement &filmAnimEle,bool saveTrx ,bool saveCollision );

	GfxModel *GetGfxModel();
	bool IsReady(){return isReady;}
	void SetIsReady(bool isReady);

	ModelInstanceEdit *GetEdit(){return edit;}

	void SetGhost(){isGhost=true;}
	bool IsGhost(){return isGhost;}
	bool HaveModder();
	ModelInstance *GetMasterModel(){return masterModel;}
	bool IsAutoModel(){ return HaveModder() && masterModel!=NULL;}
	bool IsAutoLocked();
	void Lock();
	void UserLock();
	void UnLock();
	void SwitchToBase();

	double GetPhaseTime(MovePath *path);
	void SetPhaseTime(double phaseTime,bool fireEvent=true);

	inline bool GetInverseDirection(){return inverseDirection;}
	void SetInverseDirection(bool inverseDirection);
	/*int GetLoopCount();
	void SetLoopCount(int loopCount);*/
	double GetRelativeOffset();
	void SetRelativeOffset(double value);
	double GetStartSpace();
	void SetStartSpace(double value);
	double GetStartDelay();
	void SetStartDelay(double time);

	bool IsLastFixed();
	/*bool GetPlayLoop();
	void SetPlayLoop(bool loop);*/
	//int GetLastAutoLoopCount(double totalTime);

	double GetStartTime(); 
	void SetStartTime(double time);

	Animation *GetAnimation(){return anim;}
	Model *GetModel(){return model;}
	MovePath *GetModelMovePath(){return lastModelPath;}
	MovePath *GetModelBasePath(){return lastBasePath;}
	AnimationPart *GetLastAnimationPart(){return lastPart;}
	ClipBlendAssign *GetClipBlendAssign(){return lastAssign;}
	void ClearLastAssign(){lastAssign=NULL;}
	double GetLastTime();//{return lastTime;}
	double GetPathLen();
    
	double GetPathSpace(){return pathSpace;}
	Ogre::Vector3 GetDirection(){return direction;}

	double GetPlayTime(double globalTime);
	double GetBuildTime(double globalTime);
	double CurrentBuildTime();
	double CurrentPlayTime();

	SpeedFunction *GetSpeedFunction(){return speedFunction;}
	ModelOverlayFunction *GetOverlayFunction(){return overlayFunction;}

	QImage Image(){return ResourceManager::Singleton.item_man;}

	QString GetName(){return name;}
	QString GetFullName();
	QString GetDesc(){return name;}
	void SetName(const QString &name);

	void OnInsertAninPart(AnimationPart *animPart);
	void OnDeleteAninPart(AnimationPart *animPart);
	virtual void OnEnabledChanged();

	QString PopStorageOverlaySerial(){ QString s =storageOverlaySerial; storageOverlaySerial=""; return  s;}

	virtual void OnPreRender();
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);
	virtual void OnPostRenderTargetUpdate();

	virtual bool IsRenderVisibleFromRoot();

	// ------
	// a seconda delal modalità ritorna l'entità di base o quella privata
	Ogre::Entity *GetEntity();
	ModelMesh *GetModelMesh(){return modelMesh;}

	bool HasAccess(SceneLibraryElementAccess access);

	// GFX
	void UpdateGfx(double frame);
	virtual Ogre::Vector3 GetPosition(){return position;}
	virtual Ogre::Quaternion GetOrientation(){return orientation;}

};

//class ModelInstanceBaseGfx: public SubSceneObjectGfx
//{
//protected:	
//
//public:
//
//	ModelInstanceBaseGfx(ModelInstance *modelInstance);
//	virtual ~ModelInstanceBaseGfx(){}
//    void ModelInstanceBaseGfx::SetMaterial(const QString &mat);
//};
//
class ModelInstanceMoveGfx: public SubSceneObjectGfx
{
protected:
		
	ModelInstance *modelInstance;

public:

	ModelInstanceMoveGfx(SceneObjectGfx *obj);
	virtual ~ModelInstanceMoveGfx(){}

	void Set(ModelInstance *modelInstance);
	ModelInstance *GetModelInstance(){return modelInstance;}
	virtual QString GetDesc(){return "ModelInstanceMoveGfx";}
	virtual AObject::Type GetType(){return AObject::ModelInstanceMoveGfxType;}
    void SetMaterial(const QString &mat);
};

class ModelInstanceAdminGfx: public SubSceneObjectGfx
{
protected:
		
	ModelInstance *modelInstance;

public:

	ModelInstanceAdminGfx(SceneObjectGfx *obj);
	virtual ~ModelInstanceAdminGfx(){}

	void Set(ModelInstance *modelInstance);
	ModelInstance *GetModelInstance(){return modelInstance;}
	virtual QString GetDesc(){return "ModelInstanceAdminGfx";}
	virtual AObject::Type GetType(){return AObject::ModelInstanceMoveGfxType;}
};


#include "Anima_nsend.h"
