#pragma once

#include "Core/Context.h"

#include "Anima_nsbegin.h"

class MovePath;
class MovePathTrx;
class SelectionManagerGfxSplineHelp;
class PointMovePathGeom;

class Point_PathManipolator : public BObject
{
public:
	int pointIdx;
	
public:

	MovePathTrx *undoPathTrx;
	MovePathTrx *pathTrx;
	Ogre::Vector3 outP;
	double startRot;
	
	Ogre::Vector3 GetStartPoint();
	Ogre::Vector3 GetPoint();
	Ogre::Vector3 GetHelpPoint();
	//void MoveHelpPoint(Ogre::Vector3 &p);
	void WriteHelpValue();
	void WriteStoreValue();
	void Undo();
	void Clear(bool clearAbs,bool clearRel);

	Point_PathManipolator(MovePathTrx *trx,int idx);
	~Point_PathManipolator();
};

class PointList_PathManipolator : public BObject
{
public:
	QList<SelectionManagerGfxSplineHelp *> splineHelpList;
	PointMovePathGeom *pointGeom;

	QList<Point_PathManipolator *>  pointList;

	PointList_PathManipolator():pointGeom(NULL){}
	~PointList_PathManipolator();

	void UpdateGfx(const Ogre::Vector3 &offset,double angle);
	void EndGfx();
};

class ANIMA_CORE_API MovePathManipolator  : public BObject
{
public:
	Ogre::AxisAlignedBox bbox;
	QMap<MovePath *,PointList_PathManipolator *> pointMap;
	bool  useGhost;

	Ogre::Vector3 startPointW;
	Ogre::Vector3 firstDragPointW;
	Ogre::Vector3 currentPointW;
	double startAngle;
	double endAngle;

protected:
	virtual Ogre::Vector3 GetPivot();
	virtual void GetInfo(QList<Point_PathManipolator *> &points,bool &useGhost)=0;
public:

	MovePathManipolator();
	~MovePathManipolator();

	void Begin(Ogre::Vector3 &worldP);
	void Drag(Ogre::Vector3 &worldP);
	void End();
	void Undo();
};

class ANIMA_CORE_API Point_MovePathManipolator  : public MovePathManipolator
{
protected:
	virtual void GetInfo(QList<Point_PathManipolator *> &points,bool &useGhost);
public:
	Point_MovePathManipolator();
};
class ANIMA_CORE_API Path_MovePathManipolator  : public MovePathManipolator
{
protected:
	virtual Ogre::Vector3 GetPivot();
	virtual void GetInfo(QList<Point_PathManipolator *> &points,bool &useGhost);
public:
	Path_MovePathManipolator();
};

class ANIMA_CORE_API ClearPathManipolator  : public MovePathManipolator
{
protected:
	virtual void GetInfo(QList<Point_PathManipolator *> &points,bool &useGhost);
	//QList<MovePathTrx *> pathList;
	QMap<int,QList<MovePathTrx *> *> modelList;
public:
	
	ClearPathManipolator(){}
	~ClearPathManipolator(){}

	void Clear(bool clearAbs,bool clearRel);
	void Undo();
};

// ====================================-

class ANIMA_CORE_API PathManipolator  : public BObject
{
private:
	MovePathManipolator *point_movePathManipolator;
	MovePathManipolator *path_movePathManipolator;
	ClearPathManipolator *clearPathManipolator;
public:
	static MovePathManipolator *currentManipolator;

public:

	PathManipolator();
	~PathManipolator();

	MovePathManipolator *GetPointManipolator(){return point_movePathManipolator;}
	MovePathManipolator *GetPathManipolator(){return path_movePathManipolator;}
	ClearPathManipolator *GetClearPathManipolator(){return clearPathManipolator;}
	
//	bool Rotate(Ogre::Vector3 &pivot,double axz);*/

};

#include "Anima_nsend.h"

