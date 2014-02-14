#pragma once

#include "Anima.Core.h"
#include "GfxRenderRecord.h"
#include "./Render/OgreRenderer.h"


#include "Anima_nsbegin.h"

class OgreGridPlane;
class GfxClipAnim;
class BaseModel;
class Model;
class GfxBaseModel;
class AnimClip;
class ModelGroup;
class View;
class SelectionRectangleOverlayFactory;
class GfxLabelManager;
class GfxZoomManager;
class IModelRenderer;
class GfxClipAnimCache;
class GfxPerformanceManager;
class GfxScene;
class VideoManager;

// render queues
#define RENDER_QUEUE_OUTLINE_GLOW_OBJECTS   Ogre::RENDER_QUEUE_6 + 1
#define RENDER_QUEUE_OUTLINE_GLOW_GLOWS      Ogre::RENDER_QUEUE_6 + 2
#define RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW   Ogre::RENDER_QUEUE_6 + 3
#define RENDER_QUEUE_FULL_GLOW_GLOW         Ogre::RENDER_QUEUE_6 + 4
#define LAST_STENCIL_OP_RENDER_QUEUE      RENDER_QUEUE_FULL_GLOW_GLOW

class GfxContextEntry: public BObject
{
public:
	ModelGroup *modelGroup;
	QList<AnimClip *> clipList;
	GfxContextEntry(ModelGroup *modelGroup): modelGroup(modelGroup){}
};

struct GfxBaseModelInfo
{
	BaseModel *baseModel;
	GfxBaseModel *gfxModel;
	int reference;
};

struct GfxModelInfo
{
	Model *model;
	Ogre::Entity *entity;
	int reference;
};

class ANIMA_CORE_API GfxContextScreenGrab: public BObject
{
private:
	uchar *data ;
	QImage image;
	QPixmap pixmap;
	QPixmap *src_pixmap;
private:
	void Resize(int dst_w,int dst_h);
public:
	GfxContextScreenGrab(uchar *data,int src_w,int src_h,int dst_w,int dst_h,QImage::Format format );
	GfxContextScreenGrab(const QString &filePath,int dst_w,int dst_h );
	~GfxContextScreenGrab(void);

	QImage &GetImage(){return image;}
	QPixmap &GetPixmap(){return pixmap;}
	bool IsNull();
};


class ANIMA_CORE_API GfxContext : public Ogre::RenderTargetListener ,public Anima::OgreRenderer
{
public:
	static GfxContext *Singleton;

	enum GfxContextViewMode
	{
		Normal = 0,
		Admim = 1
	//	Physic
	};

private:

	bool disposed;
	QHash<BaseModel *,GfxBaseModelInfo *> currentBaseModels;
	QHash<Model *,GfxModelInfo *> currentModels;
	GfxClipAnimCache *clipAnimCache;
	GfxPerformanceManager *performanceManager;
	VideoManager *videoManager;
#ifndef OGRE_VERSION_17
    Ogre::OverlaySystem *mOverlaySystem ;
#endif
	IModelRenderer *modelRenderer;
	// ---------------------

	Ogre::Root *root;
	Ogre::SceneManager* sceneMgr;
	Ogre::RenderTarget *renderTarget;
	Ogre::RenderWindow *renderWindow;
	Ogre::RenderWindow *renderWindowPhysic;
	Ogre::RenderSystem* renderSystem;
	Ogre::TexturePtr rtt_tump_texture;
	//Ogre::TexturePtr rtt_preview_texture;

	//Entity
	Ogre::Entity* mainEnt;
	Ogre::AnimationState* mainEntAnim;
	Ogre::SceneNode* mainNode;
	Ogre::SubEntity* mainSubEnt;

	//Ogre Stats
	int FPS;
	int triangles;
	GfxContextViewMode viewMode;
	GfxLabelManager *labelManager;
	GfxZoomManager *zoomManager;

	GfxScene *gfxScene;

	OgreGridPlane *ogreGridPlane;

	// stats
	
	QTime updateTime;
	QTime perfTimer;
	GfxRenderRecord currentRecord;

    SelectionRectangleOverlayFactory * selectionRectOverlayFactory;

	bool haveShades;
	bool showSkeleton;
	bool invalidCache;
	bool invalidPerformance;
	bool renderMode;
	QStringList updateCommandList;

	// GFX
	int fsaa;
	static int renderDriverMode; // 0 = opengl, 1 = directx9, 2 = directx10, 3 = directx11

private:
	void SetupResources();
	void CreateOfflineBuffer();
	void Init(IModelRenderer *modelRenderer);;

public:

	GfxContext(void);
	~GfxContext(void);

	bool IsDisposed(){return disposed;}

	int GetFSAA(){return fsaa;}
	void SetFSAA(int fsaa){this->fsaa=fsaa;}

	Ogre::RenderWindow *getRenderWindow(){return renderWindow;}

	static int GetRenderDriverMode(){return renderDriverMode;}
	static void SetRenderDriverMode(int _renderDriverMode){renderDriverMode=_renderDriverMode;}

	static void GfxContext_New(IModelRenderer *modelRenderer);
	static void Dispose();

	void OpenWIN(unsigned long winID,int width,int height);
	void InitUI(Ogre::RenderWindow *renderWindow,Ogre::RenderWindow *renderWindowPhysic);
	void End();

	void InvalidateCache();
//	void OnResize();
	void UpdateFromGUI();
	void Update();
	void UpdateAndWaitTask();
	void UpdateAndContinue();
	void WaitTask();

	IModelRenderer *GetModelRenderer(){return modelRenderer;}
	Ogre::Entity *GetModelEntity(Model *model);
	GfxClipAnimCache *GetClipAnimCache(){return clipAnimCache;}
	GfxPerformanceManager *GetPerformanceManager(){return performanceManager;}
	VideoManager *GetVideoManager(){return videoManager;}
	Ogre::RenderTarget *GetRenderTarget(){return renderTarget;}

	GfxLabelManager *GetLabelManager(){return labelManager;}
	GfxZoomManager *GetZoomManager(){return zoomManager;}

	void SetupShadow();

	//void SetWindowMode(GfxContextWindowMode mode);
	//Control the Scene
	void addObject(Ogre::String);
	virtual void OnTimerTick(int timeMilliseconds);

	bool GetHaveShades(){return haveShades;}

	Ogre::RenderSystem* GetRenderSystem(){return renderSystem;}

	// -------- VIEW ----

	GfxContextViewMode GetViewMode(){return viewMode;}
//	GfxContextWindowMode GetWindowMode(){return windowMode;}
	void SetViewMove(GfxContextViewMode viewMode);
	bool ShowSkeleton(){return showSkeleton;}
	void SetShowSkeleton(bool _showSkeleton){showSkeleton=_showSkeleton;}

	void GetCameraList(QList<Ogre::Camera *> &list);
	Ogre::Camera *GetCamera(const Ogre::String &name);
	Ogre::Camera *GetCamera(int x,int y);
	Ogre::Camera *GetCamera(int idx);
	void ZoomAll();

	void AddUpdateCommand(const QString &cmd);

	// ----

	void SetRenderedClip(MovePath *path,double startSpace,ModelInstance *mi,GfxClipAnim *clip,double time,double innerTime);
	GfxRenderRecord GetCurrentRenderRecord(){return currentRecord;}
	
	// ritorna un box a schermo
	void BoxToScreenCoordinates( Ogre::Vector3 *position,double *w,double *h);

	void ClearScene();
	void SetupViews(int layout);

	void DestroyNode(Ogre::SceneNode *n);
	void DestroyMesh(const Ogre::String &name);
	void DestroyEntity(Ogre::Entity* ent,bool releaseMesh=false);

	// ------------------

	//-->Display
	void setRenderDetail(int);
	void setLOD(int);
	void setBoundingBoxes(int);

	Ogre::Entity* getMainEnt();
	Ogre::SceneNode* getMainNode();
	Ogre::SceneManager* getSceneManager(){return sceneMgr;}
	float getFPS();
	float getTriangles();
	void updateStats();

	void GrabScreenToFile(const QString &filePath);
	GfxContextScreenGrab *GrabScreen(bool forTumb,int imageWidth=-1,int imageHeight=-1,const QString &filePrefix="");

	// -------------

	inline bool IsRenderMode(){return renderMode;}
	void SetRenderMode(bool renderMode);

	virtual void  preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt) ;
	virtual void  postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);
	
	void OnAddModel(Model *model);
	void OnRemoveModel(Model *model);

	// debug 
	void Dump();
};
#include "Anima_nsend.h"
