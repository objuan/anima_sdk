#pragma once

#include "Anima_nsbegin.h"

class GfxClipAnim;

struct GfxClipAnimCacheEntry
{
	GfxClipAnim *anim;
	bool isLive;
	GfxClipAnimCacheEntry *prev;
	GfxClipAnimCacheEntry *next;
};

class ANIMA_CORE_API GfxClipAnimCache  : public BObject
{
private:

	QMap<GfxClipAnim * , GfxClipAnimCacheEntry *> entryList;
	bool isBuilding;
	GfxClipAnimCacheEntry *first;
	GfxClipAnimCacheEntry *last;

public:

	GfxClipAnimCache(void);
	~GfxClipAnimCache(void);

	void Add(GfxClipAnim *anim);
	void Remove(GfxClipAnim *anim);

	void BeginRender();
	void EndRender();
	void NotifyRender(GfxClipAnim *anim);

	// interfaccia per le animazioni sulel mesh
	//void EnsureAnimation(Ogre::Mesh *mesh,Ogre::String clipID);

	int GetMemoryUsage();
	int GetLiveMemoryUsage();
	int GetLiveObjects();
	int GetTotalObjects();
};


#include "Anima_nsend.h"