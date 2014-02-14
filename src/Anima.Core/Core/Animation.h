#pragma once

#include "SceneObject.h"
#include "SceneLibrary.h"
#include "SceneObjectMaterial.h"
#include "AnimationEvent.h"

//class Qt::QPoint;

#include "Anima_nsbegin.h"
class AnimationPart;
class AnimClip;
class Model;
class ModelInstance;
class FilmLayer;
class AnimationPartClipSpline;
class AnimationPartStairs;
class MovePath;
class ContextIO;
class Clip;
class BaseModel;
class CoreFunction;
class AnimClipModel;
class ModderCollection;
class ActorContainer;
class AnimationPartContainer;

struct ANIMA_CORE_API AnimationPoint
{
public:

	int animationPartIdx;
	int pointIdx;

	AnimationPoint()
	{
		animationPartIdx=-1;
		pointIdx=-1;
	}
	AnimationPoint(int animationPartIdx,int pointIdx)
	{
		this->animationPartIdx=animationPartIdx;
		this->pointIdx=pointIdx;
	}
};

class ANIMA_CORE_API Animation : public SceneObjectMovable , public SceneObjectMaterial  , public SceneObjectUI, public SceneBlockElement
{
	friend class ContextIO;
	friend class FilmLayer;
	friend class AnimationPart;
	friend class RenameCommand;
	friend class MovePath_BuildDispatcher;

private:

//	long part_id;

	FilmLayer *filmLayer;

	double startTime;
	QList<AnimationPart *> animationPartList;
	QHash<Model *,int> currentModels;
	QList<ModelInstance *> modelInstanceList; // stesso gruppo
	QString Name;
	QColor Color;
	ModderCollection *modders;

	// GUI
	ActorContainer *actorContainer;
	AnimationPartContainer *animationPartContainer;

	// performance
	int modelCountFullAssign;
	bool buildDispatcherPathChanged;
	// gfx
	bool cutYLevelEnabled;
	double cutYLevel;

private:
	void OnAddModel(ModelInstance *mi);
	void OnRemoveModel(ModelInstance *mi);
	virtual void OnPostRenderTargetUpdate();
	bool CanInsertSpline(AnimationPart *before , AnimationPart *after );
	virtual void OnEnsureTrx();

protected:
	virtual void ComputeBound();
	
public:
	Animation(FilmLayer *filmLayer);
	~Animation(void);

	virtual AObject::Type GetType(){return AObject::AnimationType;}
	QString ClassName(){return "Animation";}

	//virtual SceneObject *GetParent();
	virtual AObject::Type GetChildType(int idx);
	virtual int GetChildCount(AObject::Type type);
	virtual SceneObject *GetChild(AObject::Type type,int idx);

	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);
	void OnLoad();

	QImage Image(){return ResourceManager::Singleton.item_animation;}

	QString GetName(){return Name;}
    void SetName(const QString &name);
	QString GetDesc(){return "";}

	ModderCollection *GetModders(){return modders;}

    //returns next child object default name
    //i.e. for spline objects prefix == "SPLINE", return values: "SPLINE 1", "SPLINE 2" etc
    QString GetNextDefaultChildName(const QString & prefix);

	void GetBaseModels(QList<ModelInstance *> &list,bool includeCloneLocked);
	void GetCloneModels(QList<ModelInstance *> &list);
	QList<ModelInstance *> &GetModels(){return modelInstanceList;}
	FilmLayer *GetFilmLayer(){return filmLayer;}

	QString GetModelName(const QString &baseName);

	void InsertAnimationPart(AnimationPart *part,int idx=-1);
	void InvertModels(); // perator
	/*bool GetPlayLoop();
	void SetPlayLoop(bool loop);*/
	bool IsOnlyFixed();
	bool IsLastFixed();

	double StartTime(){return startTime;}
	void SetStartTime(double startTime);
	double EndTime();

	double StartFrame();
	double EndFrame();

	double PathLen();
	//double TotalLen();
	double FrameCount();

	// models
	void ClearModels();
	void Clear();

	// deve essere dello stesso gruppo del precedente
	ModelInstance *AddModel(Model *model,ModelInstance *baseModel=NULL,int pos=-1); 
	void _AddModel(ModelInstance *modelInstance,int pos=-1); 
	ModelInstance *ReplaceModel(ModelInstance *oldModel,Model *model); 
	void DeleteModel(ModelInstance *model,bool checkForEmpty); 
	void DeleteModels(QList<ModelInstance *> &modelList); 
	ModelInstance *GetModel(const QString &name); 
	int GetModelIndex(ModelInstance *instance); 
	
	void ClearPaths();

	int GetAnimationPartIndex(AnimationPart *part);
	AnimationPart *GetAnimationPart(int partIdx);
	AnimationPart *GetAnimationPart(const QString &name);
	int AnimationPartCount(){return animationPartList.count();}
	QList<AnimationPart *> &GetAnimationParts(){return animationPartList;}

	void EnsureValidPaths();
	bool CanAddAnimationPart(AObject::Type part_type,int idx=-1); // -1 = append
	bool CanDeleteAnimationPart(int idx);
	void DeleteAnimationPart(AnimationPart *part);
	bool RenameAnimationPart(AnimationPart *part,const QString &newName);
	void MoveAnimationPartPos(int idx,int toIdx);

	void GetAnimClipList(QList<AnimClip *> &clipList);
	void GetAnimClipModelList(ModelInstance *model,QList<AnimClipModel *> &clipList);

	// merge / split
	bool CanMerge(int idx,int idxOffset); // idxOffset>0
	bool CanSplit(int idx);
	void MergePart(int idx,int idxOffset);
	void SplitPart(int idx);

	// ritorna la 
	bool PickPoint(float point[3],float maxDistance,float *dist,int *pointIdx);

//	void Invalidate();
	//void RebuildModels();
	virtual void InvalidateBuild(bool pathChanged);
	void _InvalidateBuild(bool pathChanged);
	// ---------------

	ActorContainer *GetActorContainer(){return actorContainer;}
	AnimationPartContainer *GetAnimationPartContainer(){return animationPartContainer;}

	void OnTimerTick();

	void OnFPSChanged();
	void InvalidateGfxFromIK(BaseModel *model);

	// performance

	int GetModelCountFullAssign(){return modelCountFullAssign;}
	int AssignMaxModel(int maxModelCount);

	// GFX
	void SetCutYLevel(double cutYLevel);
	double GetCutYLevel(){return cutYLevel;}
	bool IsCutYLevelEnabled();
	void ClearCutYLevel();
	
	// IO
	void Save(QDomDocument &doc,QDomElement &ele,bool onlyPath=false );
	void Load(QDomElement &animEle );
	void Load(QDomElement &ele,QString &importMessage  );
	static Animation *Load(FilmLayer *layer,QDomElement &ele );

	virtual QString ToString();

};

class ANIMA_CORE_API ActorContainer : public SceneObject
{
private :
	QColor color;
	Animation *anim;
public:

	ActorContainer(Animation *anim);
	//virtual SceneObject *GetParent(){return (SceneObject*)anim;}

	Animation *GetAnimation(){return anim;}

	virtual DAG GetDAG(){return DAG("ActorContainer");}
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	virtual QImage Image(){return ResourceManager::Singleton.item_man;}
	virtual QColor GetMaterialColor(){return anim->GetMaterialColor();}
	virtual QString GetName(){return "";}
    virtual QString ClassDesc(){return "ACTORS:";}
	virtual QString ClassName(){return "ActorContainer";}
};

class AnimationPartContainer : public SceneObject
{
private :
	Animation *anim;
	QColor color;
public:
	AnimationPartContainer(Animation *anim): SceneObject(NULL,0),anim(anim){color.setRgb(190,65,63); parent = (SceneObject*)anim;}

	Animation *GetAnimation(){return anim;}

	//virtual SceneObject *GetParent();
	virtual DAG GetDAG(){return DAG("AnimationPartContainer");}
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	virtual QImage Image(){return ResourceManager::Singleton.item_path;}
	virtual QColor GetMaterialColor(){return anim->GetMaterialColor();}
	virtual QString GetName(){return "";}
    virtual QString ClassDesc(){return "PATH:";}
	virtual QString ClassName(){return "AnimationPartContainer";}
};

#include "Anima_nsend.h"