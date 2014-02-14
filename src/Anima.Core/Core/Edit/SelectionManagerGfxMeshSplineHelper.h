#pragma once

#include "Core/SceneObjectGfx.h"
//#include <OgrePanelOverlayElement.h>
//#include <OgreOverlayElementFactory.h> 

#include "Anima_nsbegin.h"

class DynamicLines;
class SceneObjectMovable;
class SplineMovePath;
class ClonerModder;
class SelectionManagerGfxManualMesh;
class SelectionManagerGfxSplineHelp;
class Point_PathManipolator;

enum SplineMode
{
	SplineMode_Waiting = 0,
	SplineMode_None,
	SplineMode_Add,
	SplineMode_Del,
	SplineMode_Move
};

// manager delle spline

class SelectionManagerGfxMultiSplineHelp: public SubSceneObjectGfx
{
private:
	SelectionManagerGfxSplineHelp *nullSplineHelper;
	QList<SelectionManagerGfxSplineHelp *> splineList;
public:

	SelectionManagerGfxMultiSplineHelp(SceneObjectGfx *obj);
	virtual ~SelectionManagerGfxMultiSplineHelp();

	void StartMove();
	void EndMove();

	SelectionManagerGfxSplineHelp *GetSplineHelper(SplineMovePath *path);
	SelectionManagerGfxSplineHelp *NewSplineHelper(SplineMovePath *path);
	void DeleteSplineHelper(SelectionManagerGfxSplineHelp *spline);

	virtual void OnPreRenderTargetUpdate();
};

class SelectionManagerGfxSplineHelpDependence
{
public:
	Point_PathManipolator* point;
	bool isPrev;
};

class SelectionManagerGfxSplineHelp : public BObject
{
private:
	SelectionManagerGfxSplineHelpDependence *dependence;

	SelectionManagerGfxMultiSplineHelp *multiSplineHelp;

	SelectionManagerGfxManualMesh *mesh;
	SplineMovePath	*obj;
	SplineMode mode;
	Ogre::ManualObject *mo;

	Ogre::Vector3 lastAddPoint;
	int lastInsertPointIdx;
	int lastDelPointIdx;

private:
	bool BuildSpline(QList<Ogre::Vector3> &pointList,QList<int> &typeList,QList<Ogre::Vector3> &tanList,QList<int> &chechIdx);
	void AddSegment(Ogre::Vector3 vertex,Ogre::Vector3 lastPt,int &segmentCount,float *v ,int &vCount,Ogre::Real roadWidth);
	void BuildIdx(int segmentCount,size_t *idx,int &idxCount);
	void CheckForAxe(QList<Ogre::Vector3> &pointList,int i,Ogre::Vector3 *gui_point);

public:
	
	SelectionManagerGfxSplineHelp(SelectionManagerGfxMultiSplineHelp *multiSplineHelp,SelectionManagerGfxManualMesh *mesh);
	virtual ~SelectionManagerGfxSplineHelp();

	void SetDependence(Point_PathManipolator *point,bool isPrev);
	void UpdateDependences();

	void Begin(SplineMovePath *obj);
	bool TryMove(const Ogre::Vector3 &offset,double angle);
	void TryAddDelete(const Ogre::Vector3 point);
	SplineMode GetMode(){return mode;}
	bool CanDoOperation(){return mode != SplineMode_None && mode != SplineMode_Waiting;}
	void End();

	Ogre::Vector3 GetAddPoint(){return lastAddPoint;}
	int GetInsertPointIdx(){return lastInsertPointIdx;}
	int GetDelPointIdx(){return lastDelPointIdx;}

	SplineMovePath *GetSplineMovePath(){return obj;}
	
	virtual QString GetDesc(){return "SelectionManagerGfxSplineHelp";}
};

class SelectionManagerGfxManualMesh : public SceneObjectGfxMesh  
{
private:
	int count;
	QList<Ogre::ManualObject *> mesh_list;

public:
	SelectionManagerGfxManualMesh(int count);
	virtual ~SelectionManagerGfxManualMesh();

	Ogre::ManualObject *GetMesh(int idx){return mesh_list[idx];}
	void Create(Ogre::String &name);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_EDIT;}
};

#include "Anima_nsend.h"