#pragma once

#include "SplineMovePath.h"
#include "StairMovePath.h"
#include "PointMovePath.h"
#include "Anima_nsbegin.h"

class ModelInstance;

class ANIMA_CORE_API LinkSplineMovePath: public SplineMovePath
{
friend class SplineMovePath;
friend class LinkSplineMovePathGfx;
private:
	SubSceneObjectGfx *gfx;

	SplineMovePath *movePath;
	ModelInstance *modelInstance;

	Ogre::Vector3 beginPoint;
	Ogre::Vector3 zeroPoint;

	void GetDir90(int fullPointIdx,Ogre::Vector3 &dirXZ,Ogre::Vector3 &dirXZ90);

public:
	LinkSplineMovePath(SplineMovePath *movePath,ModelInstance *modelInstance);
	virtual ~LinkSplineMovePath(void);

	virtual bool IsLink();
	SubSceneObjectGfx *GetLinkGfx(){return gfx;}

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);

	virtual MovePath *GetBasePath();
	virtual ModelInstance *GetModel(){return modelInstance; }
        virtual AObject::Type GetType(){return AObject::LinkSplineMovePathType;}
	virtual QString ClassName(){return "LinkSplineMovePath";}
	virtual QString GetDesc(){return "LinkSplineMovePath";}
	virtual DAG GetDAG();

	virtual MovePath *GetPrevPart();
	virtual MovePath *GetNextPart();

	//void FireChangedEvent();
	//void OnBasePathChanged();

	// IBuildElement

	virtual void Build();

//	virtual void DrawLine(bool selectMode,bool curveSelected,int pointIdxSelected,ModelInstance *modelInstance);
};

class ANIMA_CORE_API LinkStairMovePath: public StairMovePath
{
private:
	StairMovePath *movePath;
	ModelInstance *modelInstance;

public:
	LinkStairMovePath(StairMovePath *movePath,ModelInstance *modelInstance);
	~LinkStairMovePath(void);

	virtual MovePath *GetBasePath();
	virtual ModelInstance *GetModel(){return modelInstance; }
        virtual AObject::Type GetType(){return AObject::LinkStairMovePathType;}
	virtual DAG GetDAG();

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);

	virtual void Build();
};

class ANIMA_CORE_API LinkPointMovePath: public PointMovePath
{
friend class PointMovePath;
friend class ContextIO;
friend class PointMovePathGeom;
friend class AnimationPart;
friend class LinkPointMovePathGfx;

private:
	SubSceneObjectGfx *gfx;

	PointMovePath *movePath;
	ModelInstance *modelInstance;
public:
	PointMovePathGeom *geom;
//	PointMovePathLinkEntry *GetLinkOffset();

public:
	LinkPointMovePath(PointMovePath *movePath,ModelInstance *modelInstance);
	~LinkPointMovePath(void);

	SubSceneObjectGfx *GetLinkGfx(){return gfx;}
	virtual bool IsLink(){return true;}

	virtual MovePath *GetBasePath();
	virtual ModelInstance *GetModel(){return modelInstance; }
        virtual AObject::Type GetType(){return AObject::LinkPointMovePathType;}

	Ogre::Vector3 GetOffset();
	double GetUserAngleXZ();

	// bypass
	//virtual int Count(){return movePath->Count();}
	//virtual Ogre::Vector3 GetClipFixedOffset(){return movePath->GetClipFixedOffset(); }
	//virtual Ogre::Vector3 GetClipToOffset(){return movePath->GetClipToOffset(); }

	/*virtual double GetLinkFromAngleXZ(){return movePath->GetLinkFromAngleXZ(); }
	virtual double GetLinkToAngleXZ(){return movePath->GetLinkToAngleXZ(); }
	*/
	/*virtual bool GetFirstNormal(Ogre::Vector3 &normal){return movePath->GetFirstNormal(normal); }
	virtual bool GetLastNormal(Ogre::Vector3 &normal){return movePath->GetLastNormal(normal); }
	*/
	
	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);

	virtual Ogre::Vector3 GetWPosition(Ogre::Vector3 &clipPoint,int phase,double timePercent);

	virtual DAG GetDAG();
	//Ogre::Vector3 GetGeometryPivot();
	/*virtual bool Translate(Ogre::Vector3 &__dir,bool test=false);
	virtual bool Rotate(Ogre::Vector3 &pivot,double axz);*/
	//void MovePoint(int idx,const Ogre::Vector3 &p);

	virtual void Build();
};

// ====================================================

class LinkSplineMovePathGfx: public SubSceneObjectGfx
{
	friend class LinkSplineMovePath;

protected:
		
	LinkSplineMovePath *linkSplineMovePath;
public:

	LinkSplineMovePathGfx(LinkSplineMovePath *linkSplineMovePath);
	virtual ~LinkSplineMovePathGfx();

	LinkSplineMovePath *GetLinkSplineMovePath(){return linkSplineMovePath;}
	virtual QString GetDesc(){return "LinkSplineMovePath";}

	virtual AObject::Type GetType(){return AObject::LinkSplineMovePathType;}
};
class LinkPointMovePathGfx: public SubSceneObjectGfx
{
	friend class LinkPointMovePath;

protected:
		
	LinkPointMovePath *linkPointMovePath;
public:

	LinkPointMovePathGfx(LinkPointMovePath *linkPointMovePath);
	virtual ~LinkPointMovePathGfx();

	LinkPointMovePath *GetLinkPointMovePath(){return linkPointMovePath;}
	virtual QString GetDesc(){return "LinkPointMovePath";}

	virtual AObject::Type GetType(){return AObject::LinkPointMovePathType;}
};


#include "Anima_nsend.h"
