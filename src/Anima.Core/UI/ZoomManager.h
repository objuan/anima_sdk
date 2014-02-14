#pragma once

#include "Anima_nsbegin.h"
enum ZoomManagerMode
{
	ZoomManagerMode_Frames=0,
	ZoomManagerMode_Seconds
};

class Film;

class ANIMA_CORE_API ZoomManager: public BObject
{
private:

	Film *film;

	int width;
	int startValue;
	int endValue;
	int minValue;
	int maxValue;
	int currentValue;
	double maxBoundFrameValue;

	ZoomManagerMode mode;

private:
	double GetValue(int frame);
public:
	ZoomManager();

	int GetFrame(double value);

	ZoomManagerMode GetMode(){return mode;}
	void SetMode(ZoomManagerMode mode);

	void Refresh();
	void Load(Film *film);

	double GetDisplayFactor();

	void SetWidth(int width);
	bool IsValid();

	int GetDecimals();
	double GetSingleStep();

	int GetCurrentValue();
	void SetCurrentValue(int val);

	int GetStartValue();
	void SetStartValue(int val);

	int GetEndValue();
	void SetEndValue(int val);

	void MoveValues(int val);

	int GetMaxValue();
	//void SetMaxValue(int val);
	int GetMinValue();

	double GetEndFrameBoundValue();
	void SetEndFrameBoundValue(double val);
};
#include "Anima_nsend.h"