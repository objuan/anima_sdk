#pragma once
#ifdef WIN32
#ifdef ANIMA_CORE_EXPORTS
#define ANIMA_CORE_API __declspec(dllexport)
#else
#define ANIMA_CORE_API __declspec(dllimport)
#endif
#else
#define ANIMA_CORE_API
#endif

#ifndef MAX
#define MAX(a,b) (a>=b) ? a : b
#endif
#ifndef MIN
#define MIN(a,b) (a<=b) ? a : b
#endif

#define HIT_CODE_MODEL 100
#define HIT_CODE_FILM 200
#define HIT_CODE_EDIT 300
#define HIT_CODE_GRID 400
#define HIT_CODE_FILM_LAYER 210
#define HIT_CODE_ANIMATION 220
#define HIT_CODE_ANIMATION_PART 230
#define HIT_CODE_ANIMATION_SPLINE 300
#define HIT_CODE_ANIMATION_STAIR 400
#define HIT_CODE_ANIMATION_LINK 500

#define HIT_CODE_ANIMATION_POINT 310
#define HIT_CODE_ANIMATION_CLIP 320
#define HIT_CODE_ANIMATION_CLIP_LIMIT 330
//#define HIT_CODE_ANIMATION_PATH_OFFSET_MARK 340

#define HIT_CODE_ANIMATION_STAIR_POINT 410

#define HIT_CODE_EDIT_SPACE_MARK 510

#define HIT_CODE_ANIMATION_MODEL 600

#define HIT_CODE_SCENE_OBJECT 700
#define HIT_CODE_SCENE_OBJECT_MOVE 710
#define HIT_CODE_SCENE_OBJECT_MOVE_X 720
#define HIT_CODE_SCENE_OBJECT_MOVE_Z 730
#define HIT_CODE_SCENE_OBJECT_ROTATE 740

//#include ".\Model.h"
#include "stdafx.h"
#include "Core/Edit/SelectionManager.h"

#include "Anima_nsbegin.h"

class ModelLibrary;
class ClipLibrary;
class Animation;
class Film;
class IMesh;
class MeshStatic;
class EditorManager;
class Animation;
class GUIManager;
class SelectionManager;
class ActionLibrary;
class TaskQueue;
class CollisionObjectStorage;
class GeneralParams;
class BuildDispatcher;
class GfxContext;
class ModelIA;
class AddOnManager;
class PresetsExporterManager;
class SceneLibraryManager;

//class ANIMA_CORE_API ContextDebug
//{
//public:
//
//	double morph_x_time_prev;
//	double morph_x_time_next;
//	double morph_x_path_time_prev;
//	double morph_x_path_time_next;
//
//	ContextDebug(){
//		morph_x_path_time_prev=morph_x_path_time_next=morph_x_time_prev=morph_x_time_next=0.1;
//	}
//	void SetClipMorph(double morph_x_time_prev,double morph_x_time_next);
//	void SetPathMorph(double morph_x_time_prev,double morph_x_time_next);
//};

class ANIMA_CORE_API UnitMeterUtil 
{
public:
	QStringList unitList;
	QStringList unitListDesc;
	QStringList unitListSpeedDesc;
	QStringList unitListAngleDesc;

public:
	
	UnitMeterUtil();
        static QString GetDesc(UnitMeter type);
        static QString GetSpeedDesc(UnitMeter type);
	static QStringList &GetDescList();

        static UnitMeter GetActualMeter(int misureScale);

	static double ConvertSpaceToUI(double value,int misureScale=0);
	static double ConvertSpaceToAnima(double value,int misureScale=0);

	static double ConvertAngleToUI(double value);
	static double ConvertAngleToAnima(double value);

        static double ScaleTxt(UnitMeter from);
	static int GetDecimal(int misureScale=0);

	static QString GetSpaceUI(double value,int misureScale=0);
	static QString GetSpeedUI(double value,int misureScale=0);
	static QString GetAngleUI(double value);

	static QString GetSpaceUIDesc(double value);
	//QString GetSpaceUIDesc(int value);
	//QString GetSpeedUIDesc(double value);
	//QString GetSpeedUIDesc(int value);
	static QString GetSpaceUIUnit(int misureScale=0);
	static QString GetSpeedUIUnit(int misureScale=0);
	static QString GetAngleUIUnit();
};

class ANIMA_CORE_API RenderPresetInfo : public QObject 
{
public:
	QString Preset;
	QString Folder;
	QString Downscale;
	QString Mode;
};

class ANIMA_CORE_API RenderInfo : public QObject 
{
public:
	QList<RenderPresetInfo *> presetInfo;
	RenderPresetInfo *CurrentPreset;
	void SetPreset(const QString &preset);
	RenderInfo(){}
	~RenderInfo(){qDeleteAll(presetInfo);}
};

class ANIMA_CORE_API Context : public QObject 
{
	Q_OBJECT
private:
	
	Ogre::Log *ogreLog;
	Ogre::Log *debugLog;
	Ogre::Log *collisionLog;
	QWidget *meshWidget;
	QWidget *mainWindow;
	QStringList startupMessageList;

	bool disposed;
	ModelLibrary *modelLibrary;
	ClipLibrary *clipLibrary;
	ActionLibrary *actionLibrary;
	TaskQueue *gfxBuildQueue;
	CollisionObjectStorage *collisionObjectStorage;
	SelectionManager* selectionManager;
	EditorManager* editorManager;
	RenderInfo *renderInfo;
	PresetsExporterManager *presetsExporterManager;
	SceneLibraryManager *sceneLibraryManager;

	GUIManager *guiManager;
	GeneralParams *generalParams;
	ModelIA *modelIA;
	AddOnManager *addOnManager;

	// mesh
	QList<MeshStatic *> backgroundMeshList;
	//QList<Mesh *> renderMeshList;
	
	bool isPlugin;
	Film *film;
	QTime tickTime;
	bool needGLUpdate;
	bool needGLUpdateFromIK;
	int ikTimeTick;
	bool needGLPhysicUpdate;
	int meshIdx;
	bool loading;
//	ContextDebug contextDebug;
	BuildDispatcher *buildDispatcher;
	// debug
	long memoryUsage;	
	long totalObjects;
	bool disposing;

public:

	static Context Singleton;

	// mesh

	Ogre::Vector3 lastLoadedMeshRot;
	QString lastLoadedMeshPath;
	UnitMeter lastLoadedMeshUnit;
	bool isDefaultMesh;

private:
	Context(void);
	~Context(void);

	void _Init();
public:
	void Init(bool isPlugin);
	void InitUI(QWidget *mainWindow,QWidget *meshWidget,GUIManager *guiManager,Ogre::RenderWindow *renderWindow,Ogre::RenderWindow *renderWindowPhysic);
	void End();

	void Abort(); // abortisce i threads
	bool IsDisposing(){return disposing;}

	inline bool IsPlugin(){return isPlugin;}
	
	bool IsDisposed(){return disposed;}
	long GetMemoryUsage(){return memoryUsage;}
	long GetTotalObjects(){return totalObjects;}
	QString DumpLastObjects();
	void ClearLastObjects();
	void AddMemoryUsage(int usage,int _totalObj){memoryUsage+=usage;totalObjects=_totalObj;}

	ModelIA *GetIA(){return modelIA;}

	GeneralParams *GetGeneralParams(){return generalParams;}
	BuildDispatcher *GetBuildDispatcher(){return buildDispatcher;}

	void Clear();

	void StartLoading();
	void EndLoading();
	bool IsLoading(){return loading;}

	bool HaveFilm(){return film!=NULL;}

	Ogre::Log *GetCollisionLog(){return collisionLog;}
	Ogre::Log *GetDebugLog(){return debugLog;}
	GUIManager &GetGuiManager(){return *guiManager;}
	ModelLibrary &GetModelLibrary(){return *modelLibrary;}
	ClipLibrary &GetClipLibrary(){return *clipLibrary;}
	ActionLibrary &GetActionLibrary(){return *actionLibrary;}
	QList<MeshStatic *> &GetBackgroundMeshList(){return backgroundMeshList;}
//	QList<Mesh *> &GetRenderMeshList(){return renderMeshList;}
	TaskQueue *GetGfxBuildQueue(){return gfxBuildQueue;}
	CollisionObjectStorage *GetCollisionObjectStorage(){return collisionObjectStorage;}
	AddOnManager *GetAddOnManager(){return addOnManager;}
	RenderInfo *GetRenderInfo(){return renderInfo;}
	PresetsExporterManager *GetPresetsExporterManager(){return presetsExporterManager;}
	SceneLibraryManager *GetSceneLibraryManager(){return sceneLibraryManager;}
	
	Film &GetFilm(){return *film;}

	void FillAnimationList(QList<Animation *> &animList);

	MeshStatic *ImportBackgroundMesh(const QString &filePath,UnitMeter unit,Ogre::Vector3 &rot);
	void DeleteBackgroundMesh(MeshStatic *mesh);
	void ClearMesh();
	void DrawBackgroundModels(bool selectMode);
	void LoadDefaultMesh();
	MeshStatic *GetUserBackgroundMesh();
	MeshStatic *GetBackgroundMesh();

	//void OnEditManagerOperationChanged();

	void OnLoad(bool onlyAnim=false);
	void UpdateGfx(bool fromIK=false);
	void ForceUIUpdate();
	void ForceIARebuild();

	void UpdatePhysicGfx();
	void OnTimerTick();

	void AddLog(const QString &log);

	void AddStartupMesssage(const QString &message);

	// memory

	void NotifyDelete(SceneObject *obj);

signals:

	void BackgroundMeshListChanged();
	void UpdatingGL();
	void UpdatingPhysicGL();

private slots:
	 void SelectionChanged();
	 void CurrentFrameChanged();
};
#include "Anima_nsend.h"
