#pragma once

#include "SplineMovePath.h"
#include "StairMovePath.h"

class ModelInstance;

class ANIMA_CORE_API LinkSplineMovePath: public SplineMovePath
{
private:
	SplineMovePath *movePath;
	ModelInstance *modelInstance;

	void GetDir90(int fullPointIdx,KFbxVector4 &dirXZ,KFbxVector4 &dirXZ90);

public:
	LinkSplineMovePath(SplineMovePath *movePath,ModelInstance *modelInstance);
	~LinkSplineMovePath(void);

	virtual MovePath *GetBasePath();
	ModelInstance *GetModel(){return modelInstance; }

	void FireChangedEvent();
	void OnBasePathChanged();

	virtual void Build();
	virtual void DrawLine(bool selectMode,bool curveSelected,int pointIdxSelected,ModelInstance *modelInstance);
};

class ANIMA_CORE_API LinkStairMovePath: public StairMovePath
{
private:
	StairMovePath *movePath;
	ModelInstance *modelInstance;

public:
	LinkStairMovePath(StairMovePath *movePath,ModelInstance *modelInstance);
	~LinkStairMovePath(void);

	virtual MovePath *GetBasePath();
	ModelInstance *GetModel(){return modelInstance; }

	void FireChangedEvent();
	void OnBasePathChanged();
	virtual void Draw(bool selectMode,ModelInstance *modelInstance);
};
