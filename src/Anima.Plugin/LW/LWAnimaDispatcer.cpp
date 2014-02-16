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
//typedef size_t      LWPntScanFunc   (void *, LWPntID);

size_t __cdecl
pntScan( void *data, LWPntID id) 
{
   ObjectDB *odb = (ObjectDB *)data;
    odb->pt[ odb->npoints ].id = id;
    ++odb->npoints;
   return 0;
}
int __stdcall
Scan(LWMeshInfo *mesh,ObjectDB *odb)
{
	if ( mesh->scanPoints( mesh, pntScan, (void *)odb ))
		return 0;
	return 1;
}

int AnimaStore::DisplacementInit(LWDisplacementHandler *local, void *serverData)
{
	EnsureInit();

	if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;

	// salvo la copia dell'oggetto

   LWItemInfo *iteminfo;
   LWObjectInfo *objinfo;
   LWItemID id;
   LWTime t = 3.0;          /* seconds */
   const char *fname;
   int npoints, npols;
   unsigned int shopts;
   double dissolve;

   iteminfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
   objinfo  = (LWObjectInfo *)global( LWOBJECTINFO_GLOBAL, GFUSE_TRANSIENT );

   if ( iteminfo && objinfo ) {
      id = iteminfo->first( LWI_OBJECT, NULL );
      if ( id ) {
         fname  = objinfo->filename( id );
		 QString namePath = QString::fromLocal8Bit(fname);

		 // trucco per capire che e' l'oggetto anima
		 QString ani_namePath = QString("%1.ani").arg(namePath.mid(0,namePath.length()-4));
		 if (QFile(ani_namePath).exists())
		 {
			if (!QFile(namePath).exists())
			{
				// fase di LOAD , salvo l'oggetto 
				 LWCommandFunc *evaluate;
				 evaluate = (LWCommandFunc *)global( LWCOMMANDFUNC_GLOBAL, GFUSE_TRANSIENT );

				QString cmd1 =QString("SelectItem x%1").arg((int)id,0,16);
				QString cmd2 = QString("SaveObjectCopy %1").arg(namePath);

				evaluate(cmd1.toAscii().data());

				evaluate(cmd2.toAscii().data());
			}
			else
			{
				// fase di RELOAD
			
				try
				{
					Anima::Context::Singleton.Clear();
	
					Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("loading '%1'..").arg(QFileInfo(ani_namePath).fileName()),4,true);

					Anima::ContextIO contextIO;
					contextIO.Load(ani_namePath,false,progressBar); // carico nell'ambiente

					totalVertexCount=0;
					pointMap->clear();
				
					Anima::GfxContext::Singleton->UpdateAndWaitTask();

					Anima::MeshExporter _meshExporter;
					_meshExporter.BuildCurrentTime();

					LWMeshInfo *mesh;
					mesh = objinfo->meshInfo( id, 1 );
					if ( !mesh ) return 0;

					lwObject=new ObjectDB();
					int npts = mesh->numPoints( mesh );
				    lwObject->pt = new DBPoint[npts];
				    if ( !lwObject->pt ) return 0;

				    if ( !Scan( mesh, lwObject ))
					  return 0;

					// controllo il numero dei vertici ..

					int numVertex=0;
					Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
					for(int i=0;i<meshList.InstanceCount();i++)
					{
						Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
						numVertex+= meshExt->GetVertexCount();
						/*Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
						NotifyModel(local,mesh,&verteIdx,meshExt);*/
					}
					if (numVertex!= lwObject->npoints)
					{
						delete lwObject;
						delete progressBar;
						return 0;
					}
					for(int i=0;i<numVertex;i++)
					{
						pointMap->insert(lwObject->pt[i].id,i);
					}

					delete lwObject;
					delete progressBar;
				}
				catch(Anima::CoreException &e)
				{
					return 0;
				}
			}
		}
      }
   }

	return 1; // OK
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