#pragma once

#include "Anima_nsbegin.h"
class FilmLayer;
class Animation;
class AnimationPart;
//class GfxBaseModel;
class Model;
class BaseModel;
class ModelInstance;
class SelectionManagerGfx;
class ContextIO;
class SceneObjectMovable;
class AnimaFileInfo;
//void (* FireModelDrawEventHandler)(GfxModel *);

enum FilmEventType
{
	FilmEventType_FireFilmLayer,
	/*FilmEventType_FireAnimationEvent,
	FilmEventType_FireAnimationPartEvent,
	FilmEventType_FireAnimationModelChanged,
	FilmEventType_FireAnimationPartChanged,
	FilmEventType_FireAnimationChanged,
	FilmEventType_FireEnabledChanged,*/
	FilmEventType_CurrentFrameChanged,
	FilmEventType_ChangedUI,
	FilmEventType_FireBoundFramesChanged,
	FilmEventType_FirePropertyChangedEvent,
	FilmEventType_FireMovableChangedEvent,
	FilmEventType_SceneObjectDeleting
};

struct FilmEvent
{
	FilmEventType type;
	SceneObject *obj;
	char m;

	FilmEvent(FilmEventType type,SceneObject *obj) :obj(obj),type(type),m('u'){
	}
	FilmEvent(FilmEventType type,SceneObject *obj,char m) :obj(obj),type(type),m(m)
	{
	}
};

#ifndef WIN32
#define _stdcall
#endif
typedef void (_stdcall *FPSTickHandler)(int);


class ANIMA_CORE_API Film : public QObject  , public BObject 
{
	Q_OBJECT
        friend class ContextIO;

public:

	enum FilmDrawMode
	{
		DrawMode_Full,
		DrawMode_Points
	};

	enum FilmPlaybackLoopMode
	{
		On = 0,
		Off = 1,
	};
    
private:
    //used for automatic coloring of layers
    QList<QColor> layersStandartColors;
    int nextNewLayerColor;

	QList<FilmLayer *> filmLayers;
	void (* fireModelDrawEventHandler)(ModelInstance *);
	FilmDrawMode drawMode;
	SelectionManagerGfx *selectionManagerGfx;

    int startPlaybackFrame, endPlaybackFrame;
    bool usePlaybackBounds;
	bool wasStartPlaybackFrameBound;
	bool wasEndPlaybackFrameBound;
	QList<FilmEvent *> eventStack;

	FPSTickHandler *fpsTickHandler;

	double _startTimeBound,_endTimeBound;
	bool _invalidateInfo;
	AnimaFileInfo *info;
	QString animFileVersion;

public:

	bool eventEnabled;
	bool play;

	double currentTime;
	double maximumTime;

	// params
	FilmPlaybackLoopMode playbackLoopMode;
	bool customFps;
	double fps;

	// build
	double _fpsFactor;
	
private:

	QString GetValidName(const QString &name);

public:
	Film(void);
	~Film(void);
    
    //returns color for creating layer
    QColor GetNextNewLayerColor();

	void OnLoad(bool onlyAnim=false);

	// controls

	void Play();
	void Stop();
	void Rewind();
	void Forward();
	void Start();
	void End();
    void PrevFrame();
    void NextFrame();

	// params

	bool GetCustomFPS(){return customFps;}
	double GetFPS(){return fps;}
	void SetFPS(bool custom,double fps);

	FilmPlaybackLoopMode GetPlaybackLoopMode(){return playbackLoopMode;}
	void SetPlaybackLoopMode(FilmPlaybackLoopMode playbackLoopMode);
    void SetPlaybackBounds(bool usePlaybackBounds, int startPlaybackFrame, int endPlaybackFrame);
	int GetStartPlaybackFrame();
	int GetEndPlaybackFrame();
	bool GetUsePlaybackBounds(){return usePlaybackBounds;}
	// frames
	void MoveStartFrameBound(int delta);

	double GetStartTimeBound();
	double GetEndTimeBound();
	void SetEndTimeBound(double maximumTime);

	double GetStartFrameBound();
	double GetEndFrameBound();

	double GetStartActiveFrame();
	double GetEndActiveFrame();

	double GetStartActiveTime();
	double GetEndActiveTime();

	double GetPlayTime(){return currentTime ;}
	void SetPlayTime(double time);

	double GetBuildTime();
	int GetPlayFrame(){return (int)(currentTime * fps);}
	void SetPlayFrame(int currentFrame);
	
	bool IsCurrentFrameVisible();
	void Clear();

	// info

	QString GetCurrentAnimFileVersion(); 
	QString GetAnimFileVersion() {return animFileVersion;}
	bool IsOldAnimFileVersion();

	AnimaFileInfo *GetSceneInfo();

	// film layers
	int FilmLayerCount(){return filmLayers.count();}
	QList<FilmLayer *> &GetFilmLayers(){return filmLayers;}
	void GetAnimationList(QList<Animation *> &);
	FilmLayer *AddFilmLayer(const QString &name);
	FilmLayer *AddFilmLayer();
	FilmLayer *AddFilmLayer(FilmLayer *layer);
	void InsertFilmLayer(int idx,FilmLayer *filmLayer);
	void DeleteFilmLayer(FilmLayer *filmLayer);
	int IndexOf(FilmLayer *filmLayer);
	FilmLayer *Find(const QString &name);
	bool RenameFilmLayer(FilmLayer *filmLayer,const QString &newName);
	ModelInstance *FindModel(int id);

	// 
	FilmDrawMode GetFilmDrawMode(){return drawMode;}
	void SetFilmDrawMode(FilmDrawMode drawMode){this->drawMode=drawMode;}
	void OnFPSTick(int timeDeltaMills);
	void PreRenderTargetUpdate();
	void PostRenderTargetUpdate();

	void Register_FPSTickHandler(FPSTickHandler fpsTickHandler);

	// API interna
	void FireFilmLayer(FilmLayer *layer,char type);
	void FireAnimationEvent(Animation *anim,char type);
	void FireAnimationPartEvent(AnimationPart *animPart,char type);
	void FireAnimationModelChanged(ModelInstance *modelInstance);
	void FireAnimationPartChanged(AnimationPart *part);
	void FireEnabledChanged(SceneObject *so);
	void FireBoundFramesChanged();
	void FirePropertyChangedEvent(SceneObject *so);
	void FireMovableChangedEvent(SceneObjectMovable *obj);
	void FireRenderDone();
	void FireBuilderDone();

	void FlushEvents();

	void _FireFilmLayer(FilmLayer *layer,char type);
	void _FireAnimationEvent(Animation *anim,char type);
	void _FireAnimationPartEvent(AnimationPart *animPart,char type);
	void _FireAnimationModelChanged(ModelInstance *modelInstance);
	void _FireEnabledChanged(SceneObject *so);
	void _FireBoundFramesChanged();
	void _FireMovableChangedEvent(SceneObjectMovable *obj);

	void NotifyDelete(SceneObject *obj);

	void SetFireModelDrawEventHadler(void (* fireModelDrawEventHandler)(ModelInstance *));
	void FireModelDrawEvent(ModelInstance *model);

	void InvalidateGfx();
	void InvalidateGfxFromIK(BaseModel *model);

private:

	void CheckFrameInterval();
	void AddEvent(const FilmEvent &evt);

signals:

	void OnFilmChanged();
	void OnAddFilmLayer(FilmLayer *filmLayer);
	void OnRemoveFilmLayer(FilmLayer *filmLayer);
	void OnChangedFilmLayer(FilmLayer *filmLayer);
	void OnChangedUI(FilmLayer *filmLayer);
	void OnPropertyChanged(SceneObject *obj);
	void OnMovableChangedEvent(SceneObjectMovable *obj);
	void CurrentFrameChanged();
	void BoundFramesChanged();
	void RenderDone();
	void OnBuilderDone();

	void SceneLoaded();

	void SceneObjectDeleting(SceneObject *obj);
};
#include "Anima_nsend.h"

