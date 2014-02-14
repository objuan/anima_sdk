#pragma once

#include "BodyModder.h"
#include "Anima_nsbegin.h"
class StairMovePath;
class Traker;


class BodyModderStairBase : public BodyModder
{
protected:
	bool built;
	QList<Ogre::Vector3> footPositions;

protected:
	// all ini
	virtual void OnLoad(){}
	virtual void OnBuild(){}
public:
	BodyModderStairBase(StairMovePath *movePath,GfxClip *clip);


	virtual void Build(int subClipIdx,const QString animSlotName); // ALTRO THREAD
};

class BodyModderStairFirstStep : public BodyModderStairBase
{
private:

private:
	virtual void OnLoad();
	virtual void OnBuild();
public:
	virtual QString Type(){return "FirstStep";};

	BodyModderStairFirstStep(StairMovePath *movePath,GfxClip *clip);
};


class BodyModderStair : public BodyModderStairBase
{
private:

	bool haveSmallClip;
	bool haveOneStep;
	int clipCount;

private:
	virtual void OnLoad();
	virtual void OnBuild();
	virtual void OnBuild(bool infoPhase);
public:

	virtual QString Type(){return "Stair";};
	BodyModderStair(StairMovePath *movePath,GfxClip *clip);

};


class BodyModderStairLastStep : public BodyModderStairBase
{
private:

private:
	virtual void OnLoad();
	virtual void OnBuild();

	StairMovePath *getPath();
public:
	BodyModderStairLastStep(StairMovePath *movePath,GfxClip *clip);
	virtual QString Type(){return "LastStep";};
};
#include "Anima_nsend.h"