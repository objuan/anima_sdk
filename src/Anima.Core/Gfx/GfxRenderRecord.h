#pragma once

#include "Anima_nsbegin.h"
//class GfxModel;
class GfxClip;
class MovePath;
class GfxClipAnim;
class ModelInstance;
class GfxModel;

class ANIMA_CORE_API GfxRenderRecord: public BObject
{

private:

	ModelInstance *mi;
	GfxModel *model;
	GfxClipAnim *clip;
	MovePath *path;
	double startSpace;
	double innerTime;
	double time;

public:
	/*double start;
	double end;*/

public:

	GfxRenderRecord();
	GfxRenderRecord(MovePath *path,double startSpace,ModelInstance *model,GfxClipAnim *clip,double time,double innerTime);
	~GfxRenderRecord(void);

	ModelInstance *GetModelInstance(){return mi;}
	double GetStartSpace(){return startSpace;}
	double GetInnerTime(){return innerTime;}
	double GetTime(){return time;}
	MovePath *GetPath(){return path;}
	GfxModel *GetModel(){return model;}
	GfxClipAnim *GetGfxClipAnim(){return clip;}

	
};
#include "Anima_nsend.h"