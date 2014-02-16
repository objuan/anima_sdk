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
 

int AnimaStore::Load( LWObjectImport *local,void *serverData)
{
	QString fileName(local->filename);
	if (!fileName.endsWith(".ani")) return 1;
		
	EnsureInit();

	if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;

	// load

	// tolgo l'oggetto di anima se ce'

	const char *fname;
	LWItemInfo *iteminfo;
	LWObjectInfo *objinfo;
	LWItemID id;

	iteminfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	objinfo  = (LWObjectInfo *)global( LWOBJECTINFO_GLOBAL, GFUSE_TRANSIENT );
	QList<LWItemID> toRemove;

	if ( iteminfo && objinfo ) {
		id = iteminfo->first( LWI_OBJECT, NULL );
		while ( id ) 
		{
			 fname  = objinfo->filename( id );
			 QString namePath = QString::fromLocal8Bit(fname);

			 // trucco per capire che e' l'oggetto anima
			 QString ani_namePath = QString("%1.ani").arg(namePath.mid(0,namePath.length()-4));
			 if (QFile(ani_namePath).exists())
			 {
				 // era l'oggetto di anima, lo tolgo
				 toRemove.append(id);
			 }

			 id = iteminfo->next( id );
	   }
	}

	foreach(LWItemID id , toRemove)
	{
		LWCommandFunc *evaluate;
		evaluate = (LWCommandFunc *)global( LWCOMMANDFUNC_GLOBAL, GFUSE_TRANSIENT );

		QString cmd1 =QString("SelectItem x%1").arg((int)id,0,16);
		QString cmd2 = QString("ClearSelected  0");

		evaluate(cmd1.toAscii().data());

		evaluate(cmd2.toAscii().data());
	}

	if (toRemove.count()>0)
	{
		//QMessageBox::warning(0,"Warning","Due Lighwave ");
		return 1;
	}

	// ------

	Clear();
	
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

	// --------------------

	/*char tag[ 40 ];
	unsigned char data[50000 ];
	char name[ 40 ];
	const char *layerName;
	int size;*/

	/*LWItemInfo *iteminfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	LWImageList *imglist = (LWImageList *)global( LWIMAGELIST_GLOBAL, GFUSE_TRANSIENT );
	LWTextureFuncs *txfunc= (LWTextureFuncs *)global( LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT );
	LWSurfaceFuncs *surff = (LWSurfaceFuncs *)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );*/

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
	local->layer( local->data, 1, "Anima Object" );
	local->pivot( local->data, pos );

	Anima::GfxContext::Singleton->UpdateAndWaitTask();

	Anima::MeshExporter _meshExporter;
	_meshExporter.BuildCurrentTime();

	AddMaterials(local,_meshExporter);

	//// materials && textures

	//Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	//for(int i=0;i<meshList.ModelCount();i++)
	//{
	//	Anima::MeshExtModel *meshExt = (Anima::MeshExtModel *)meshList.GetModel(i);
	//	Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

	//	int txtCount = mesh->getNumSubMeshes();
	//	for(int t=0;t<txtCount;t++)
	//	{
	//		Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
	//		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(subMesh->getMaterialName());
	//		Ogre::Technique *tec = mat->getTechnique(0); // il primo
	//		Ogre::Pass *pass = tec->getPass(0);
	//		int txtSubNum = pass->getNumTextureUnitStates();
	//		Ogre::TextureUnitState * txt = pass->getTextureUnitState(0);
	//		Ogre::String txtName = txt->getFrameTextureName(0);

	//		QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
	//		QString modelDir = Anima::Context::Singleton.GetModelLibrary().GetModelDir(meshExt->GetModel()->GetModel());
	//		QString imagePath = modelDir+"/"+txtName.c_str();
	//		QString mapPath = QString("%1_Txt_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
	//		QString defPath = Anima::Config::DataRootPath()+"/plugin/lw/default.srf";

	//		LWImageID  imageID =  imglist->load(imagePath.toAscii().constData());

	//		int fsize = QFile(defPath).size();
	//		size = LoadSurf(defPath.toAscii().constData(),1,mapPath, data,fsize );

	//		local->surface( local->data, name.toAscii().constData(), NULL, size, data );

	//		LWSurfaceID surfID = surff->first();
	//		do
	//		{
	//			if (surfID != NULL)
	//			{
	//				if (strcmp(surff->name(surfID),name.toAscii().constData()) == 0)
	//				{
	//					LWTextureID txt = surff->getTex(surfID,SURF_COLR);
	//					LWTLayerID txtLayer = txfunc->firstLayer(txt);
	//					int result = txfunc->setParam( txtLayer, TXTAG_IMAGE, &imageID );
	//				}
	//				surfID = surff->next(surfID);
	//			}
	//		}
	//		while(surfID!=NULL);
	//	}
	//}

	// LOAD

	totalVertexCount=0;

	pointMap->clear();
//	if (TotalVertexArray!=NULL) delete[] TotalVertexArray;

	Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	for(int i=0;i<meshList.InstanceCount();i++)
	{
		Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
		AddModel(local,meshExt);
	}

	// cancello lwo
	 QString ani_namePath = QString("%1.lwo").arg(fileName.mid(0,fileName.length()-4));

	 if (QFile(ani_namePath).exists())
		QFile(ani_namePath).remove();
	// ini 
	//Anima::GfxContext::Singleton->UpdateAndWaitTask();

	//meshExporter = new Anima::MeshExporter();
	//meshExporter->BuildCurrentTime();

	return 0;
}
