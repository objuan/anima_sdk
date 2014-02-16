#include "stdafx.h"

#include "LWApi.h"
#include "LWAnima.h"

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
#include "Gfx/GfxModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/MeshExt.h"

#include "./GLView.h"
//#include "Import/ImportLWO.h"


int LoadSurf( const char *filename,int clipIdx,QString &txtMap,unsigned char *buf,int fileSize)
{
	FILE *fp;
	char buf_aus[1000];
	int c,i;
	int vmapMarker,blockMarker,postVmapMarker,imagMarker;

	fp = fopen(filename, "rb" );
	if ( !fp ) return -1;

	c = fread(buf,fileSize,1,fp);

	fclose( fp );

	// letto, calcolo i marker

	for(i=0;i<fileSize-4;i++)
	{
		if (buf[i] == 'V' && buf[i+1] == 'M' && buf[i+2] == 'A' && buf[i+3] == 'P')
			vmapMarker=i;
		else if (buf[i] == 'B' && buf[i+1] == 'L' && buf[i+2] == 'O' && buf[i+3] == 'K')
			blockMarker=i;
		else if (buf[i] == 'I' && buf[i+1] == 'M' && buf[i+2] == 'A' && buf[i+3] == 'G')
			imagMarker=i;
	}

	// imag

	buf[imagMarker+7] = (unsigned char)clipIdx;

	// cambio VMAP
	
	int txtMapCount = txtMap.count();
	int oldSize = buf[vmapMarker+5];
	postVmapMarker = vmapMarker +6+oldSize; 

	memcpy(buf_aus,buf + postVmapMarker,fileSize - postVmapMarker);

	buf[vmapMarker+5] = (unsigned char)txtMapCount+1;
	if (buf[vmapMarker+5] & 1) buf[vmapMarker+5]++;

	for(int i=0;i<txtMapCount;i++)
		buf[vmapMarker+6+i] = txtMap[i].toAscii();
	buf[vmapMarker+6+txtMapCount] = '\0';

	memcpy(buf + vmapMarker+6+buf[vmapMarker+5],buf_aus,fileSize - postVmapMarker);

	// blocksize

	short oldBlockSize = (buf[blockMarker+4] << 8 )+ buf[blockMarker+5];
	short newBlockSize = oldBlockSize  + (buf[vmapMarker+5] - oldSize);
	if (newBlockSize &1) newBlockSize++;

	buf[blockMarker+4] = newBlockSize >> 8 & 0x00FF;
	buf[blockMarker+5] = newBlockSize  & 0x00FF;

	return fileSize + (newBlockSize - oldBlockSize);
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

	//if (openwin)
	//	GetCOREInterface()->ProgressStart (QString("Anima: %1").arg(title).toAscii().data(),true,&OpenProgressBar_FN,bar);
	return bar;
}

void BulkGUIManager::UpdateProgressBar(Anima::ProgressBar *bar)
{
	if (bar->openwin)
	{
	//	QString desc = QString("%1   %2").arg(bar->phaseDesc).arg(bar->stepDesc);
	//	GetCOREInterface()->ProgressUpdate(bar->PercentPhase(),false,desc.toAscii().data());
	}
}

void BulkGUIManager::CloseProgressBar(Anima::ProgressBar *bar)
{
	//if (bar->openwin)
	//	GetCOREInterface()->ProgressEnd();
}

//class TestViewWindow : public ViewWindow {
//public:
//	MCHAR *GetName() { return "Anima"; }
//	HWND hwnd;
//	//int EnsureInit(HWND win );
//
//	HWND CreateViewWindow(HWND hParent, int x, int y, int w, int h) 
//	{
//		return hwnd;
//	}
//
//	void DestroyViewWindow(HWND hWnd) {
//
//		DestroyWindow(hWnd);
//	}
//};
//
//static TestViewWindow testwin;

AnimaStore::AnimaStore()
{
	pointMap = new QHash<LWPntID,int>();
	initialized=false;
	//TotalVertexArray=NULL;
	meshExporter=NULL;
}

AnimaStore::~AnimaStore()
{
	delete pointMap;
	/*if (TotalVertexArray!=NULL) delete[] TotalVertexArray;
	TotalVertexArray=NULL;*/
	if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;
}

//
//void _SetFireModelDrawEventHadler(Anima::GfxModel *model)
//{
//	//animaStore->OnDrawModel(model);
//}

BulkGUIManager *designerGUIManager = NULL;
Ogre::RenderWindow *renderWindow = NULL;

int AnimaStore::Load(GlobalFunc *global, LWObjectImport *local,void *serverData,char *_programPath)
{
	int argc=0;
	char **argv=NULL;

	QString fileName(local->filename);
	if (!fileName.endsWith(".ani")) return 1;
		
	if (!initialized)
	{
		QApplication app(argc, argv);

		bool done=false;
  		try
		{
			bool userNullRender=true;

			try
			{
				char line[1000];
				
				QString path = "";
				path = QString("\\..\\Plugins\\Anima.xml");

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

				HWND hParent = 0;// GetCOREInterface()->GetMAXHWnd ();
				renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, NULL);
				//////renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("View",100,100, false, NULL);//&params);
			
			////	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			//		renderWindow->getCustomAttribute("WINDOW", (void*)&testwin.hwnd);

			//		ShowWindow(testwin.hwnd,false);
			//		SetParent(testwin.hwnd,hParent);
			//		ShowWindow(testwin.hwnd,false);

			//		 // LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
			//		 // SetClassLong( hwnd, GCL_HICON, iconID );
			////	#endif
				Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);
			}
			else
			{

				Anima::GfxContext::GfxContext_New(NULL);
				Anima::Context::Singleton.Init(true);

				designerGUIManager = new BulkGUIManager();

				renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, NULL);
		
				Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);
			}

			initialized=true;

		/*
  		try
		{
			Anima::Config::Init();
			Anima::GfxContext::Singleton.Init();
			Anima::Context::Singleton.Init();

			// bitmab non utilizzate
			//ResourceManager::Singleton.Init();

			designerGUIManager = new BulkGUIManager();
			//Ogre::NameValuePairList params;


		////	GLView *mainWin = new GLView();
		////	mainWin->show();
		////	HWND win = (HWND)mainWin->winId();

		////	Ogre::NameValuePairList params;
		////	Ogre::String s ="16";
		////	params["colourDepth"] = s;

		/////	Ogre::String s1 = Ogre::StringConverter::toString((size_t)win);
		////	params["externalWindowHandle"] = s1;

		////	renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("View", mainWin->width(), mainWin->height(), false, &params);

			// TODO TEST
			HWND win;
			renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, NULL);
			renderWindow->getCustomAttribute("WINDOW", (void*)&win);
			ShowWindow(win,false);
			//SetParent(testwin.hwnd,hParent);
			//ShowWindow(testwin.hwnd,false);

			Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);

			initialized=true;
			*/
		}
		catch(Anima::CoreException &e)
		{
			QMessageBox::warning(NULL, "Anima startup error",e.Message());
			return -1;
		}
	}

	if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;

	// load

	Anima::Context::Singleton.Clear();
	
	try
	{
		Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("loading '%1'..").arg(QFileInfo(fileName).fileName()),4,true);

		Anima::ContextIO contextIO;
		contextIO.Load(fileName,false,progressBar); // carico nell'ambiente

		delete progressBar;
	}
	catch(Anima::CoreException &e)
	{
		return -1;
	}

	// update

	//renderWindow->update();
	Anima::GfxContext::Singleton->UpdateAndWaitTask();

	// TODO scan all and IK wait

	// --------------------

	char tag[ 40 ];
	unsigned char data[50000 ];
	char name[ 40 ];
	const char *layerName;
	int size;

	LWItemInfo *iteminfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	LWImageList *imglist = (LWImageList *)global( LWIMAGELIST_GLOBAL, GFUSE_TRANSIENT );
	LWTextureFuncs *txfunc= (LWTextureFuncs *)global( LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT );
	LWSurfaceFuncs *surff = (LWSurfaceFuncs *)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );

	// levo quello vecchio, se c'e'

	// TODO
	//LWItemID id = iteminfo->first( LWI_OBJECT, NULL );
 //   while ( id ) 
	//{
	//	QString s(iteminfo->name( id ));
	//	if (s.endsWith(":EMotion"))
	//	{
	//		// levo l'oggetto vecchio
	//		//SelectItem id
	//		// ClearSelected

	//		return -1;
	//	}
 //       id = iteminfo->next( id );
 //  }

	// ---------

	LWFVector pos = { 0.0f };
	local->layer( local->data, 1, "anima" );
	local->pivot( local->data, pos );

	Anima::MeshExporter _meshExporter;
	_meshExporter.BuildCurrentTime();

	// materials && textures

	Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	for(int i=0;i<meshList.ModelCount();i++)
	{
		Anima::MeshExtModel *meshExt = (Anima::MeshExtModel *)meshList.GetModel(i);
		Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

		int txtCount = mesh->getNumSubMeshes();
		for(int t=0;t<txtCount;t++)
		{
			Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(subMesh->getMaterialName());
			Ogre::Technique *tec = mat->getTechnique(0); // il primo
			Ogre::Pass *pass = tec->getPass(0);
			int txtSubNum = pass->getNumTextureUnitStates();
			Ogre::TextureUnitState * txt = pass->getTextureUnitState(0);
			Ogre::String txtName = txt->getFrameTextureName(0);

			QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
			QString modelDir = Anima::Context::Singleton.GetModelLibrary().GetModelDir(meshExt->GetModel()->GetModel());
			QString imagePath = modelDir+"/"+txtName.c_str();
			QString mapPath = QString("%1_Txt_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
			QString defPath = Anima::Config::DataRootPath()+"/plugin/lw/default.srf";

			LWImageID  imageID =  imglist->load(imagePath.toAscii().constData());

			int fsize = QFile(defPath).size();
			size = LoadSurf(defPath.toAscii().constData(),1,mapPath, data,fsize );

			local->surface( local->data, name.toAscii().constData(), NULL, size, data );

			LWSurfaceID surfID = surff->first();
			do
			{
				if (surfID != NULL)
				{
					if (strcmp(surff->name(surfID),name.toAscii().constData()) == 0)
					{
						LWTextureID txt = surff->getTex(surfID,SURF_COLR);
						LWTLayerID txtLayer = txfunc->firstLayer(txt);
						int result = txfunc->setParam( txtLayer, TXTAG_IMAGE, &imageID );
					}
					surfID = surff->next(surfID);
				}
			}
			while(surfID!=NULL);
		}
	}

	// LOAD

	totalVertexCount=0;

	pointMap->clear();
//	if (TotalVertexArray!=NULL) delete[] TotalVertexArray;

	for(int i=0;i<meshList.InstanceCount();i++)
	{
		Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
		AddModel(local,meshExt);
	}

	// ini 
	Anima::GfxContext::Singleton->UpdateAndWaitTask();

	meshExporter = new Anima::MeshExporter();
	meshExporter->BuildCurrentTime();

	return 0;
}

void AnimaStore::Clear()
{
	pointMap->clear();

	/*if (TotalVertexArray!=NULL) delete[] TotalVertexArray;
	TotalVertexArray=NULL;*/
	if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;

	Anima::Context::Singleton.Clear();
}

int lw_time=0;
//aggiunge 
void AnimaStore::AddModel(LWObjectImport *local,Anima::MeshExtModel *meshExt)
{
	// punti

	Anima::ModelInstance *model = meshExt->GetModel();
	Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

	LWFVector pos = { 0.0f };

	int startVertexIdx = totalVertexCount;

	LWPntID pID;
	int vertexCount = meshExt->GetVertexCount(); // mesh->GetControlPointsCount();
	float *vertices = meshExt->GetVertices();

	LWPntID *pntID = new LWPntID[vertexCount];
	LWPntID vPntID[4];
	int c=0;
	for(int i=0;i<vertexCount;i++)
	{
		pos[0] = vertices[c++] + ((lw_time>0) ? 10 : 0);
		pos[1] = vertices[c++];	// y = z
		pos[2] = -vertices[c++];	// z = -y
		pID = local->point( local->data, pos );
		pntID[i] = pID;
		pointMap->insert(pID,totalVertexCount++);
	}

	lw_time++;

	// poly + textures
	char matName[100];

	size_t *triUVList = meshExt->GetUVTriList();
	float *uvList = meshExt->GetUVList();
	int triCount = meshExt->GetTriCount()/3;
	size_t *triList  = meshExt->GetTriList();
	size_t *triMatEndList  = meshExt->GetTriEndList();
	size_t v[3];
	int matIdx=0;
	c=0;
	int endTriMat = triMatEndList[matIdx]/3;
	float valarray[2] = {0,0};

	/*for(int m=0;m<meshExt->GetMaterialCount();m++)
	{
		local->vmap( local->data, LWVMAP_TXUV ,2, QString("%1_Txt_%2").arg(model->GetModel()->GetName()).arg(m).toAscii().constData() ) ;
	}*/

	for(int t=0;t<triCount;t++)
	{
		if( t == 0 || t == endTriMat)
		{
			// cambio materiale
			if (t != 0)
				endTriMat = triMatEndList[++matIdx]/3;

			QString _matName = QString("%1_%2").arg(model->GetModel()->GetName()).arg(matIdx);
			strcpy(matName,_matName.toAscii().data());
		
			QString _mapName= QString("%1_Txt_%2").arg(model->GetModel()->GetName()).arg(matIdx);

			local->vmap( local->data, LWVMAP_TXUV ,2,_mapName.toAscii().constData() ) ;
		}

		v[0] = triList[c+0];
		v[2] = triList[c+1];
		v[1] = triList[c+2];

		vPntID[0] = pntID[v[0]];
		vPntID[1] = pntID[v[1]];
		vPntID[2] = pntID[v[2]];
		LWPolID polID = local->polygon( local->data, LWPOLTYPE_FACE, 0, 3, vPntID );
		local->polTag( local->data, polID, LWPTAG_SURF, matName );

		for (int lVerticeIndex = 0; lVerticeIndex < 3; lVerticeIndex++)
		{
			//int lCurrentUVIndex;
			////if (lMappingMode == KFbxLayerElement::eBY_POLYGON_VERTEX)
			//{
			//	lCurrentUVIndex = mesh->GetTextureUVIndex(lPolygonIndex, lVerticeIndex);
			//}
			//KFbxVector2 &uv = lUVArray[lCurrentUVIndex];

			int vv = triUVList[c+lVerticeIndex];

			valarray[0] = uvList[vv * 2];
			valarray[1] = 1-uvList[vv * 2 +1];

			local->vmapPDV( local->data,vPntID[lVerticeIndex], polID, valarray );
		}
		c+=3;
	}
	delete pntID;
}

void AnimaStore::OnDrawModel(Anima::GfxModel *model)
{
	// TODO
//	memcpy(TotalVertexArray + baseVertexCount,model->VertexArray,model->VertexCount * sizeof(Ogre::Vector3));
//	baseVertexCount+= model->VertexCount;
}

void AnimaStore::NewTime( AnimaData *dat, LWFrame fr, LWTime t,double fps  )
{
//	baseVertexCount=0;
	currentVertex=0;
	// faccio il render di tutti i modelli su il vettore dei punti

	if (meshExporter!=NULL) delete meshExporter;

	//memset(TotalVertexArray,0,totalVertexCount * sizeof(Ogre::Vector3));

	//Anima::Context::Singleton.GetFilm().SetFilmDrawMode(Anima::Film::DrawMode_Points);

	// TODO
	Anima::Context::Singleton.GetFilm().SetFPS(true,fps);
	Anima::Context::Singleton.GetFilm().SetPlayFrame(fr);
	//Anima::Context::Singleton.ForceUIUpdate();

	//Anima::GfxContext::Singleton.OnResize();

	//renderWindow->update();
	Anima::GfxContext::Singleton->UpdateAndWaitTask();

	meshExporter = new Anima::MeshExporter();
	meshExporter->BuildCurrentTime();

	//Anima::Context::Singleton.GetFilm().DrawFrame(false,fr); // disegno
}

void AnimaStore::DisplaceLWPoint( AnimaData *dat, LWDisplacementAccess *da )
{
	if (meshExporter == NULL) return;
	int pointID = (*pointMap)[da->point];
	/*if (currentVertex != pointID)
	{
		AnimaDoCoreException("bad point number");
	}*/

	uint subVertexIdx;
	Anima::MeshExt *mesh = meshExporter->GetModelExportColl().GetModelByVertexIdx(pointID,subVertexIdx);
	if (mesh == NULL) return;
	
	float *v = mesh->GetVertices();
	subVertexIdx = subVertexIdx * 3;
	
	///*Ogre::Vector3 &v = TotalVertexArray[pointID];

	da->source[0] = v[subVertexIdx];
	da->source[1] = v[subVertexIdx+1];
	da->source[2] = -v[subVertexIdx+2];

	currentVertex++;
}