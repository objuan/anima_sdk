#pragma once

#include "Core/MovePath.h"
#include "Core/Property.h"

#include "Anima_nsbegin.h"
class AnimationPartClipPoint;
class ModelInstance;
class MeshEditable;
class ModelClipPath;
//class AnimClipModelGfx;
class AnimClipModel;
class LinkPointMovePath;
class GfxClip;
class QRandom;

//class PointMovePathLinkEntry
//{
//public:
//	Ogre::Vector3 pos_offset;
//	double angleXZ_offset;
//	PointMovePathLinkEntry()
//	{ 
//		pos_offset = Ogre::Vector3::ZERO;
//		angleXZ_offset=0;
//	}
//};

class ANIMA_CORE_API PointMovePath: public MovePath
{
friend class ContextIO;
friend class MovePath;
friend class PointMovePathMesh;
friend class AnimationPartClipPoint;
friend class PointMovePathGeom;
friend class LinkPointMovePath;
friend class AnimationPart;

protected:

	double pathLen;

	//QMap<QString ,PointMovePathLinkEntry *> linkOffsetMap;
	MovePathTrx *trx;

	bool geometryFullMode;
	bool enableLinkOffset;
	bool autoLinkOffset;
	AnimClipModel *modelClip;
	bool modifyEnabled;
//	bool havePoint;
//	double user_angleXZ;
	double linkFromAngleXZ;
	double linkToAngleXZ;
	Ogre::Vector3 clipFixedOffset;
	Ogre::Vector3 clipToOffset;
	//Ogre::Vector3 pivot;
	Ogre::Vector3 enter;
	Ogre::Vector3 exit;
	Ogre::Vector3 enterOffset;
	Ogre::Vector3 exitOffset;
	Ogre::Vector3 enterDir;
	Ogre::Vector3 exitDir;
	Ogre::Vector3 box[4];
	MovePathPoint *movePathPoint;
	QList<Ogre::Vector3> posList;
	Ogre::Vector3 pivotOffset;
	Ogre::Vector3 pivotMid;
	Ogre::Vector3 pivotLast;

	// base

	Ogre::Vector3 base_box[4];
	Ogre::Vector3 base_enter;
	Ogre::Vector3 base_exit;
	Ogre::Vector3 base_enterDir;
	Ogre::Vector3 base_exitDir;
	double	base_linkFromAngleXZ;
	double	base_linkToAngleXZ;

	bool valid;
	QList<GfxClip *> gfxList;
	int old_Bounds;
	Ogre::Quaternion q_rot;
	QString oldClip;

	PropertyCollection *propertyList;

//	MeshEditable *mesh;
public:
	int old_enterIdx; // usati per controllare l'indice del clip corrente selezionato dall'utente
	int old_exitIdx;

	int clipEnterIdx; // indice sulla lista dei clip 
	int clipLoopIdx;
	int clipExitIdx;
	QString oldClipID;

private:
	void BuildDirPos(ModelClipPath &path,bool isStart,Ogre::Vector3 &startPos,double startPosAngle,Ogre::Vector3  *pos,Ogre::Vector3 *dir);

protected:
	virtual MovePathTrx *GetTrx(){return trx;}

	virtual void OnPreRender();
	virtual void OnPreRender(int group,int idx,SubSceneObjectGfx *gfxObj);
	virtual void OnPreRenderTargetUpdate();

public:
	PointMovePath(AnimationPart *animPart, const QString & name = "");
	~PointMovePath(void);

	
	virtual bool IsLink(){return false;}
	virtual AObject::Type GetType(){return AObject::PointMovePathType;}
	virtual QString ClassName(){return "PointMovePath";}
	virtual QString GetDesc(){return "Fixed";}

	virtual ModelInstance *GetModel();
	PropertyCollection &GetPropertyList(){return *propertyList;}

	void SetPositionsFromModels(bool setPivotAsModelOffset,QRandom *rnd_rot);

	Ogre::Vector3 GetPivot();
	void SetPivot(const Ogre::Vector3 &v);
	bool GetAutoLinkOffset(){return autoLinkOffset;}
	void SetAutoLinkOffset(bool autoLinkOffset);
	bool GetGeometryFullMode();
	void SetGeometryFullMode(bool geomFullMode);
	bool GetEnableLinkOffset();
	void SetEnableLinkOffset(bool enableLinkOffset);

	bool IsPuntual();
	virtual Ogre::Vector3 GetClipFixedOffset(){return clipFixedOffset;}
	virtual Ogre::Vector3 GetClipToOffset(){return clipToOffset;}
	virtual double GetAngleXZ();
	virtual double GetUserAngleXZ();
	virtual double GetLinkFromAngleXZ(){return linkFromAngleXZ;}
	virtual double GetLinkToAngleXZ(){return linkToAngleXZ;}
	void SetAngleXZ(double angleXZ);

	virtual double Length(){return pathLen;}
	virtual double Height(){return 0;}

	virtual void Copy(SceneObject *fromObj);
	virtual int AddPoint(const Ogre::Vector3 &p,int idx);

	virtual Ogre::Vector3 GetGeometryPivot();
	
	Ogre::Vector3 GetPoint(int offset);

	virtual bool GetPathSplineFirst(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);
	virtual bool GetPathSplineLast(QList<SplinePointInfo> &points,int *backOffset,Ogre::Vector3 &lastSplinePoint);

	virtual bool GetFirstNormal(Ogre::Vector3 &normal);
	virtual bool GetLastNormal(Ogre::Vector3 &normal);
	void SetFirst(Ogre::Vector3 &point);
	virtual bool GetFirstPoint( Ogre::Vector3 &p,int offset=0);
	virtual bool GetLastPoint( Ogre::Vector3 &p,int offset=0);

	virtual Ogre::Vector3 GetWPosition(Ogre::Vector3 &clipPoint,int phase,double timePercent);
	virtual bool Rotate(Ogre::Vector3 &pivot,double axz);

	Ogre::Vector3 Rotate(Ogre::Vector3 &v);

	//calcola il punto alla distanza passata
	virtual bool GetPointAtDistance(double dist,Ogre::Vector3 &precPoint,Ogre::Vector3 &point);
	virtual bool GetInfoAtDistance(int *idx,double distance,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);
	virtual double GetDistanceAtTime(double time){return 0;}

	virtual void ProjectPoint(Ogre::Vector3 &p,Ogre::Vector3 &minCurvePoint,double &minSpace,double &minTime,Ogre::Vector3 &dir,double &error);

	// 0 = enter, 1 = fixed, 2 = exit
	int GetClipType(GfxClip * clip);
	GfxClip *GetClipByType(int type);

	Ogre::Vector3 COLL_GUIToWorld(const Ogre::Vector3 &p);

	virtual void GUINotifyAddPoint(Ogre::Vector3 &p);
	virtual void GetAnchor(Ogre::Vector3 &pivot,float &angleXZ,float &dimension);

	// IBuildElement
	virtual void InvalidateBuild(int type=0);

	virtual void Build();
	virtual void PostBuild();
};

class ANIMA_CORE_API PointMovePathGeom: public SubSceneObjectGfx
{
protected:
		
	QString meshName;
	PointMovePath *movePath;
	LinkPointMovePath *linkPath;

public:

	PointMovePathGeom(PointMovePath *movePath,const QString &meshName);
	virtual ~PointMovePathGeom();

	PointMovePath *GetMovePath();
	void Assign(PointMovePath *movePath);

	QString GetMeshName(){return meshName;}
	void OnPreRender();

	//virtual QString GetDesc(){return "Path Offset";}

	virtual AObject::Type GetType(){return AObject::PointMovePathGeomType;}
};

#include "Anima_nsend.h"
