#pragma once

#include "MovePath.h"

class AnimationPartStairs;
class ModelInstance;

class ANIMA_CORE_API StairMovePath: public MovePath
{
friend class MovePath;
friend class LinkStairMovePath;

protected:

	// parametri

	double beginOffsetDown;
	double endOffsetDown;
	double beginOffsetUp;
	double endOffsetUp;
	float scaleSize;

	//
	QList<KFbxVector4> points;
	QList<MovePathPoint *> pathPoints;

	QList<double> levelDistance;
	QList<KFbxVector4> levelPoints;
	int stepCount;

	// calcolati
	bool directionUp;
	double pathLen;

	KFbxVector4 baseDir;
	KFbxVector4 normalBaseDir;
	float height;
	float scaleW;
	float scaleH;

private:

public:
	StairMovePath(AnimationPart *animPart);
	~StairMovePath(void);

	virtual AObject::Type GetType(){return AObject::Type::StairMovePathType;}
	virtual QString ClassName(){return "StairMovePath";}

	void Compute(int stepCount);

	virtual int AddPoint(const KFbxVector4 &p);
	virtual void MovePoint(int idx,const KFbxVector4 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear();
	virtual void Rebuild(bool fireEvents=true);

	KFbxVector4 &GetStart(){return points[0];}
	KFbxVector4 &GetEnd(){return points[1];}
	KFbxVector4 &GetPoint(int offset);

	int GetStepCount(){return stepCount;}
	bool IsDirectionUp(){return directionUp;}
	float GetScaleW(){return scaleW;}
	float GetScaleH(){return ((directionUp)? 1:-1) * scaleH;}
	float GetScaleHWithSign(){return scaleH;}
	float GetHeight(){return ((directionUp)? 1:-1) * height;}

	// lunghezza della curva
	virtual double Lenght(){return pathLen;}
	virtual double Height(){return height;}
	virtual double GetClipCount();
	virtual double GetClipW();//bool parMode);
	virtual double GetClipH();
	virtual bool GetFirst(KFbxVector4 &point,int offset=0);
	virtual bool GetLast(KFbxVector4 &point,int offset=0);
	virtual bool GetFirstNormal(KFbxVector4 &normal);
	virtual bool GetLastNormal(KFbxVector4 &normal);

	virtual int Count(){return points.count(); }
	virtual MovePathPoint *GetPathPoint(int idx);

	//calcola il punto alla distanza passata
	virtual bool GetPointAtDistance(double dist,KFbxVector4 &precPoint,KFbxVector4 &point);
	virtual bool GetInfoAtDistance(double distance,KFbxVector4 &point,double &height,KFbxVector4 &dir);
	virtual void ProjectPoint(KFbxVector4 &p,KFbxVector4 &minCurvePoint,double &minDist,KTime &minTime,KFbxVector4 &dir);

	virtual void GetFootOffset(QList<KFbxVector2> &footPosition,int &startFootCount); 

	virtual void FireChangedEvent();
	virtual void Draw(bool selectMode,ModelInstance *modelInstance);
};
