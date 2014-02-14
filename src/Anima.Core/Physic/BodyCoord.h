#pragma once
#include "Anima_nsbegin.h"
class WorldCoord;
struct ClipCoord;
struct PhysicCoord;
class Body;
class GfxClip;
class BodyModderSubClip;

// dipende dal modello

class CoordOrigin: public BObject
{
public:
	GfxClip *gfxClip;
	Ogre::Vector3 clipOrigin;
	BodyModderSubClip *modder;

	Ogre::Vector2 physicalOffset;

	Ogre::Vector3 worldOrigin;
	Ogre::Vector3 worldDirXZ;

	CoordOrigin(){}
	CoordOrigin(GfxClip *gfxClip,BodyModderSubClip *modder);
	Body *GetBody();
	double GetClipGroundOffset();
	Ogre::Vector3 GetClipOrigin(){return clipOrigin;}
	Ogre::Vector3 Delta(PhysicCoord &coord);
};

struct ClipCoord //: public BObject
{
//	friend class PhysicCoord;

//private:
	CoordOrigin *coordOrigin;

//public:
	double X;
	double Y;
	double Z;

	ClipCoord(){}
	ClipCoord(CoordOrigin &coordOrigin,Ogre::Vector3 &p);
	ClipCoord(CoordOrigin &coordOrigin,double X,double Y,double Z);
	ClipCoord(const ClipCoord &ClipCoord,double X,double Y,double Z);
	ClipCoord(const ClipCoord &ClipCoord,Ogre::Vector3 &p);
	ClipCoord(const PhysicCoord &physicCoord,Ogre::Vector3 &p);

	CoordOrigin &GetCoordOrigin(){return *coordOrigin;};
	Ogre::Vector3 Origin();
	PhysicCoord ToPhysic();
	Body *GetBody();
};

// metri
struct PhysicCoord 
{
//	friend class WorldCoord;
//	friend class ClipCoord;
//private:
	ClipCoord clipCoord; 
//public:
	double D;
	double H;
	double X;

	PhysicCoord(){}
//	PhysicCoord( PhysicCoord &c){}
	PhysicCoord(const ClipCoord &clipCoord,double D,double H,double X);
	PhysicCoord(const PhysicCoord &physicCoord,double D,double H);

	WorldCoord ToWorld();
	//ClipCoord ToClip();
	ClipCoord ToClip(const Ogre::Vector2 &offset);
	void AddFootOffset(bool isLeft,int keyIdx);

    inline PhysicCoord Add(double _D,double _H) const
	{
		return PhysicCoord(clipCoord,D+_D,H+_H,X);
	} 
	inline PhysicCoord Add(PhysicCoord &a) const
	{
		return PhysicCoord(clipCoord,D+a.D,H+a.H,X);
	} 
	inline PhysicCoord Sub(PhysicCoord &a) const
	{
		//return PhysicCoord(clipCoord,D-a.D,H-a.H,X-a.X);
		return PhysicCoord(clipCoord,D-a.D,H-a.H,X);
	}
	inline PhysicCoord Scale(PhysicCoord &origin,double SD,double SH) const
	{
		return PhysicCoord(clipCoord,origin.D+(D-origin.D)*SD, origin.H+(H-origin.H)*SH, X);
	}
	inline PhysicCoord Scale(double SD,double SH) const
	{
		return PhysicCoord(clipCoord,D*SD,H*SH,X);
	}
};

// in metri
class WorldCoord : public BObject
{
private:
	ClipCoord clipCoord;
public:
	double X;
	double Y;
	double Z;

	WorldCoord(const ClipCoord &clipCoord,const Ogre::Vector3 &p);
	WorldCoord(const PhysicCoord &physicCoord,const Ogre::Vector3 &p);

	ClipCoord ToClip();
	PhysicCoord ToPhysic();//Ogre::Vector3 &origin,Ogre::Vector3 &dirXY);
	Ogre::Vector3 T(){return Ogre::Vector3(X,Y,Z);}
};
#include "Anima_nsend.h"
