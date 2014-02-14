#pragma once

//namespace PLib
//{
//	class NurbsCurvef;
//}

#include "MovePath.h"

class AnimationPartClipSpline;
class ModelInstance;
class MeshEditable;

class PointInfo
{
public:
	KFbxVector4 point;
	double time;
	double distance;
	
	PointInfo(KFbxVector4 &point) : point(point)
	{
	}
	PointInfo(KFbxVector4 &point,double time,double distance) : point(point)
	{
	}
};

class ANIMA_CORE_API SplineMovePath: public MovePath
{
friend class LinkSplineMovePath;

protected:

	static MeshEditable *offsetKeyMesh;
	//static Mesh *offsetKeyMoveMesh;
	static Mesh *clipBoundMesh;
	static Mesh *startOffsetKeyMesh;

	double beginTime;

	bool fireChangeEvent;
	double curveLen; // calcolata ad ogni cambiamento
	KFbxVector4 firstNormal;
	KFbxVector4 lastNormal;
	QList<PointInfo> fullPoints;
	QList<KFbxVector4> points;
	QList<MovePathPoint *> pathPoints;
	void *curveObj;
//	GLUnurbs *obj;
//	PLib::NurbsCurvef *curve;

	KFbxVector4 prevNormal;
	KFbxVector4 nextNormal;
	bool prevNormalActive;
	bool nextNormalActive;

	KTime cursorTime;
	KFCurve *translation[3]; // x,y,z

	double *preComputeFrames[3];

private:

	virtual void Build();
	bool InitNormalPoints();

protected:

	void Preload();
	virtual void FireChangedEvent();

public:
	SplineMovePath(AnimationPart *animationPath);
	~SplineMovePath(void);

	virtual AObject::Type GetType(){return AObject::Type::SplineMovePathType;}
	virtual QString ClassName(){return "SplineMovePath";}

	// lunghezza della curva
	virtual double Lenght(){return curveLen;}
	virtual double Height(){return 0;}
	virtual bool GetFirst(KFbxVector4 &point,int offset=0);
	virtual bool GetLast(KFbxVector4 &point,int offset=0);
	virtual bool GetFirstNormal(KFbxVector4 &normal);
	virtual bool GetLastNormal(KFbxVector4 &normal);

	//calcola il punto alla distanza passata
	virtual bool GetPointAtDistance(double dist,KFbxVector4 &precPoint,KFbxVector4 &point);
	double GetPointTime(KFbxVector4 &p,double &dist,double &minDist);
	// calcola il punto piu' vicino alla curva
	virtual void ProjectPoint(KFbxVector4 &p,KFbxVector4 &minCurvePoint,double &minDist,KTime &minTime,KFbxVector4 &dir);

	virtual bool GetInfoAtDistance(double distance,KFbxVector4 &point,double &height,KFbxVector4 &dir);
	bool GetInfoAtTime(double time,double &distance,KFbxVector4 &point,KFbxVector4 &dir);
	bool GetTimeAtDistance(double d1,double &t1,double d2,double &t2);
	double GetDistance(double t1,double t2);

	KFbxVector4 Eval(double time);

	// puliti, senza i punti delle normali
	virtual int Count();
	virtual MovePathPoint *GetPathPoint(int idx);
	KFbxVector4 &GetPoint(int idx);

	bool SetPrevNormal(KFbxVector4 &n);
	bool SetNextNormal(KFbxVector4 &n);
	
	// mod
	virtual int AddPoint(const KFbxVector4 &p);
	virtual void MovePoint(int idx,const KFbxVector4 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear();

	virtual void Rebuild(bool fireEvents=true);

	void InnerAddPoint(int idx,const KFbxVector4 &p);

	bool PickPoint(float point[3],float maxDistance,float *dist,int *pointIdx);

	virtual void DrawLine(bool selectMode,bool curveSelected,int pointIdxSelected,ModelInstance *modelInstance);
	virtual void DrawTube(bool selectMode,bool curveSelected,int pointIdxSelected);

	// debug
	void OnTimerTick();

};
