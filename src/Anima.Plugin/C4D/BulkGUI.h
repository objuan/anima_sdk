#pragma once
#include "AnimaIncludes.h"
#include "Utils.h"


class C4DProgress : public Anima::ProgressBar
{

public:
	C4DProgress(Anima::GUIManager *guiManager,const QString &title,int phaseCount,bool openwin) : Anima::ProgressBar(guiManager, title, phaseCount, openwin)
	{
		StatusSetText(Utils::toC4DString(title));
	}

};
class BulkGUIManager : public Anima::GUIManager
{
private:
	Anima::ProgressBar* m_statusbar;
public:

	BulkGUIManager()
	{
	}
	~BulkGUIManager(void)
	{
	}

	virtual bool IsPlugin(){return true;}

	virtual Anima::ProgressBar *OpenProgressBar(const QString &title,int phaseCount,bool openwin) { return new C4DProgress(&Anima::Context::Singleton.GetGuiManager(), title, 0, FALSE); }
	virtual void UpdateProgressBar(Anima::ProgressBar *);
	virtual void CloseProgressBar(Anima::ProgressBar *);

	virtual void OpenWarningMessage(const QString &msg){}
	virtual bool OpenMessageYesNo(const QString &title,const QString &msg){return true;}
	virtual bool OpenSelectImportFile(QString *path,Anima::UnitMeter *unit,Ogre::Vector3 *rot){return 0;}
	virtual int OpenSelectMultiContextMenu(const QPoint &pos){return 0;}
	virtual int OpenContextMenu(const QPoint &pos,Anima::SceneObject *sceneObject){return 0;}
	virtual int EditProperty(Anima::SceneObject *sceneObject,OpenMode mode){return 0;}
	//virtual int OpenSelectMultiContextMenu(const QPoint &pos){return 0;}
	//virtual int OpenContextMenu(const QPoint &pos,Anima::SceneObject *sceneObject){return 0;}
	//virtual int EditProperty(Anima::SceneObject *sceneObject,OpenMode mode){return 0;}
	
	virtual void NotifyPropertyWidgetClosed(int id,bool ok,Anima::SceneObject *obj){}

	virtual QString BrowseLoadAnimaFile(const QString &dialogName){return "";}
	virtual QString BrowseSaveAnimaFile(const QString &dialogName){return "";}
	virtual QString BrowseLoadModelFile(const QString &dialogName){return "";}

	virtual void CloseWidgetByID(long id){}

	virtual void OnSceneLoad(const QString &path){}
	virtual void OnSceneImported(const QString &path){}
	virtual void OnSceneSaved(const QString &path){}

};
