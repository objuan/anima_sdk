#pragma once

#include "Anima_nsbegin.h"

class ANIMA_CORE_API GfxScene
{
public:
	QList<Ogre::Light *> lightList;
public:
	GfxScene(void);
	virtual ~GfxScene(void);

private:
	void LoadLights(Ogre::SceneManager *sceneMgr,QDomElement &ele);
public:

	Ogre::Light *GetLight(Ogre::SceneManager *sceneMgr,const Ogre::String &name);

	void LoadSceneCamera(Ogre::SceneManager *sceneMgr);
	void Load(Ogre::SceneManager *sceneMgr);
	void LoadScene(Ogre::SceneManager *sceneMgr,QDomElement &ele);

	void Clear(Ogre::SceneManager *sceneMgr);
	
};
#include "Anima_nsend.h"