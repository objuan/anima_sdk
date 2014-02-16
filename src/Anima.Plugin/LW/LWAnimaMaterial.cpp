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
#include "Export/PresetsExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"

#include "./GLView.h"
//#include "Import/ImportLWO.h"

int AnimaStore::LoadSurf( const char *filename,int clipIdx,QString &txtMap,unsigned char *buf,int fileSize,int *startBufferIdx)
{
	FILE *fp;
	char buf_aus[1000];
	int c,i;
	int vmapMarker,blockMarker,postVmapMarker,imagMarker,surfMarker;

	fp = fopen(filename, "rb" );
	if ( !fp ) return -1;

	c = fread(buf,fileSize,1,fp);

	fclose( fp );

	// letto, calcolo i marker

	for(i=0;i<fileSize-4;i++)
	{
		if (buf[i] == 'S' && buf[i+1] == 'U' && buf[i+2] == 'R' && buf[i+3] == 'F')
			surfMarker=i;
		else if (buf[i] == 'V' && buf[i+1] == 'M' && buf[i+2] == 'A' && buf[i+3] == 'P')
			vmapMarker=i;
		else if (buf[i] == 'B' && buf[i+1] == 'L' && buf[i+2] == 'O' && buf[i+3] == 'K')
			blockMarker=i;
		else if (buf[i] == 'I' && buf[i+1] == 'M' && buf[i+2] == 'A' && buf[i+3] == 'G')
			imagMarker=i;
	}

	*startBufferIdx = surfMarker;

	return fileSize;
/*
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
	*/
}

void AnimaStore::AddMaterials(LWObjectImport *local,Anima::MeshExporter &_meshExporter)
{
	char tag[ 40 ];
	unsigned char data[300000 ];
	char name[ 40 ];
	const char *layerName;
	int size;

	LWItemInfo *iteminfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	LWImageList *imglist = (LWImageList *)global( LWIMAGELIST_GLOBAL, GFUSE_TRANSIENT );
	LWTextureFuncs *txfunc= (LWTextureFuncs *)global( LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT );
	LWSurfaceFuncs *surff = (LWSurfaceFuncs *)global( LWSURFACEFUNCS_GLOBAL, GFUSE_TRANSIENT );

	Anima::RenderPresetInfo *qualityInfo =  Anima::Context::Singleton.GetRenderInfo()->CurrentPreset;

	Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	QString current_render_type="STANDARD";

	if (!Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial())
	{
		int matCount=0;
		// multimaterial
		for(int i=0;i<meshList.InstanceCount();i++) 
			matCount+= meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();

		int matIdx=0;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::ModelExporterRender *modelExporterRender =NULL;
			Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
		
			Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

			Anima::ModelExporter *_modelExporter = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->GetExporter(Anima::ModelExporterType::LW,9);
			
			if (_modelExporter!= NULL)
				modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);

			matCount = mesh->getNumSubMeshes();
			for(int t=0;t<matCount;t++)
			{
				Anima::MeshExtSubmeshInfo &subInfo = meshExt->GetSubInfo(t);
				QString partName = subInfo.name; //QString("%1").arg((t == 1) ? "Head" : " Body");
				Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
			
				// ho i dati del materiale di ogre

				if (modelExporterRender!=NULL && qualityInfo->Folder !="")
				{
					Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource(subMesh->getMaterialName().c_str());
					if (expMat!=NULL)
					{
						QString name = QString("an(i)ma_%1_%2").arg(meshExt->GetModel()->GetID()).arg(t);
						QString path = expMat->GetFullMeshExternalFilePath();
						QString matName=expMat->meshExternalName;
						QString mapPath = QString("%1_Txt_%2").arg(meshExt->GetModel()->GetID()).arg(t);
				
						int subIdx=-1;
						QStringList matNameList = matName.split(",");
						if (matNameList.count() == 2)
						{
							matName = matNameList[0];
							subIdx =  matNameList[1].toInt();
						}

						int fsize = QFile(path).size();
						int startIdx= 0;
						size = LoadSurf(path.toAscii().constData(),1,mapPath, data,fsize,&startIdx );

						local->surface( local->data , name.toAscii().constData(), NULL, size, data+startIdx );
					}
					else
					{
						//// metto un materiale vuoto
						
					}
				}
				else
				{
					// metto un materiale vuoto
				
				}
			}
			matIdx+= matCount;
		}
	}
	else
	{
		// single material
		Anima::ModelExporter *_modelExporter = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->GetExporter(Anima::ModelExporterType::LW,9);
		if (_modelExporter!= NULL)
		{
			Anima::ModelExporterRender *modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);
			Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource("All");
			if (expMat!=NULL)
			{
				QString name =  QString("Anima_%1").arg(Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->Name);
				QString path = expMat->GetFullMeshExternalFilePath();
				QString matName=expMat->meshExternalName;
				int subIdx=-1;
				QStringList matNameList = matName.split(",");
				if (matNameList.count() == 2)
				{
					matName = matNameList[0];
					subIdx =  matNameList[1].toInt();
				}

			}
		}
	}

	////Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	////for(int i=0;i<meshList.ModelCount();i++)
	////{
	////	Anima::MeshExtModel *meshExt = (Anima::MeshExtModel *)meshList.GetModel(i);
	////	Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

	////	int txtCount = mesh->getNumSubMeshes();
	////	for(int t=0;t<txtCount;t++)
	////	{
	////		Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
	////		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(subMesh->getMaterialName());
	////		Ogre::Technique *tec = mat->getTechnique(0); // il primo
	////		Ogre::Pass *pass = tec->getPass(0);
	////		int txtSubNum = pass->getNumTextureUnitStates();
	////		Ogre::TextureUnitState * txt = pass->getTextureUnitState(0);
	////		Ogre::String txtName = txt->getFrameTextureName(0);

	////		QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
	////		QString modelDir = Anima::Context::Singleton.GetModelLibrary().GetModelDir(meshExt->GetModel()->GetModel());
	////		QString imagePath = modelDir+"/"+txtName.c_str();
	////		QString mapPath = QString("%1_Txt_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(t);
	////		QString defPath = Anima::Config::DataRootPath()+"/plugin/lw/default.srf";

	////		LWImageID  imageID =  imglist->load(imagePath.toAscii().constData());

	////		int fsize = QFile(defPath).size();
	////		size = LoadSurf(defPath.toAscii().constData(),1,mapPath, data,fsize );

	////		local->surface( local->data, name.toAscii().constData(), NULL, size, data );

	////		// mi cerco e cambio le txt
	////		LWSurfaceID surfID = surff->first();
	////		do
	////		{
	////			if (surfID != NULL)
	////			{
	////				if (strcmp(surff->name(surfID),name.toAscii().constData()) == 0)
	////				{
	////					LWTextureID txt = surff->getTex(surfID,SURF_COLR);
	////					LWTLayerID txtLayer = txfunc->firstLayer(txt);
	////					int result = txfunc->setParam( txtLayer, TXTAG_IMAGE, &imageID );
	////				}
	////				surfID = surff->next(surfID);
	////			}
	////		}
	////		while(surfID!=NULL);
	////	}
	////}
}