#pragma once

//namespace PLib
//{
//	class NurbsCurvef;
//}

#include "Core/MovePath.h"

#include "Anima_nsbegin.h"
class AnimationPartClipSpline;
class ModelInstance;
class MeshEditable;
class OgreSpline;
class LinkSplineMovePath;

class PointInfo
{
public:
	Ogre::Vector3 point;
	double time;
	double distance;
	bool isVirtual;
	Ogre::Vector3 dir;
	
	PointInfo(Ogre::Vector3 &point) : point(point),isVirtual(false)
	{
	}
	PointInfo(Ogre::Vector3 &point,bool isVirtual) : point(point),isVirtual(isVirtual)
	{
	}
	/*PointInfo(Ogre::Vector3 &point,double time,double distance,Ogre::Vector3 dir) : point(point),isVirtual(false),dir(dir)
	{
	}*/
};

struct CurveCacheEntry
{
	short prevKeyIdx;
	unsigned char prevSlineKeyIdxOffset;
	double space;
	double time;
	Ogre::Vector3 point;
};

enum PointLabelType
{
	PointLabelType_DISABLED=0,
	PointLabelType_ANGLE=1,
	PointLabelType_POINT_MIN_DIST=2,
	PointLabelType_POINT_MAX_DIST=4,
	PointLabelType_POINT_MIN_MAX = 4+2 // generica
};

class ANIMA_CORE_API SplineMovePath: public MovePath 
{
friend class LinkSplineMovePath;
friend class SplineMovePathMesh;
friend class AnimationPartClipSpline;
friend class SelectionManagerGfxSplineHelp;

private:
	int prevAddPointNum;
	int nextAddPointNum;
	int startGoodIdx;
	int endGoodIdx;
protected:

	//LinkSplineMovePath *currentLinkPath;

	//bool fireChangeEvent;
	double curveLen; // calcolata ad ogni cambiamento
	QList<CurveCacheEntry> curveCache;

	Ogre::Vector3 firstNormal;
	Ogre::Vector3 lastNormal;
	QList<SplinePointInfo> splinePoints;
	int splineGoodPointCount;
	QList<PointInfo> pointsInfo;
	//QList<Ogre::Vector3> points;
	double startSpline;
	OgreSpline *pathSpline;

	Ogre::Vector3 prevNormal;
	Ogre::Vector3 nextNormal;
	bool prevNormalActive;
	bool nextNormalActive;
	Ogre::Vector3 _prevNormal;
	Ogre::Vector3 _nextNormal;

	MovePathTrx *trx;

	QList<PointLabelType> pointsLabelInfo;
	bool badPath;
	
private:

	//virtual void Build();
	bool InitNormalPoints();

protected:

	virtual MovePathTrx *GetTrx(){return trx;}

	void Preload();
	void OnPreRenderTargetUpdate();
	virtual void OnPreRender();
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);    
public:
	SplineMovePath(AnimationPart *animationPath, const QString & name = "");
	virtual ~SplineMovePath(void);

	//LinkSplineMovePath *GetCurrentLinkPath(){return currentLinkPath;};
	//void SetCurrentLinkPath(LinkSplineMovePath *path){currentLinkPath=path;}

	virtual bool IsLink();

	static PointLabelType GetAngleType(QList<Ogre::Vector3 > &pointList,int i);

        virtual AObject::Type GetType(){return AObject::SplineMovePathType;}
	virtual QString ClassName(){return "SplineMovePath";}
	virtual QString GetDesc(){return "Spline";}

	int _ProjectPoint(const Ogre::Vector3 &p,double &error);

	virtual ModelInstance *GetModel(){return NULL;}

	// lunghezza della curva
	virtual double Length(){return curveLen;}
	virtual double Height(){return 0;}
	virtual bool GetFirstNormal(Ogre::Vector3 &normal);
	virtual bool GetLastNormal(Ogre::Vector3 &normal);

	QList<SplinePointInfo> &GetSplinePointInfo(){return splinePoints;}

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);

	int FindByDistance(double distance,int startIdx);
	int FindByTime(double time);

	virtual void Copy(SceneObject *fromObj);

	// calcola il punto piu' vicino alla curva
	virtual void ProjectPoint(Ogre::Vector3 &p,Ogre::Vector3 &minCurvePoint,double &minSpace,double &minTime,Ogre::Vector3 &dir,double &error);
	virtual bool GetInfoAtDistance(int *idx,double distance,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);
	virtual double GetDistanceAtTime(double time);
	virtual bool GetInfoAtByIdx(int pointIdx,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);

	double GenCurveLen(double distanceFrom,double distanceTo);

	Ogre::Vector3 Eval(double time);

	// puliti, senza i punti delle normali
  //  int GetPointsCount() {return points.count();}
	// mod
	//virtual int Count();
	//Ogre::Vector3 &GetPoint(int idx);
	//virtual int AddPoint(const Ogre::Vector3 &p);
	//virtual void MovePoint(int idx,const Ogre::Vector3 &p);
	//virtual void DeletePoint(int idx);
	//virtual void Invert();
	virtual void Clear();

	void AssignGUIPoints(QList<MovePath *> currentPointList);

	//virtual void Rebuild(bool fireEvents=true);

	virtual void GetAnchor(Ogre::Vector3 &pivot,float &angleXZ,float &dimension);

	//void InnerAddPoint(int idx,const Ogre::Vector3 &p);

	bool PickPoint(float point[3],float maxDistance,float *dist,int *pointIdx);

	bool Effect_CursorAdd(const Ogre::Vector3 &p);

	// IBuildElement

	virtual void Build();
};
#include "Anima_nsend.h"
