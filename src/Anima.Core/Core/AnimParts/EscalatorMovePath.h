#pragma once

#include "Core/MovePath.h"

#include "Anima_nsbegin.h"
class AnimationPartClipPoint;
class ModelInstance;
class MeshEditable;
class EscalatorMovePathMesh;
class GfxClipAnim;
class GfxClip;
class ModelClip;
class Model;
class AnimClipModel;
class GfxModel;

struct MovePathClipPartInfo
{
	double distance;
	double height;
	double time;
	//double path_statTime;
};

class EscalatorMoveLine
{
	friend class EscalatorMovePath;
public:
//	float stepWidth;
	float angle;
	float speed_base;
	float speed_raise;
	bool upDir;
	float len;
	float height;
	float totalLen;
	//float dir;
	bool rampaMode;
	float stepWidth;
	
	float raiseBaseLen;
	float timeEnter;
	float timeExit;
	float raiseTime;
	float periodTime;
	float stepAspectRatio;
	float fullLen;
	float guiStepH;

	float baseEnterLink;
	float baseEnterFixed;
	float baseExitLink;
	float baseExitFixed;
	
	Ogre::Vector3 topPoint;
	float sa;
public:
	int totalStep;
	// parametri
	double baseEnter; 
	double baseExit;

private:
	float GetLastTimePosition(float globalTime,float stepStartTime,float clipWesCutTime,int stepIdx,float lastDistance);
	int GetStepInfo(float globalTime,GfxClip *clip,float &waitTime,float &clipCutTime);
//	float GetWaitTime(float globalTime);
//	void GetStepOffset(float globalTime,float &timeOffset,float &spaceOffset);

public:
	EscalatorMoveLine();
	void Set(float totalLen,float height,float speed,bool upDir,double baseEnter,double baseExit);
	Ogre::Vector3 GetStepPosition(float time,int stepIdx,bool &visible,float &rot);
		
	bool GetInfo(GfxClip *clip,float globalPathStartTime,int partNumber,float &time,float &outSpace,Ogre::Vector3 &point,float &height,Ogre::Vector3 &dir
		,float &_startPathClipTime0, float &_startPathClipTime1);
	void GetClipInfo (GfxModel *gfxModel ,float globalPathStartTime, QList<MovePathClipPartInfo> &clipList
		 ,float &_startPathClipTime0, float &_startPathClipTime1);
	//void GetClipInfo(GfxClip *clip,double globalPathStartTime,int partNumber,double &distance,double &height,double &time);
	
	bool IsUpDir(){return upDir;}
};


class ANIMA_CORE_API EscalatorMovePath: public MovePath
{
friend class MovePath;
friend class EscalatorMovePathMesh;

protected:

	double pathLen;
	MovePathPoint *movePathPoint;

	float deg;		// readonly
	int stepNumber; // readonly

	bool modifyEnabled;
	bool valid;
	//bool havePoint;

//	Ogre::Vector3 pivot;
	double angleXZ;
	//QList<Ogre::Vector3> points;
	MovePathTrx *trx;

	EscalatorMovePathMesh *mesh;

	EscalatorMoveLine moveLine;

	Ogre::Vector3 stairDir;
	Ogre::Vector3 old_stairDir;
	Ogre::Vector2 dirXZ;
	double oldSpeed;
	float xzAngle;

	Ogre::Vector3 baseDir;
	Ogre::Vector3 normalBaseDir;

	//Ogre::Vector3 enterDir;
	//Ogre::Vector3 exitDir;

	double pathOffset;
	float _lastDy;

private:
	
protected:
	virtual MovePathTrx *GetTrx(){return trx;}

	virtual void OnPreRender();
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);

public:
	EscalatorMovePath(AnimationPart *animPart, const QString & name = "");
	~EscalatorMovePath(void);

	virtual AObject::Type GetType(){return AObject::EscalatorMovePathType;}
	virtual QString ClassName(){return "EscalatorMovePath";}
	virtual QString GetDesc(){return "Escalator";}

	EscalatorMoveLine &GetMoveLine(){return moveLine;}
	Ogre::Vector2 GetDirXZ(){return dirXZ;}
	float GetXZAngle(){return xzAngle;}
	Ogre::Vector3 GetStairDir(){return stairDir;}

	Ogre::Vector3 ResolveLastPoint(const Ogre::Vector3 &p,bool worldCoordinate);
	Ogre::Vector3 ResolveFirstPoint(const Ogre::Vector3 &p);

	virtual double Length(){return pathLen;}
	virtual double Height(){return stairDir[1];}

	Ogre::Vector3 GetStart();
	Ogre::Vector3 GetEnd();
//	virtual int Count(){return points.count(); }
	virtual int AddPoint(const Ogre::Vector3 &p,int idx);
	virtual void MovePoint(int idx,const Ogre::Vector3 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear();
//	virtual void Invert();

	//Ogre::Vector3 &GetPoint(int offset);

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetFirstNormal(Ogre::Vector3 &normal);
	virtual bool GetLastNormal(Ogre::Vector3 &normal);
	void SetFirst(Ogre::Vector3 &point);

	Ogre::Vector3 GetWPosition(Ogre::Vector3 &clipPoint,int phase);
	virtual bool Rotate(Ogre::Vector3 &pivot,double axz);

	Ogre::Vector3 Rotate(Ogre::Vector3 &v);
//	virtual MovePathPoint *GetPathPoint(int idx);

	//calcola il punto alla distanza passata
	virtual bool GetPointAtDistance(double dist,Ogre::Vector3 &precPoint,Ogre::Vector3 &point);
	virtual bool GetInfoAtDistance(int *idx,double distance,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);
	virtual double GetDistanceAtTime(double time);

	bool GetAtTime(GfxClip *clip,float globalPathStartTime,float &time,float &outSpace,Ogre::Vector3 &point,float &height,Ogre::Vector3 &dir
		,float &_startPathClipTime0, float &_startPathClipTime1);

	void GetClipInfo (Model *model,float globalPathStartTime, QList<MovePathClipPartInfo> &clipList
			 ,float &_startPathClipTime0, float &_startPathClipTime1);

	virtual void ProjectPoint(Ogre::Vector3 &p,Ogre::Vector3 &minCurvePoint,double &minSpace,double &minTime,Ogre::Vector3 &dir,double &error);

	virtual void GetAnchor(Ogre::Vector3 &pivot,float &angleXZ,float &dimension);
	virtual void GUINotifyAddPoint(Ogre::Vector3 &p);

	// IBuildElement
	//virtual void InvalidateBuild(int type=0);
	virtual void Build();

	virtual void ExportGeometry(MeshExporter *,GeomExtCollection &extList);
};

class EscalatorPathPoint: public MovePathPoint
{

public:

	EscalatorPathPoint(MovePath *movePath);
//	virtual ~EscalatorPathPoint();

};

class EscalatorMovePathGeom: public SubSceneObjectGfx
{
protected:
		
	QString meshName;

public:

	EscalatorMovePathGeom(MovePath *movePath,const QString &meshName);
	virtual ~EscalatorMovePathGeom();

	QString GetMeshName(){return meshName;}

	void Update(float internalTime);

};

#include "Anima_nsend.h"
