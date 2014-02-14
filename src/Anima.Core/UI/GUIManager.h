#pragma once
#include "Anima_nsbegin.h"
class AnimationPartStairs;
class SceneObject;
class ZoomManager;
class GUIManager;

class ANIMA_CORE_API ProgressBar: public BObject
{
private:
	GUIManager *guiManager;
public:
	QString title;
	
	int phaseCount;
	QString phaseDesc;
	int actualPhase;

	int stepCount;
	QString stepDesc;
	int actualStep;
	bool openwin;
	bool valid;

public:
	ProgressBar(GUIManager *guiManager,const QString &title,int phaseCount,bool openwin);
	~ProgressBar();

	void PushPhase(const QString &phaseDesc,int step);
	void PopPhase();
	void PushStep(const QString &desc); 
	void PopStep();

	int PercentPhase();
	int PercentStep();
};

class ANIMA_CORE_API GUIManager : public QObject, public BObject
{
	Q_OBJECT

public:

	enum OpenMode
	{
		GUINormal = 0,
		GUIDialog,
		GUIDialogNoClose,
	};

private:

	ZoomManager *zoomManager;
	//UnitMeter unit;

	bool gridSnapEnabled;
	bool angleCheckEnabled;
	bool tooltipEnabled;

public:

	GUIManager();
	~GUIManager(void);

	ZoomManager &GetZoomManager(){return *zoomManager;}

	/*UnitMeter GetUnit(){return unit;}
	void SetUnit(UnitMeter unit);*/

	virtual bool IsPlugin()=0;

	bool GetGridSnapEnabled(){return gridSnapEnabled;}
	bool GetAngleCheckEnabled(){return angleCheckEnabled;}
	bool GetTooltipEnabled(){return tooltipEnabled;}
	void SetGridSnap(bool enabled){gridSnapEnabled=enabled;}
	void SetAngleCheck(bool enabled){angleCheckEnabled=enabled;}
	void SetTooltip(bool enabled){tooltipEnabled=enabled;}

	virtual QString BrowseLoadAnimaFile(const QString &start_path)=0;
	virtual QString BrowseSaveAnimaFile(const QString &start_path)=0;
	virtual QString BrowseLoadModelFile(const QString &start_path)=0;

	virtual ProgressBar *OpenProgressBar(const QString &title,int phaseCount,bool openWindow)=0;
	virtual void UpdateProgressBar(ProgressBar *)=0;
	virtual void CloseProgressBar(ProgressBar *)=0;

	virtual void OpenWarningMessage(const QString &msg)=0;
	virtual bool OpenMessageYesNo(const QString &title,const QString &msg)=0;
	virtual bool OpenSelectImportFile(QString *path,UnitMeter *unit,Ogre::Vector3 *rot)=0;
	virtual int OpenSelectMultiContextMenu(const QPoint &pos)=0;
	virtual int OpenContextMenu(const QPoint &pos,SceneObject *sceneObject)=0;

	virtual int EditProperty(SceneObject *sceneObject,OpenMode mode)=0;
	//virtual int Edit_ModelProperty(ModelInstance *modelInstance,OpenMode mode)=0;
	virtual void NotifyPropertyWidgetClosed(int id,bool ok,SceneObject *obj);
	virtual void CloseWidgetByID(long id)=0;

	// eventi

	virtual void OnSceneLoad(const QString &path)=0;
	virtual void OnSceneImported(const QString &path)=0;
	virtual void OnSceneSaved(const QString &path)=0;

signals:
	 void OnPropertyWidgetClosed(int id,bool ok,SceneObject *obj);
};
#include "Anima_nsend.h"
