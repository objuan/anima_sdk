#pragma once

#include "Core/MovePath.h"
#include "Anima_nsbegin.h"
class AnimationPartStairs;
class ModelInstance;
class LinkStairMovePath;

class ANIMA_CORE_API StairMovePath: public MovePath
{
friend class MovePath;
friend class LinkStairMovePath;
friend class StairMovePathMesh;
friend class AnimationPartStairs;

protected:

	// parametri

	double beginOffsetDown;
	double endOffsetDown;
	double beginOffsetUp;
	double endOffsetUp;

	MovePathTrx *trx;

	QList<double> levelDistance;
	QList<Ogre::Vector3> levelPoints;
	int stepCount;
	double stepWidth;

	// calcolati
	bool directionUp;
	double pathLen;

	Ogre::Vector3 baseDir;
	Ogre::Vector3 normalBaseDir;
	float height;
	float scaleW;
	float scaleH;
	float stepW;
	float stepH;
	bool isValid;
	AnimationPartStairs *animationPartStairs;

	// runtime
	double beginOffset;
	double endOffset;


protected:
	virtual MovePathTrx *GetTrx(){return trx;}

	virtual void OnPreRender();
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);

public:
	StairMovePath(AnimationPart *animPart, const QString &name = "");
	~StairMovePath(void);

        virtual AObject::Type GetType(){return AObject::StairMovePathType;}
	virtual QString ClassName(){return "StairMovePath";}
	virtual QString GetDesc(){return "Stair";}

	void Compute(int stepCount);

	//LinkStairMovePath *GetCurrentLinkPath(){return currentLinkPath;};
	//void SetCurrentLinkPath(LinkStairMovePath *path){currentLinkPath=path;}


	virtual int AddPoint(const Ogre::Vector3 &p,int idx);
	virtual void MovePoint(int idx,const Ogre::Vector3 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear();
	//virtual void Invert();
	//virtual void Rebuild(bool fireEvents=true);

	Ogre::Vector3 GetStart();
	Ogre::Vector3 GetEnd();
	//Ogre::Vector3 &GetPoint(int offset);
	Ogre::Vector3 &GetDirection(){return baseDir;}

	virtual bool IsValid(){return isValid;}

	double GetStepWidth(){return stepWidth;}
	int GetStepCount(){return stepCount;}
	bool IsDirectionUp(){return directionUp;}
	float GetScaleW(){return scaleW;}
	float GetScaleH(){return ((directionUp)? 1:-1) * scaleH;}
	float GetScaleHWithSign(){return scaleH;}
	float GetHeight(){return ((directionUp)? 1:-1) * height;}

	double GetFirstW();
	double GetLastW();
	double GetStepW(){return stepW;}
	double GetStepH(){return stepH;}

	// lunghezza della curva
	virtual double Length(){return pathLen;}
	virtual double Height(){return height;}
	virtual double GetClipCount();
	virtual double GetClipW();//bool parMode);
	virtual double GetClipH();
//	virtual bool GetPathFirst(Ogre::Vector3 &point,int offset=0);
	//virtual bool GetPathLast(Ogre::Vector3 &point,int offset=0);
	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetFirstNormal(Ogre::Vector3 &normal);
	virtual bool GetLastNormal(Ogre::Vector3 &normal);

//	virtual int Count(){return points.count(); }
//	virtual MovePathPoint *GetPathPoint(int idx);

	//calcola il punto alla distanza passata
	virtual bool GetInfoAtDistance(int *idx,double distance,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);
	virtual double GetDistanceAtTime(double time);

	virtual void ProjectPoint(Ogre::Vector3 &p,Ogre::Vector3 &minSpace,double &minDist,double &minTime,Ogre::Vector3 &dir,double &error);

	virtual void GetFootOffset(QList<Ogre::Vector2> &footPosition,int &startFootCount); 

	virtual void GUINotifyAddPoint(Ogre::Vector3 &p);
	virtual void GetAnchor(Ogre::Vector3 &pivot,float &angleXZ,float &dimension);

	void GetFootPoints(QList<Ogre::Vector3 > &footPosition);
//	virtual void FireChangedEvent();
//	virtual void Draw(bool selectMode,ModelInstance *modelInstance);
	void CheckOneStep();

	// IBuildElement
	//virtual void InvalidateBuild(int type=0);
	virtual void Build();
};
#include "Anima_nsend.h"
