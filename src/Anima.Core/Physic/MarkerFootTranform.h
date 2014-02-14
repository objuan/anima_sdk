#pragma once
#include "Anima_nsbegin.h"
class Marker;
class GfxClip;

struct MarkerFoot
{
public:
	int startKey;
	int endKey;
	Ogre::Vector3 pos;
	double distanceXZ; 
	double height;

	double prevStepDistanceXZ;
	double nextStepDistanceXZ;

	double newDistanceXZ;
	double newPrevStepDistanceXZ;

	double prevStepHeight;
	double newHeight;

	double GetStartDistance(){return distanceXZ-prevStepDistanceXZ;}
	double GetStartHeight(){return height-prevStepHeight;}
};

class MarkerFootTranform: public BObject
{
protected:
	Marker *marker;
	MarkerInfo markerInfo;
	QList<MarkerFoot> footList;

public:
	double distanceXZFinal;
	double heightBegin;
	double heightFinal;
	//double heightMax;

public:
	MarkerFootTranform(Marker *marker,MarkerInfo &markerInfo,GfxClip *gfxClip,bool isLeft);
	~MarkerFootTranform();

	Marker *GetMarker(){return marker; }
	int FootCount(){return footList.count();}
	MarkerFoot *GetFoot(int idx){return &footList[idx];} 

	void SetHOffset(double h);
	void SetPosition(int idx,double dist,double height);
	void Resize(double distFactor,double heightFactor);
};
#include "Anima_nsend.h"