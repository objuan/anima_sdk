#pragma once

#include "Anima_nsbegin.h"
class Model;
class ModelInstance;
class AnimClipModelGfxProperty;
class AnimClip;
class OBBoxRenderable;
class SceneObjectGfxMesh;

class OBBoxRenderableEntry: public BObject
{
public:
	OBBoxRenderable *box;
	OBBoxRenderableEntry *prev;
	OBBoxRenderableEntry *next;
};

enum GfxPerformanceManager_ViewMode
{
	GfxPerformanceManager_ViewMode_SOLID = 0,
	GfxPerformanceManager_ViewMode_BBOX = 1
};

class ANIMA_CORE_API GfxPerformanceManager: public BObject
{
private:
	//QList<OBBoxRenderable *> bboxList;
	OBBoxRenderableEntry *first;
	OBBoxRenderableEntry *last;
	int maxModelCount;
	double gizmosZoom;
	GfxPerformanceManager_ViewMode viewMode;

	int activeBBoxCount;

	QList<int> lastRenderTime;

private:
//	void EnsureNumber(int count);
		
public:
	
	GfxPerformanceManager();
	~GfxPerformanceManager();

	void InitGfx();

	void BeginRender(bool previewRenderMode);
	void DelBBoxRender(SceneObjectGfxMesh *mesh);
	void EndRender();

	OBBoxRenderableEntry *Push();
	void Pop(OBBoxRenderableEntry *);


	GfxPerformanceManager_ViewMode GetViewMode(){return viewMode;}
	void SetViewMode(GfxPerformanceManager_ViewMode mode);

	int GetMaxModelCount(){return maxModelCount;}
	void SetMaxModelCount(int maxModelCount);

	double GetGizmosZoom(){return gizmosZoom;}
	void SetGizmosZoom(double value);

	void NotifyRenderTime(int time);
};

#include "Anima_nsend.h"
