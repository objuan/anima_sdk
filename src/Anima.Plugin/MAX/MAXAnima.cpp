#include "stdafx.h"

#include "MAXAnima.h"
#include <IPathConfigMgr.h> 
#include "notify.h"
#include <stdio.h>

#ifdef MAX2009_SDK
#include "ISceneEventManager.h"
#endif

#include "Core/Context.h"
#include "Core/Film.h"
#include "Core/FilmLayer.h"
#include "Core/Animation.h"
#include "Core/ModelInstance.h"
#include "Core/Model.h"
#include "Core/ModelLibrary.h"
#include "Core/Config.h"
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxBaseModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/MeshExt.h"
//#include "ui_ProgressControl.h"

//#include "UI/MainWindow.h"

#include "MeshObj.h"

//#include "./GLView.h"

HINSTANCE hInstance;
HINSTANCE plugin_hInstance;

//extern MeshObjectClassDesc *meshObjectClassDesc;
//BlobMeshClassDesc *blobObjectClassDesc = NULL;
//HINSTANCE hInstance;
static MAXAnima maxAnima;

MAXAnima *MAXAnima::Singleton()
{
	return &maxAnima;
}

class BulkGUIManager : public Anima::GUIManager
{
private:

public:

	BulkGUIManager(){}
	~BulkGUIManager(void){}

	virtual bool IsPlugin(){return true;}

	virtual QString BrowseLoadAnimaFile(const QString &dialogName){return "";}
	virtual QString BrowseSaveAnimaFile(const QString &dialogName){return "";}
	virtual QString BrowseLoadModelFile(const QString &dialogName){return "";}

	virtual Anima::ProgressBar *OpenProgressBar(const QString &title,int phaseCount,bool openwin);
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

	virtual void CloseWidgetByID(long id){}

	virtual void OnSceneLoad(const QString &path){}
	virtual void OnSceneImported(const QString &path){}
	virtual void OnSceneSaved(const QString &path){}

};

DWORD WINAPI OpenProgressBar_FN(LPVOID arg)
{
	return 0;
}

Anima::ProgressBar *BulkGUIManager::OpenProgressBar(const QString &title,int phaseCount,bool openwin)
{
	Anima::ProgressBar *bar = new Anima::ProgressBar(this,title,phaseCount,openwin);

	if (openwin)
		GetCOREInterface()->ProgressStart (QString("Anima: %1").arg(title).toAscii().data(),true,&OpenProgressBar_FN,bar);
	return bar;
}

void BulkGUIManager::UpdateProgressBar(Anima::ProgressBar *bar)
{
	if (bar->openwin)
	{
		QString desc = QString("%1   %2").arg(bar->phaseDesc).arg(bar->stepDesc);
		GetCOREInterface()->ProgressUpdate(bar->PercentPhase(),false,desc.toAscii().data());
	}
}

void BulkGUIManager::CloseProgressBar(Anima::ProgressBar *bar)
{
	if (bar->openwin)
		GetCOREInterface()->ProgressEnd();
}

class TestViewWindow : public ViewWindow {
public:
	MCHAR *GetName() { return "Anima"; }
	HWND hwnd;
	//int EnsureInit(HWND win );

	HWND CreateViewWindow(HWND hParent, int x, int y, int w, int h) 
	{
		//return CreateWindow("button", "Test Button", WS_VISIBLE | WS_CHILD, x, y, w, h, hParent, NULL, (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE), NULL);
		//MainWindow *mainWin = new MainWindow();
		//mainWin->show();
		//HWND win = (HWND)mainWin->winId();

		////HWND win =  CreateWindow(QStringToTCHAR(QString("button")), QStringToTCHAR(QString("finestra di test")), WS_VISIBLE | WS_CHILD, x, y, w, h, hParent, NULL, (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE), NULL);

		////EnsureInit(win);

		//return win;
		return hwnd;
	}

	void DestroyViewWindow(HWND hWnd) {

		DestroyWindow(hWnd);
	}
};

static TestViewWindow testwin;

// ============================================

BulkGUIManager *designerGUIManager = NULL;
Ogre::RenderWindow *renderWindow = NULL;

MAXAnima::MAXAnima()
{
	initialized=false;
}

MAXAnima::~MAXAnima()
{
}


void CheckAnimaIntegrity()
{
	if (MeshObject::meshObject!=NULL)
	{
		INode* node = FindNodeRef(MeshObject::meshObject);
		if (node == NULL)
		{
			//MeshObject::meshObject->Clear();
			MeshObject::meshObject = NULL;
		}
	}
}

void NOTIFYPROC_POST_RENDERER_CHANGE(void *param, NotifyInfo *info)  
{
	MAXAnima *max = ((MAXAnima *)param);
	CheckAnimaIntegrity();
	if (MeshObject::meshObject != NULL)
	{
		MeshObject::meshObject->CheckMaterial();
	}
}

void NOTIFYPROC_PRE_RENDER(void *param, NotifyInfo *info)  
{
	// OKKIO, distinguere render finestra da quello dei materiali
	MAXAnima *max = ((MAXAnima *)param);
	CheckAnimaIntegrity();
	if (MeshObject::meshObject != NULL)
	{
		MeshObject::meshObject->OnPreRender();
	}
}
void NOTIFYPROC_POST_RENDER(void *param, NotifyInfo *info)  
{
	// OKKIO, distinguere render finestra da quello dei materiali
	MAXAnima *max = ((MAXAnima *)param);
	CheckAnimaIntegrity();
	if (MeshObject::meshObject != NULL)
	{
		MeshObject::meshObject->OnPostRender();
	}
}
void NOTIFYPROC_SELECTIONSET_CHANGED(void *param, NotifyInfo *info)  
{
	CheckAnimaIntegrity();
	MAXAnima *max = ((MAXAnima *)param);
	if (MeshObject::meshObject != NULL)
	{
		MeshObject::meshObject->OnSelectChanged();
	}
}

class MyRedrawViewsCallback : public RedrawViewsCallback
{
	virtual void  proc (Interface *ip)
	{
	//	Anima::_LogDebug(QString("MyRedrawViewsCallback"),__FUNCTION__,__LINE__);

		CheckAnimaIntegrity();

		//	Anima::_LogDebug(QString("MyRedrawViewsCallback 1"),__FUNCTION__,__LINE__);

		if (MeshObject::meshObject != NULL)
		{
			MeshObject::meshObject->OnRefresh();
		}
	}
};

MyRedrawViewsCallback myRedrawViewsCallback;

int MAXAnima::EnsureInit(const char *_programPath)
{
	if (!initialized)
	{
	//	int argc=0;
		//char **argv=NULL;
//		QApplication app(argc, argv);

		bool done=false;
  		try
		{
			bool userNullRender=true;
			try
			{
				char line[1000];
				// provo con il file di config
				IPathConfigMgr *mgr =  IPathConfigMgr::GetPathConfigMgr();
				for(int i=0;i<mgr->GetPlugInEntryCount();i++)
				{
					QString path;
					path = QString("%1/Anima.xml").arg(mgr->GetPlugInDir(i));

					if (QFile(path).exists())
					{
						FILE *configFp = fopen(path.toAscii().data(), "r" );
						if (configFp!=NULL)
						{
							path="";
							char *l=NULL;
							do
							{
								l = fgets(line,1000,configFp);
								if (strncmp(line,"PATH",4) == 0)
								{
									path = QString("%1").arg(line);
									path=path.mid(5);
									break;
								}
							}while(l!=NULL);
							fclose(configFp);
						}
						Anima::Config::Init(userNullRender,path);
						done=true;
					}
				}
			}
			catch(Anima::CoreException )
			{
				
			}

			if (!done)
			{
				QString programPath;
				programPath =  QString::fromLocal8Bit(_programPath);
				QString prgPath = QFileInfo(programPath).dir().path();
				Anima::Config::Init(userNullRender,prgPath);
			}

			if (!userNullRender)
			{
				Anima::GfxContext::GfxContext_New(NULL);
				Anima::Context::Singleton.Init(true);

				designerGUIManager = new BulkGUIManager();

				////// window

				HWND hParent = GetCOREInterface()->GetMAXHWnd ();
			//WS_VISIBLE | WS_CHILD
			//	HWND w = CreateWindow("button", "Test Button",WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP ,0,0,100,100, hParent, NULL, (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE), NULL);

			//	Ogre::NameValuePairList params;
			//	Ogre::String externalWindowHandleParams;
			/*	externalWindowHandleParams = Ogre::StringConverter::toString((unsigned int)(hParent));
				params["externalWindowHandle"] = externalWindowHandleParams;*/
				renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, NULL);
				//////renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("View",100,100, false, NULL);//&params);
			
			//	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
					renderWindow->getCustomAttribute("WINDOW", (void*)&testwin.hwnd);

					ShowWindow(testwin.hwnd,false);
					SetParent(testwin.hwnd,hParent);
					ShowWindow(testwin.hwnd,false);

					 // LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
					 // SetClassLong( hwnd, GCL_HICON, iconID );
			//	#endif
				Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);
			}
			else
			{

				Anima::GfxContext::GfxContext_New(NULL);
				Anima::Context::Singleton.Init(true);

				designerGUIManager = new BulkGUIManager();

				renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, NULL);
			//	renderWindow=NULL;

				Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);
			}
		
		//	GetCOREInterface()->RegisterViewWindow(&testwin);

			GetCOREInterface()->RegisterRedrawViewsCallback  (  &myRedrawViewsCallback   )  ;
			RegisterNotification (&NOTIFYPROC_POST_RENDERER_CHANGE,this,NOTIFY_POST_RENDERER_CHANGE);
			RegisterNotification (&NOTIFYPROC_PRE_RENDER   ,this,NOTIFY_PRE_RENDER   );
			RegisterNotification (&NOTIFYPROC_POST_RENDER   ,this,NOTIFY_POST_RENDER   );

			// SOTTO e' per i materiali !!!!!!!!!!
			//RegisterNotification (&NOTIFYPROC_PRE_RENDER     ,this,NOTIFY_PRE_RENDERFRAME     );
			RegisterNotification (&NOTIFYPROC_SELECTIONSET_CHANGED     ,this,NOTIFY_SELECTIONSET_CHANGED     );
			initialized=true;

		}
		catch(Anima::CoreException &e)
		{
			QMessageBox::warning(NULL, "Anima startup error",e.Message());
			return -1;
		}
	}
	return 0;
}

//int MAXAnima::Load1(QString &name,ImpInterface *importInt,Interface *globalInt, bool suppressPrompts)
//{
//}
//

INode *FindAnimaNode(INode *node)
{
	Object *obj = node->GetObjOrWSMRef  () ;
	if (obj != NULL)
	{
		if (obj->ClassID()==MeshObject_CLASS_ID  )
		{
			return node;
		}
	}
	for(int i=0;i<node->NumberOfChildren ();i++)
	{
		INode *n = FindAnimaNode(node->GetChildNode (i));
		if (n != NULL)
			return n;
	}
	return NULL;
}

INode *CreateRootNode(ImpInterface *importInt,Interface *globalInt)
{
	if (MeshObject::meshObject==NULL) 
		return NULL;

	// cerco
	INode *n = FindAnimaNode(globalInt->GetRootNode());
	if (n!=NULL) 
	{
		// cancello
		/*theHold.Begin();
		int del = globalInt->DeleteNode(n,FALSE);
		theHold.Accept(_M("Delete"));*/
		return n;
	}


	ImpNode *node = importInt->CreateNode();
	if(!node) return NULL;
	node->Reference(MeshObject::meshObject);
	Matrix3 tm(1);

	importInt->AddNodeToScene(node);
	TSTR gName("AXYZ Anima");
	globalInt->MakeNameUnique(gName);
	node->SetName(gName);

	return node->GetINode();
}

int MAXAnima::Load(const char *programPath,QString &name,ImpInterface *importInt,Interface *globalInt, bool suppressPrompts)
{
	this->importInt=importInt;
	this->globalInt=globalInt;

	int ret = EnsureInit(programPath);

	// c'e' già il nodo anima ?
	Anima::_LogDebug(QString("MAX Load   '%1'").arg(programPath),__FUNCTION__,__LINE__);

	if (MeshObject::meshObject == NULL)
	{
		MeshObject *obj  = (MeshObject*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObject_CLASS_ID);
		//BlobMesh *obj  = (BlobMesh*)CreateInstance(GEOMOBJECT_CLASS_ID, BLOBMESH_CLASS_ID);
		
		assert(obj);

		INode *node = CreateRootNode(importInt,globalInt);
		if (!node) return -1;
		// create shape object
		//ImpNode *node = importInt->CreateNode();
		//if(!node) return -1;
		//node->Reference(obj);
		//Matrix3 tm(1);
		////Box3 bbox = shape->GetBoundingBox();
		////Point3 center = Point3(bbox.Center().x, bbox.Center().y, 0.0f);
		////tm.SetTrans(center);	
		////node->SetPivot(-center);	
		////node->SetTransform(0,tm);
		//importInt->AddNodeToScene(node);

		//TSTR gName("AXYZ Anima");
		//globalInt->MakeNameUnique(gName);
		//node->SetName(gName);

		//obj->LoadFromGUI(name,node);
		obj->Load(name,node,true,true);

		//node->GetINode()->SetMtl(obj->GetMaterial());

		// intervallo
		/*TimeDisp disp = GetTimeDisplayMode();
		int tickPerFrame = GetTicksPerFrame();
		int frameRate = GetFrameRate();*/

		/*Anima::Film &film = Anima::Context::Singleton.GetFilm();
		double startTime = film.GetStartTimeBound();
		double endTime = film.GetEndTimeBound();
		TimeValue s(SecToTicks (startTime));
		TimeValue e(SecToTicks (endTime));
		importInt->SetAnimRange(Interval(s,e));*/

		//obj->CheckSceneNode();
		//node->SetMtl(obj->GetMaterial());
	}
	else
	{
	/*	INode *inode = node1->GetINode();
		Mesh *mesh = &(((TriObject *)inode->GetObjectRef())->GetMesh());*/

		//MeshObject::meshObject->GetNode();

		if (MeshObject::meshObject != NULL)
		{
			//MSTR s;
			//RefTargetHandle t = MeshObject::meshObject->GetReference(0);
			//t->GetClassName(s);

			//INode *node =(INode *)t;

		/*	RefList &list = MeshObject::meshObject->GetRefList();
			RefListItem *t = list.FirstItem();
			while(t != NULL)
			{		
				MSTR s;
				t->GetClassName(s);
				t = list.next;
			}*/

			//ImpNode *node = importInt->CreateNode();
			//if(!node) return -1;
			//node->Reference(MeshObject::meshObject);
			//Matrix3 tm(1);
			////Box3 bbox = shape->GetBoundingBox();
			////Point3 center = Point3(bbox.Center().x, bbox.Center().y, 0.0f);
			////tm.SetTrans(center);	
			////node->SetPivot(-center);	
			////node->SetTransform(0,tm);
			//importInt->AddNodeToScene(node);
			//QString n = QString("Anima");

			// controllo prima se e' stato cancellato 
			//MeshObject::meshObject->Restart();
			if (MeshObject::meshObject != NULL)
			{
				MeshObject::meshObject->Clear();

				// se non ce' INode lo devo cancellare
				//INode* node = FindNodeRef(MeshObject::meshObject);
				//if (node == NULL)
				//	MeshObject::meshObject->animaNode = NULL;//DeleteThis();
	
				//MeshObject::meshObject=NULL;	
			}

			if (MeshObject::meshObject->animaNode == NULL)
			{
				// TODO BETTER
				//MeshObject *obj  = (MeshObject*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObject_CLASS_ID);
				//assert(obj);
				//// create shape object
			
				//INode* animaNode = FindAnimaNode();
				//if (animaNode == NULL)
				{
					INode *node = CreateRootNode(importInt,globalInt);
					if (!node) return -1;

					MeshObject::meshObject->animaNode = node;
				}
		
				//node->GetINode()->SetMtl(obj->GetMaterial());
			}
	
			//MeshObject::meshObject->LoadFromGUI(name,FindNodeRef(MeshObject::meshObject));
			MeshObject::meshObject->Load(name,FindNodeRef(MeshObject::meshObject),true,true);


			// intervallo
			/*TimeDisp disp = GetTimeDisplayMode();
			int tickPerFrame = GetTicksPerFrame();
			int frameRate = GetFrameRate();*/

		/*	Anima::Film &film = Anima::Context::Singleton.GetFilm();
			double startTime = film.GetStartActiveTime();
			double endTime = film.GetEndActiveTime();
			TimeValue s(SecToTicks (startTime));
			TimeValue e(SecToTicks (endTime));
		
			Interval currentInt =  importInt->GetAnimRange();
			
			s = MIN( s, currentInt.Start () );
			e = MAX( e, currentInt.End () );

			importInt->SetAnimRange(Interval(s,e));*/

			//node->GetINode()->SetMtl(MeshObject::meshObject->GetMaterial());
			//node->SetMtl(MeshObject::meshObject->GetMaterial());
		}
	}

	return IMPEXP_SUCCESS;
}

SimpleObject *MAXAnima::CreateObject(const char*programPath,MeshObjectClassDesc *classDesc,bool loading)
{
	int ret = EnsureInit(programPath);
	if (MeshObject::meshObject == NULL)
	{
		MeshObject *obj = new MeshObject(classDesc,loading);
		return obj;
	}
	else
		return MeshObject::meshObject;
}

/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      hInstance = hinstDLL;
      DisableThreadLibraryCalls(hinstDLL);
   }
   return(TRUE);
}

HINSTANCE MeshObjectClassDesc::HInstance()
{
	return hInstance; 
}

HINSTANCE MeshObjectSubMeshClassDesc::HInstance()
{
	return hInstance; 
}
