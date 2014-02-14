#pragma once

class GfxClipAnim;

class ANIMA_CORE_API GfxClipAnimTask
{
private:
	
	GfxClipAnim *gfxClipAnim;

public:
	GfxClipAnimTask(GfxClipAnim *gfxClipAnim);
	virtual ~GfxClipAnimTask();
};

