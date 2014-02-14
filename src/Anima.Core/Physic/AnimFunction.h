#pragma once
#include "Anima_nsbegin.h"

struct StepInfo
{
	int startTime;
	int endTime;
};

class AnimFunction {

private:

	double minVal;
	double maxVal;
	int size;
	int time;
	double *values;
	double *newValues;
	double oldValue;
	int stepCount;
	bool upDir;
	bool startWithStep;
	bool beginStep;

	int passi;
	int firstTime;
	AList<StepInfo> stepList;

public:
	double passoW;
public:
	AnimFunction(int size,bool beginStep,double passoW=-1);
	~AnimFunction();

	void Add(double value);
	void End();

	bool GetStartWithStep(){return startWithStep;}
	double GetOffset(int t);
	double GetOldValue(int t);
	double GetNewValue(int t);

	void Resize(double deltaValue);
	void Dump();
	QString Dump(int t);
};
#include "Anima_nsend.h"