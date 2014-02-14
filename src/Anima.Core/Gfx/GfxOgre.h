#pragma once

#include "Anima_nsbegin.h"

class ANIMA_CORE_API GfxOgre : public Ogre::RenderTargetListener
{
public:
	static GfxOgre Singleton;

private:
	Ogre::Root *root;
	Ogre::SceneManager* sceneMgr;
	Ogre::Camera* camera;
	Ogre::RenderTarget *renderTarget;
	Ogre::RenderWindow *renderWindow;
	Ogre::Viewport* vp;

	//Entity
	Ogre::Entity* mainEnt;
	Ogre::AnimationState* mainEntAnim;
	Ogre::SceneNode* mainNode;
	Ogre::SubEntity* mainSubEnt;

	//Light
	Ogre::Light* mainLight;
	Ogre::ColourValue lightDiffuseColor;
	Ogre::ColourValue lightSpecularColor;
	Ogre::ColourValue ambientLightColor;
	QColor ambientOldColor;
	QColor diffuseOldColor;
	QColor specularOldColor;
	///////

	//Fog
	Ogre::ColourValue fogColor;
	QColor fogOldColor;//to keep the last selected color
	Ogre::ColourValue backgroundColor;
	QColor backgroundOldColor;
	/////

	//Ogre Stats
	int FPS;
	int triangles;

private:
	void SetupResources();

	GfxOgre(void);
	~GfxOgre(void);

	void CreateOfflineBuffer();

public:

	void Init();
	void InitUI(Ogre::RenderWindow *renderWindow);

	void OnResize();

	void Update();

	//Control the Scene
	void addObject(Ogre::String);

	// camera
	
	Ogre::Vector3 getPosCamera();
	Ogre::Vector3 getDirCamera();
	Ogre::Camera* getCamera();
	Ogre::Viewport* getViewport();


	//-->Fog
	void createPhys();
	void createFog();
	void createLight();
	void createScene();

	void setFog(int);
	void setFogColor(QColor);
	QColor getFogColor();

	//-->Background
	void setBackgroundColor(QColor);
	QColor getBackgroundColor();

	//-->Shadow
	void setShadow(int);

	//-->Light
	void setAmbientLight(QColor);
	QColor getAmbientLight();
	void setDiffuseColor(QColor);
	QColor getDiffuseColor();
	void setSpecularColor(QColor);
	QColor getSpecularColor();
	void createPointLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xPos, yPos, zPos, diffuse color, specular color
	void createDirectionalLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xDir, yDir, zDir, diffuse color, specular color
	void createSpotlight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xPos, yPos, zPos, xDir, yDir, zDir, diffuse color, specular color
	void updatePointLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xPos, yPos, zPos, diffuse color, specular color
	void updateDirectionalLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xDir, yDir, zDir, diffuse color, specular color
	void updateSpotlight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xPos, yPos, zPos, xDir, yDir, zDir, diffuse color, specular color
	void deleteLight(Ogre::String);//name

	//-->Display
	void setRenderDetail(int);
	void setLOD(int);
	void setBoundingBoxes(int);
	void setSkeleton(int);

	//-->Material
	void updateMaterial();

	//-->Animation
	void setAnimationState(Ogre::String);
	void setAnimLoop(bool);
	void setAnimEnabled(bool);

	Ogre::Entity* getMainEnt();
	Ogre::SceneNode* getMainNode();
	Ogre::SceneManager* getSceneManager(){return sceneMgr;}
	float getFPS();
	float getTriangles();
	void updateStats();

	// -------------

	virtual void  preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt) ;
	virtual void  postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
	//virtual void  preViewportUpdate (const RenderTargetViewportEvent &evt) 
	//virtual void  postViewportUpdate (const RenderTargetViewportEvent &evt) 
	//virtual void  viewportAdded (const RenderTargetViewportEvent &evt) 
	//Called to notify listener that a Viewport has been added to the target in question. 
	// 
	//virtual void  viewportRemoved (const RenderTargetViewportEvent &evt) 
 // Called to notify listener that a Viewport has been removed from the target in question. 
};
#include "Anima_nsend.h"