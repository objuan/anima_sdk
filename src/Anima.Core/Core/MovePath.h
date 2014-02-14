#pragma once

#include "../Anima.Core.h"
#include "SceneObjectGfx.h"
#include "BuildDispatcher.h"

#include "Anima_nsbegin.h"

class AnimationPart;
class MovePathPoint;
class MovePath;
class AnimClipBound;
class Clip;
class AnimClip;
class ModelInstance;
class GeomExtCollection;
class MovePathTrx;

#define MOVEPATH_LINK_GROUP 0
#define MOVEPATH_POINT_GROUP 1
#define MOVEPATH_START_OFFSET_GROUP 2
#define MOVEPATH_MID_OFFSET_GROUP 3
#define MOVEPATH_ANIM_BOUND_GROUP 4
#define MOVEPATH_EXTRA_GEOM_GROUP 5

enum SplinePointInfo_TYPE
{
	SplinePointInfo_TYPE_BASE = 0,
	SplinePointInfo_TYPE_INTERNAL,
	SplinePointInfo_TYPE_EXTERNAL
};

struct SplinePointInfo
{
public:

	Ogre::Vector3 pos;
	Ogre::Vector3 tangent;
	SplinePointInfo_TYPE type;

	SplinePointInfo(){}
		
	SplinePointInfo(const SplinePointInfo &info) 
	{
		pos=info.pos;
		tangent=info.tangent;
		type = info.type;
	}

	SplinePointInfo(const Ogre::Vector3 &pos,SplinePointInfo_TYPE type) : pos(pos),type(type)
	{
		tangent = Ogre::Vector3::ZERO;
	}
		
	SplinePointInfo(const Ogre::Vector3 &pos,const Ogre::Vector3 &tangent,SplinePointInfo_TYPE type) : pos(pos),tangent(tangent),type(type)
	{
	}

};

class ANIMA_CORE_API MovePath: public SceneObjectGfx, public IMovePath_BuildElement
{
protected:
	AnimationPart *animationPart;
	bool modifyEnabled;
    QString name;
	bool _isChanged;

private:

	void _Copy(SceneObject *fromObj,bool notify);

protected:

	virtual MovePathTrx *GetTrx()=0;

public:

	MovePath(AnimationPart *animationPart, const QString & name = QString("Unnamed"));
	virtual ~MovePath();

	AnimationPart *GetAnimationPart(){return animationPart;}

	bool PopIsChanged();

	MovePathTrx *GetPathTrx(){return GetTrx();}

	virtual AObject::Type GetType(){return AObject::MovePathType;}
	virtual QString ClassName(){return "MovePath";}
//	virtual SceneObject *GetParent();
	virtual int GetChildCount(AObject::Type type);
	virtual SceneObject *GetChild(AObject::Type type,int idx);

	virtual QImage Image(){return ResourceManager::Singleton.item_color;}
	virtual QString GetName(){return name;}
    virtual void SetName(const QString &n);
	virtual QString GetDesc(){return "";}
  //  virtual QColor GetColor(){return Qt::gray;}
	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj);
	virtual SceneObject *Clone(SceneObject *newParent);
	virtual MovePath *GetBasePath();

	virtual MovePath *GetPrevPart();
	virtual MovePath *GetNextPart();
	
	// virtuali astratte
	virtual bool GetInfoAtDistance(int *idx,double distance,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir)=0;
	virtual void ProjectPoint(Ogre::Vector3 &p,Ogre::Vector3 &minCurvePoint,double &minSpace,double &minTime,Ogre::Vector3 &dir,double &error)=0;
	virtual double GetDistanceAtTime(double time)=0; // time - (0 - 1)

	virtual bool IsValid(){return true;}
	virtual double Length()=0;
	virtual double Height()=0;
	virtual double GetClipCount(){return -1;} // nomero di clip che fanno il path, -1 se non e' importante
	virtual double GetClipW(){return -1;}
	virtual double GetClipH(){return -1;}
	virtual bool GetFirstNormal(Ogre::Vector3 &normal)=0;
	virtual bool GetLastNormal(Ogre::Vector3 &normal)=0;
	virtual bool GetFirstPoint( Ogre::Vector3 &p,int offset=0);
	virtual bool GetLastPoint( Ogre::Vector3 &p,int offset=0);

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint){return false;}
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint){return false;}

	// control points
	virtual Ogre::Vector3 GetPoint(int idx);
	virtual int Count();
	virtual int AddPoint(const Ogre::Vector3 &p,int idx=-1);
	//virtual void MovePointUI(int idx,const Ogre::Vector3 &p);
	virtual void MovePoint(int idx,const Ogre::Vector3 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear();
	//virtual void Invert(){}

	// 
	bool IsPathControlsVisible();
	Ogre::Vector3 CheckPointAddDistance(const Ogre::Vector3 &_point);

	virtual bool IsRenderVisibleFromRoot();

	// anim bound

	void BeginModify();
	void EndModify(bool fireEvents=true);

	virtual void InvalidateBuild(int type=0);

	//virtual bool Translate( Ogre::Vector3 &dir,bool test=false);
	//virtual bool Rotate(Ogre::Vector3 &pivot,double axz);

	virtual bool HasAccess(SceneLibraryElementAccess access);

	virtual void OnPreRender();
	virtual void GfxBound(Ogre::Vector3 &min,Ogre::Vector3 &max);

	virtual void OnModelRename(ModelInstance *mi,const QString &name){}

	// UI
	virtual void GetAnchor(Ogre::Vector3 &pivot,float &angleXZ,float &dimension)=0;
	virtual void GUINotifyAddPoint(Ogre::Vector3 &p){}

	// distance,height
	virtual void GetFootOffset(QList<Ogre::Vector2> &footPosition,int &startFootCount){} 

	//
	virtual void ExportGeometry(MeshExporter *,GeomExtCollection &extList){}

};

class MovePathPoint: public SubSceneObjectGfx
{
	friend class SplineMovePath;

protected:
		
	MovePath *movePath;
	MovePath *link_movePath;
	//int idx;
	bool effectActive;

public:

	MovePathPoint(MovePath *movePath);
	virtual ~MovePathPoint();

	virtual QString ClassName(){return "MovePathPoint";}
	virtual QString GetDesc(){return "Path Point";}

	MovePath *GetMovePath();
	void Assign(MovePath *movePath);

	int GetIdx();
	Ogre::Vector3 GetWorldPosition();
	Ogre::Vector3 GetGUIPosition();

	virtual AObject::Type GetType(){return AObject::MovePathPointType;}

	virtual bool HasAccess(SceneLibraryElementAccess access);

	void SetEffect(bool active);
};

class AnimClipBound: public SubSceneObjectGfx
{
	friend class SplineMovePath;

protected:
		
	MovePath *movePath;
	AnimClip *prevClip;
	AnimClip *nextClip;
	bool isLast;

public:

	AnimClipBound(MovePath *movePath);
	virtual ~AnimClipBound();

	MovePath *GetMovePath(){return movePath;}
	virtual AObject::Type GetType(){return AObject::AnimClipBoundType;}
	virtual QString ClassName(){return "AnimClipBound";}
	virtual QString GetDesc(){return "Clip Bound";}

//	bool IsValid();
	void Set(AnimClip *prevClip,AnimClip *nextClip,bool isLast);

	virtual Ogre::Vector3 GetLabelGUIPosition();
};

class MovePathStartOffset: public SubSceneObjectGfx
{
	friend class SplineMovePath;

protected:
		
	MovePath *movePath;
	ModelInstance *mi;

public:

	MovePathStartOffset(MovePath *movePath);
	virtual ~MovePathStartOffset();

	void Set(ModelInstance *mi);
	ModelInstance *GetModelInstance(){return mi;}
	MovePath *GetMovePath(){return movePath;}
	virtual QString GetDesc(){return "Path Start Offset";}

	virtual AObject::Type GetType(){return AObject::MovePathStartOffsetType;}
};

class MovePathMidOffset: public SubSceneObjectGfx
{
	friend class SplineMovePath;

protected:
		
	MovePath *movePath;
	ModelInstance *mi;
	int markerIdx;
	double space;
	double spacePerc;
	double val;

public:

	MovePathMidOffset(MovePath *movePath);
	virtual ~MovePathMidOffset();

	void Set(int markerIdx,double space,double spacePerc,double val,ModelInstance *mi);

	MovePath *GetMovePath(){return movePath;}
	double GetSpacePerc(){return spacePerc;}
	double GetSpace(){return space;}
	double GetVal(){return val;}
	int GetMarkerIdx(){return markerIdx;}
	ModelInstance *GetModelInstance(){return mi;}

	virtual QString GetDesc(){return "Path Mid Offset";}

	virtual AObject::Type GetType(){return AObject::MovePathMidOffsetType;}
};


class MovePathRotate: public SubSceneObjectGfx
{
protected:
		
	MovePath *movePath;
	Ogre::Vector3 O;
	Ogre::Vector3 T;

public:

	MovePathRotate(SceneObjectGfx *obj);
	virtual ~MovePathRotate();

	MovePath *GetMovePath(){return movePath;}
	void Set(MovePath *movePath,Ogre::Vector3 &O,Ogre::Vector3 &T);

	virtual void OnPreRenderTargetUpdate();

	virtual QString GetDesc(){return "Path Rotate";}

	virtual AObject::Type GetType(){return AObject::MovePathRotateType;}
};

class MovePathTranslate: public SubSceneObjectGfx
{
protected:
		
	MovePath *movePath;
	Ogre::Vector3 O;
	Ogre::Vector3 T;

public:

	MovePathTranslate(SceneObjectGfx *obj);
	virtual ~MovePathTranslate();

	MovePath *GetMovePath(){return movePath;}
	void Set(MovePath *movePath,Ogre::Vector3 &O,Ogre::Vector3 &T);
	
	virtual void OnPreRenderTargetUpdate();

	virtual QString GetDesc(){return "Path Translate";}

	virtual AObject::Type GetType(){return AObject::MovePathTranslateType;}
};



#include "Anima_nsend.h"
