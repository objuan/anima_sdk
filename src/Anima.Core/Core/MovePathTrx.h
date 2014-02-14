#pragma once

#include "Anima_nsbegin.h"
class Model;
class ModelInstance;
//class AnimClipModelGfxProperty;
class AnimClip;
class MovePath;
class MovePathTrx_Link;

class ANIMA_CORE_API MovePathTrx: public BObject
{
protected:
	void SaveMatrix(QDomElement &ele,const QString &name,Ogre::Matrix4 &trx);
	void LoadMatrix(QDomElement &ele,const QString &name,Ogre::Matrix4 &trx);
public:

	MovePathTrx(){}
	virtual ~MovePathTrx(){}

	virtual Ogre::Vector3 GetPoint(int idx)=0;
	virtual int Count()=0;
	virtual void SetPoint(int idx,const Ogre::Vector3 &p)=0;
	virtual void DeletePoint(int idx)=0;
	virtual void Clear(bool clearAbs,bool clearRel)=0;
	virtual void Append(const Ogre::Vector3 &p)=0;
	virtual void Insert(int idx,const Ogre::Vector3 &p)=0;
	virtual double GetRotate(int idx)=0;
	virtual void RotatePoint(int idx,double rot_xz)=0;

	// help
	virtual void InitHelp()=0;
	virtual Ogre::Vector3 GetHelpPoint(int idx)=0;
	virtual void SetHelpPoint(int idx,const Ogre::Vector3 &p)=0;

	virtual QList<Ogre::Vector3> &GetPoints()=0;
	virtual MovePath *GetPath()=0;

	virtual void Load(QDomElement &ele)=0;
	virtual void Save(QDomDocument &doc,QDomElement &ele )=0;

	virtual void Lock()=0;

	virtual MovePathTrx *Clone()=0;
	virtual void Copy(MovePathTrx *trx)=0;
};

struct ANIMA_CORE_API MovePathTrx_BasePoint
{
public:

	Ogre::Vector3 point;
	double rot_xz;
	Ogre::Vector3 helpPoint;

	MovePathTrx_BasePoint(const Ogre::Vector3 &point) :point(point),rot_xz(0)
	{
	}
	MovePathTrx_BasePoint(const Ogre::Vector3 &point,double rot_xz) :point(point),rot_xz(rot_xz)
	{
	}
};

class ANIMA_CORE_API MovePathTrx_Base : public MovePathTrx
{
friend class MovePathTrx_Link;
private:
	MovePath *movePath;
	QList<Ogre::Vector3> exportPoints;

	Ogre::Matrix4 baseTrx;
	QList<MovePathTrx_BasePoint> pointList;
	
	QList<MovePathTrx_Link *> linkList;

public:
	MovePathTrx_Base(MovePath *movePath);
	virtual ~MovePathTrx_Base();

	virtual Ogre::Vector3 GetPoint(int idx);
	virtual int Count();
	virtual void SetPoint(int idx,const Ogre::Vector3 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear(bool clearAbs,bool clearRel);
	virtual void Append(const Ogre::Vector3 &p);
	virtual void Insert(int idx,const Ogre::Vector3 &p);
	virtual double GetRotate(int idx);
	virtual void RotatePoint(int idx,double rot_xz);

	virtual QList<Ogre::Vector3> &GetPoints();
	virtual MovePath *GetPath(){return movePath;}

	virtual void InitHelp();
	virtual Ogre::Vector3 GetHelpPoint(int idx);
	virtual void SetHelpPoint(int idx,const Ogre::Vector3 &p);

	virtual MovePathTrx *Clone();
	virtual void Copy(MovePathTrx *trx);

	virtual void Lock(){}

	virtual void Load(QDomElement &ele);
	virtual void Save(QDomDocument &doc,QDomElement &ele );
};

struct ANIMA_CORE_API MovePathTrx_LinkPoint
{
public:
	Ogre::Vector3 offset;
	double rot_xz;
	double relativeOffset;
	Ogre::Vector3 helpOffset;

	bool cacheValid;
	Ogre::Vector3 cachedValue;

	MovePathTrx_LinkPoint() :offset(Ogre::Vector3::ZERO),rot_xz(0),relativeOffset(0),cacheValid(false)
	{
	}
	MovePathTrx_LinkPoint(const Ogre::Vector3 &off,double rot_xz,double relOffset) :offset(off),rot_xz(rot_xz),relativeOffset(relOffset),cacheValid(false)
	{
	}
};

class ANIMA_CORE_API MovePathTrx_Link : public MovePathTrx
{
friend class MovePathTrx_Base;
private:
	QList<Ogre::Vector3> exportPoints;
	QList<MovePathTrx_LinkPoint> pointList;
	MovePathTrx_Base *trxBase;
	//bool invert;

	ModelInstance *modelInstance;
	MovePath *movePath;

private:
	Ogre::Vector3 GetOffsetDir(int idx,Ogre::Vector3 &dirXZ);

public:
	MovePathTrx_Link(MovePathTrx_Base *trxBase,MovePath *movePath,ModelInstance *modelInstance,bool isVirtual=false);
	virtual ~MovePathTrx_Link();

	ModelInstance *GetModel(){return modelInstance;}
	virtual Ogre::Vector3 GetPoint(int idx);
	virtual int Count();
	virtual void SetPoint(int idx,const Ogre::Vector3 &p);
	virtual void DeletePoint(int idx);
	virtual void Clear(bool clearAbs,bool clearRel);
	virtual void Append(const Ogre::Vector3 &p);
	virtual void Insert(int idx,const Ogre::Vector3 &p);
	virtual void ClearPoint(int idx);
	virtual double GetRotate(int idx);
	virtual void RotatePoint(int idx,double rot_xz);

	virtual double GetPointRelative(int idx);
	virtual void SetPointRelative(int idx,double value);
	virtual Ogre::Vector3 GetOffset(int idx);

	//inlinbool IsInvert();
	void InvalidateCache();

	virtual QList<Ogre::Vector3> &GetPoints();
	virtual MovePath *GetPath(){return movePath;}

	virtual void InitHelp();
	virtual Ogre::Vector3 GetHelpPoint(int idx);
	virtual void SetHelpPoint(int idx,const Ogre::Vector3 &p);

	virtual MovePathTrx *Clone();
	virtual void Copy(MovePathTrx *trx);

	virtual void Lock();

	virtual void Load(QDomElement &ele);
	virtual void Save(QDomDocument &doc,QDomElement &ele );
};



#include "Anima_nsend.h"
