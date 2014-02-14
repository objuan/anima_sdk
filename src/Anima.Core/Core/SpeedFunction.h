#pragma once
//#include "CoreFunction.h"
#include "Anima_nsbegin.h"
class Animation;
class AnimationType;
class AnimationPart;
class AnimClipModel;
class ModelInstance;
class CoreFunction;
class KeySpeedFunction;

//struct SpeedFunctionEntry
//{
//	//int frame;
//	double speed;
//	SpeedFunctionEntry(){}
//};

class ANIMA_CORE_API SpeedFunction: public BObject {
public:
	enum SpeedFunctionEditMode
	{
		SpeedFunctionEditMode_Constant,
		SpeedFunctionEditMode_User
	//	CoreFunctionEditMode_Auto
	};
public:
	Animation *anim;
	ModelInstance *modelInstance;
	float* fun;
	int frameCount;
	double speedMult;

public:
	SpeedFunction(Animation *anim,ModelInstance *modelInstance);
	virtual ~SpeedFunction();

	float *GetFun(){return fun;}

	void _SetSpeed(double mult);
	double GetSpeed(){return speedMult;}
	void SetSpeed(double speed);

	virtual bool IsEditable(){return false;}
        virtual SpeedFunctionEditMode GetEditMode(){return SpeedFunction::SpeedFunctionEditMode_Constant; }

	void Clear();
	void SetCount(int count);
	void Set(int frame,double speed);
	int GetFrameCount(){return frameCount;}

	virtual QString Serialize();
	void ResumeFromSerial(QString &ser,QList<double> &frameSpeed,int frameCount);
	virtual void ResumeFromSerial(QString &);

	//virtual void InvalidateBuild(int type=0);

	virtual void Dump();
};


#include "Anima_nsend.h"
