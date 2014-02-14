#pragma once

#include "BodyModder.h"

class StairMovePath;
class Traker;

struct FeetInfo
{
	double y;
	double dist;
	double fromT;
	double toT;

	FeetInfo(){}
	FeetInfo(double _y,double _dist,double _fromT,double _toT)
	{
		y=_y;
		dist=_dist;
		fromT=_fromT;
		toT=_toT;
	}
};

struct OffsetFunctionEntry
{
	int deltaTime;
	bool doScale;

	double distStart;
	double distEnd;
	double delta;
	double distOffset;
};

class OffsetFunction
{
public:
	QList<OffsetFunctionEntry> entryList;

	OffsetFunction(){}
	
	void SetContant(int finalTime,double deltaValue);
	void SetLeft(FeetInfo feetInfo,int finalTime,double delta,double distBeforeFinal,double distAfterFinal);
	void SetLeftUp(FeetInfo feetInfo,int finalTime,double delta,double distBeforeFinal,double distAfterFinal);
	void SetRight(FeetInfo feetInfo,int finalTime,double delta,double distBeforeFinal,double distAfterFinal);

	void AddInterval(int deltaTime,bool doScale,double distStart,double distEnd,double delta,double distOffset);
	double GetValue(int time,double dist);
};

class NodeBoundInfo
{
public:
//	KFbxVector4 rootStart;
	KFbxVector4 hipStart;
	KFbxVector4 hipEnd;
	KFbxVector4 leftStart;
	KFbxVector4 rightStart;
	KFbxVector4 leftEnd;
	KFbxVector4 rightEnd;
	double hipStartY;
	double hipEndY;
	double leftStartY;
	double leftEndY;
	double rightStartY;
	double rightEndY;
};

class YOffsetFuntion
{
private:
	double planeRef;
	double scale;
public:
	void Set(double startValue,double endValue,double delta);
	double GetValue(int time,double val);
};

class BodyModderStair : public BodyModder
{
private:
	StairMovePath *path;
	bool initialized;

	Traker *traker;
	QList<KFbxVector4> offsetLeg[2][2];

	bool doHipH;
	bool doLeftH;
	bool doRightH;
	double yDelta;

	int startFootCount;
	QList<KFbxVector2> footPositions;
	bool haveHalf;
	double clipTime;
	int clipCount;
	double clipSpace;

	double subClipSpace;
	double subClipHeight;
	double heightDelta;

private:
	void Init();

private:
	
	void BuildClip(GfxClip *clip,double space,double height);
	NodeBoundInfo ComputeNodeBounds();

public:
	BodyModderStair(GfxClip *clip,StairMovePath *path);

	double GetClipCount(){return (double)clipCount + ((haveHalf)? 0.5:0);}
	double GetHeightDelta(){return heightDelta;}

	virtual int GetSubClipCount();
	virtual void GetClipInfo(int subClipIdx,double &space,double &height,double &time,int &clipCount);
	virtual void Build(int subClipIdx,const QString animSlotName);
};


