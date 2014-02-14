#pragma once

#include "Marker.h"
#include "BodyCoord.h"
#include "Anima_nsbegin.h"

class MarkerFootTranform;

class MarkerModderFun: public BObject
{
public:
	double start;
	double end;
	double startNew;
	double endNew;
	double factor;
	bool isConstant;

	MarkerModderFun();
	void Build();
	void BuildConstant();
	double Eval(double val);

};

class MarkerModder : public BObject
{
protected:
	Marker *marker;

public:
	MarkerModder(Marker *marker);
	~MarkerModder();

	void ChangeKeyPos(MarkerInfo &markerInfo,int keyIdx,MarkerModderFun &distFun,MarkerModderFun &heightFun,bool debug=false,double spaceOffset=0,double heightOffset=0);

	void ResizeXZDistance(double newDistance,double &spaceFactor);
	void Resize(double newDistance,double newHeight,double &spaceFactor,double &heightFactor,double distOffset=0,double heightOffset=0);
	void Scale(MarkerInfo markerInfo,double spaceFactor,double heightFactor,double distOffset=0,double heightOffset=0);
	void Scale(double spaceFactor,double heightFactor,double distOffset=0,double heightOffset=0);

	void ResizeFootFixBoundMarker(double spaceFactor,double heightFactor,bool isLeftFoot);
	void ResizeFootMarker(double spaceFactor,double heightFactor,bool isLeftFoot);

	void Modify(MarkerInfo &refInfo,MarkerFootTranform &footTrx,double spaceOffset,double heightOffset);

};

class MarkerModderPos : public MarkerModder
{
public:
	enum MarkerModderPosPolicy 
	{
		NO_BUILD = 0,
		ALL_DIRECTION=1,
		DOWN_DIRECTION = 2,
		UP_DIRECTION = 3,

	};
public:
	MarkerModderPos(Marker *marker);
	~MarkerModderPos();

	void SetFinalPos(PhysicCoord &finalPos,int policy );
	void SetBoundPos(PhysicCoord &startPos,PhysicCoord &endPos,bool isHip,int policy,Ogre::Vector2 &clipOffset );
};
#include "Anima_nsend.h"