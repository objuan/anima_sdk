#pragma once

#include "SceneObject.h"
#include "SceneLibrary.h"
#include "Animation.h"
#include "BuildDispatcher.h"

#include "Anima_nsbegin.h"
class Clip;
class LibClip;
class AnimClip;
class SplineMovePath;
class Animation;
class MovePath;
class CoreFunction;
class ModelInstance;
class MovePathStorage;
class AnimClipCollection;
class Model;
//class ModelInstanceIAPath;

//class ANIMA_CORE_API IAnimationPartClipDynamic
//{
//public:
//
//	virtual AnimClip *InsertClip(LibClip *clip,int idx)=0;
//	virtual AnimClip *InsertClip(LibClip *clip,int idx,double size)=0;
//	virtual AnimClip *AddClip(LibClip *clip)=0;
//	virtual void DeleteClip(int idx)=0;
//	//virtual void InsertAnimClip(int idx,AnimClip *clip)=0;
//
//	virtual bool MoveClip(int idx,double startOffset,double endOffset)=0;
//	virtual void MoveClipPos(int idx,int toIdx)=0;
//
//	virtual	bool CanMerge(int idx,int idxOffset)=0; // idxOffset>=1
//	virtual bool CanSplit(int idx)=0;
//	virtual void MergeClip(int idx,int idxOffset)=0; // idxOffset>=1
//	virtual void SplitClip(int idx)=0;
//
//};

enum AnimationPartMode
{
	AnimationPartMode_AutoSplineLock = 1
};
struct AnimationPartModeInfo 
{
	AnimationPartMode mode;
	bool enabled;
};
class ANIMA_CORE_API AnimationPart : public SceneObject, public SceneBlockElement
{
	friend class ContextIO;
	friend class MovePath;
	friend class Animation;

protected:
	Animation *animation;
	AnimClipCollection *clipList;
	QList<AnimationPartModeInfo> modeList;  //

protected:

	void FireParamsChanged();
	bool _UpdateClipBound();
	int GetModeIdx(AnimationPartMode mode); 
	//void GetModelSelected(QList<SceneObject *> &selList);

public:
	AnimationPart(Animation *animation);
	virtual ~AnimationPart();

	virtual AObject::Type GetType(){return AObject::AnimationPartType;}
	virtual QString ClassName(){return "AnimationPart";}
	virtual QImage Image(){return ResourceManager::Singleton.item_color;}
	//virtual QColor GetColor(){return Color;}
  //  virtual void SetColor(QColor col) {Color = col; UpdateMaterial();}
	virtual QString GetName();
	virtual QString GetDesc();
   // virtual QString GetMaterial() {return materialName;}
	virtual DAG GetDAG();

	int GetIdx();

	void SetMode(AnimationPartMode mode,bool enable);
	void ClearMode(AnimationPartMode mode);
	int IsModeEnabled(AnimationPartMode mode); // 1 = enabled , 0 = disabled = -1 = non used

	virtual QColor GetMaterialColor(){return animation->GetMaterialColor();}

  //  void UpdateMaterial();

	//virtual SceneObject *GetParent();
	virtual AObject::Type GetChildType(int idx);
	virtual int GetChildCount(AObject::Type type);
	virtual SceneObject *GetChild(AObject::Type type,int idx);

	Animation *GetAnimation(){return animation;}
	AnimClipCollection *GetAnimClips(){return clipList;}
	//virtual void Detach();
	
	virtual bool HavePath(){return true;};
	virtual void OnLoad(){}

	// parzial

	/*double StartTime(ModelInstance *modelInstance);
	double StartFrame(ModelInstance *modelInstance);
	double StartSpace(ModelInstance *modelInstance);*/

	// total
	//double TotalLen();
	//double FrameCount(ModelInstance *modelInstance);

	// clip

	//bool IsClipValid();
	//virtual void OnClipChanged(bool notify)=0;
	//virtual void OnAnimClipsBeforeChanged(AnimationPart *part){}

	int ClipCount();
	AnimClip *GetClip(int idx);

	bool HaveNext(ModelInstance *model);
	bool HavePrev(ModelInstance *model);
	AnimationPart *GetPrevPart(ModelInstance *model);
	AnimationPart *GetNextPart(ModelInstance *model);

	AnimClip *GetLastClip();
	AnimClip *GetFirstClip();

	int GetClipIdx(AnimClip *animClip);
	//double GetClipTime(int idx,ModelInstance *modelInstance);
	//double GetClipFrames(int idx,ModelInstance *modelInstance);
	//double GetClipStartFrame(int idx,ModelInstance *modelInstance);
	//double GetClipStartLen(int idx,ModelInstance *modelInstance=NULL);
	//double GetClipEndLen(int idx,ModelInstance *modelInstance=NULL);
	//double GetClipLen(int idx,ModelInstance *modelInstance=NULL);

	QColor GetClipColor(int idx);
	//QString GetClipMaterial(int idx);
	QString GetClipDesc(int idx);
	bool IsClipDynamic(int idx);
	bool IsClipPose(int idx);
	bool IsClipLink(int idx);
//	bool IsClipValid(int idx);

	// path

	virtual bool CanMove(int partIdx,int pointIdx);
	virtual MovePath *GetPath(ModelInstance *modelInstance=NULL)=0; 

	virtual bool GetLastPoint(Ogre::Vector3 &v,int backCounter = 0);
	virtual bool GetFirstPoint(Ogre::Vector3 &v,int foreCounter = 0);

	virtual void OnStartTimeChanged(){}

	//virtual void InvertPath(); // operator

	// models

	virtual void OnAddModel(ModelInstance *mi);
	virtual void OnRemoveModel(ModelInstance *mi);
	//virtual void OnChangedModel(ModelInstance *mi);

	virtual bool IsRenderVisibleFromRoot();

	// gfx

	//virtual void NotifyEvent(AnimationEvent &evt );
	virtual void OnFPSChanged(){}

	void UpdateGfx(ModelInstance *modelInstance,double frameOffset);

	//void DrawFrame(ModelInstance *modelInstance,bool selectMode,double frameOffset);
	//virtual void Draw(bool selectMode)=0;
	virtual QString ToString();
	virtual QString ToString(ModelInstance *modelInstance);

	// IO
	
	void Save(QDomDocument &doc,QDomElement &ele );
	//void Load(QDomElement &ele,QList<Ogre::Vector3> &lastPathPoints);
	static AnimationPart *Load(Animation *anim,QDomElement &animPartEle,int pos=-1);
//	static AnimationPart *Load(Animation *anim,QDomElement &ele,QList<Ogre::Vector3> &lastPathPoints);

//	virtual void AddPathInfo(ModelInstance *modelInstance,ModelInstanceIAPath *path);

protected:

	
	virtual void UpdateGfx(ModelInstance *modelInstance,int subPart,double startSpace,double frameOffset)=0;
};
#include "Anima_nsend.h"
