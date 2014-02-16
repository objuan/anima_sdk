#include "AnimaIncludes.h"
#include "AnimaObject.h"
#include "AnimaMeshObject.h"
//#include "c4d.h"
#include "c4d_symbols.h"
#include "oanima.h"
#include "Utils.h"
#include "oldversions.h"
#include "ColorWeightShader.h"
#include "xcolorweight.h"

Real AnimaObject::g_endTime = MAXREALs;

AnimaObject::AnimaObject() : m_lastTime(MAXREALs)
{
	m_bCached = FALSE;
	m_bboxCount = 0;
	m_obj = NULL;
	m_lastDarkness = MAXREALs;
	m_infos = AnimaInfos();
	m_clearInfos = FALSE;
	m_lastDeformMode = TRUE;
}
AnimaObject::~AnimaObject()
{
	BaseObject::Free(m_obj);
}
		
Bool AnimaObject::Init(GeListNode *node)
{
	BaseObject		*op		= (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();
	data->SetInt32(ANIMAOBJECT_BBOX_MODE, 100);
	m_bCached = FALSE;
	BaseObject::Free(m_obj);
	return TRUE;
}
Bool AnimaObject::CopyTo(NodeData* dest, GeListNode* snode, GeListNode* dnode, COPYFLAGS flags, AliasTrans* trn)
{
	if(this->m_obj)
	{
		BaseObject::Free(((AnimaObject*)dest)->m_obj);
		((AnimaObject*)dest)->m_obj = (BaseObject*) this->m_obj->GetClone(flags, trn);
	}
	return ObjectData::CopyTo(dest, snode, dnode, flags, trn);
}

Bool AnimaObject::GetDParameter(GeListNode* node, const DescID& id, GeData& t_data, DESCFLAGS_GET& flags)
{
	switch(id[0].id)
	{
		case ANIMAOBJECT_SCENEINFO_PRESET:
			{
				flags |= DESCFLAGS_GET_PARAM_GET;
				QString name = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->Name;
				t_data.SetString(Utils::toC4DString(name));
				break;
			}
		case ANIMAOBJECT_SCENEINFO_ACTORS:
			flags |= DESCFLAGS_GET_PARAM_GET;
			t_data.SetString(LongToString(m_infos.shownActors) + " of " + LongToString(m_infos.actors));
			break;
		case ANIMAOBJECT_SCENEINFO_POLYS:
			flags |= DESCFLAGS_GET_PARAM_GET;
			t_data.SetString(LongToString(m_infos.shownPolys) + " of " + LongToString(m_infos.polys));
			break;
		case ANIMAOBJECT_SCENEINFO_POINTS:
			flags |= DESCFLAGS_GET_PARAM_GET;
			t_data.SetString(LongToString(m_infos.shownVerts) + " of " + LongToString(m_infos.verts));
			break;
		case ANIMAOBJECT_SCENEINFO_RENDERER:
			{
				BaseDocument* doc = node->GetDocument();
				String st;
				if(!doc)
					break;
				RenderData* rd = doc->GetActiveRenderData();
				if(!rd)
					break;
				switch(rd->GetDataInstance()->GetInt32(RDATA_RENDERENGINE) )
				{
					case RDATA_RENDERENGINE_STANDARD:
						st = "Standard";
						break;
					case VRAY_VP:
						st = "Vray";
						break;
				}
				flags |= DESCFLAGS_GET_PARAM_GET;
				t_data.SetString(st);
				break;
			}
        case ANIMAOBJECT_INFO_BMP:
        {
            BitmapButtonStruct bbs((BaseObject*)node, id, 0);
            t_data.SetCustomDataType(CUSTOMDATATYPE_BITMAPBUTTON, bbs);
            flags |=        DESCFLAGS_GET_PARAM_GET;
            break;
        }
	}
	return ObjectData::GetDParameter(node, id, t_data, flags);
}

Bool AnimaObject::Message(GeListNode *node, Int32 type, void *t_data)
{
	switch(type)
	{
		case MSG_DEFORMMODECHANGED:
		{
			if(((BaseObject*)node)->GetDeformMode() && m_lastDeformMode==FALSE)
				RebuildObject((BaseObject*)node, node->GetDocument());
			m_lastDeformMode=((BaseObject*)node)->GetDeformMode();
			break;
		}
		case MSG_DOCUMENTINFO:
		{
			DocumentInfoData* docdata = (DocumentInfoData*)t_data;
			if(docdata && docdata->type == MSG_DOCUMENTINFO_TYPE_SETACTIVE)
			{
				RebuildObject((BaseObject*)node, node->GetDocument());
			}
			break;
		}
		case MSG_DESCRIPTION_COMMAND:
		{
			if(((DescriptionCommand*)t_data)->id == DescID(ANIMAOBJECT_RELOAD))
			{
				RebuildObject((BaseObject*)node, node->GetDocument(), TRUE, TRUE);
			}
			break;
		}
		case MSG_DESCRIPTION_VALIDATE:
		case MSG_DESCRIPTION_POSTSETPARAMETER:
		case MSG_CHANGE:
		{
			m_bboxCount = 0;
			Filename newPath = ((BaseObject*)node)->GetDataInstance()->GetFilename(ANIMAOBJECT_PATH);
			if(m_lastFilename.GetString().Compare(newPath.GetString()) != 0)
			{
				m_lastFilename = newPath;
				RebuildObject((BaseObject*)node, node->GetDocument());
			}

			Real curDarkness = ((BaseObject*)node)->GetDataInstance()->GetFloat(ANIMAOBJECT_DARKNESS);
			if(m_lastDarkness != curDarkness)
			{
				m_lastDarkness = curDarkness;
				Utils::adjustAllMaterialsDarkness(node->GetDocument(), curDarkness);
			}
			break;
		}
		case MSG_MULTI_DOCUMENTIMPORTED:
		{
			RebuildObject((BaseObject*)node, node->GetDocument());
			break;
		}
		case MSG_MULTI_MARKMATERIALS:
		{
			if (!t_data)
			{
				AutoAlloc<AtomArray> animaMats;
				Utils::getAllAnimaMaterials(node->GetDocument(), animaMats);
				for(Int32 i = 0; i < animaMats->GetCount(); i++)
				{
					BaseMaterial* mat = (BaseMaterial*)animaMats->GetIndex(i);
					if (mat) mat->SetBit(BIT_MATMARK);
				}
			}
			break;
		}
		case MSG_MENUPREPARE:
		{
			Filename f;
			if(f.FileSelect(FILESELECTTYPE_ANYTHING, FILESELECT_LOAD, "Select the .ani File"))
			{
				if (Anima::Config::IsDemo())
				{
					MessageDialog("an(i)ma Trial Edition\r\n\
\r\n\
This Software is designed to provide everyone with an opportunity to fully\
experience the latest version of AXYZ design® an(i)ma® 3D Character animation\
application.\r\n\
 \r\n\
This trial version will allow you to do rendering tests and animations for a \
duration of 4 seconds.\r\n\
 \r\n\
Other than the animation duration limit, this trial version has full functionality.");
				}

				BaseDocument* doc = (BaseDocument*)t_data;
				((BaseObject*)node)->GetDataInstance()->SetFilename(ANIMAOBJECT_PATH, f);

				Anima::ContextIO contextIO;
				Anima::AnimaFileInfo *fileInfo = contextIO.PreloadFileInfo(Utils::toQString(f.GetString()));
				if (!fileInfo->HaveCollision)
				{
					String st = "The .ani scene file you are trying to load does not have any collision data saved.\r\n\
If you proceed, collisions are going to be calculated while loading.\r\n\
This process may take some time.";
					if(!QuestionDialog(st))
						return FALSE;
				
				}
				RebuildObject((BaseObject*)node, doc, FALSE, TRUE);
			}
			else
				return FALSE;
			break;
		}
		case MSG_DESCRIPTION_GETBITMAP:
		{
			DescriptionGetBitmap* data = (DescriptionGetBitmap*)t_data;
			if(data->id[0].id == ANIMAOBJECT_INFO_BMP)
			{
				Filename bg = GeGetPluginPath()+Filename("res")+Filename("Logo_Anima.tif");
				if (!GeFExist(bg))    return TRUE;
				AutoAlloc<BaseBitmap>    bm;
				bm->Init(bg);
				data->bmp = bm.Release();					
			}
			break;
		}
	}
	return TRUE;
}

void retargetShader(BaseDocument* doc, BaseShader* shader, const Filename& path1, const Filename& path2, const String& matPrefix)
{
	if(shader)
	{
		if(shader->GetType() == Xbitmap)
		{
			GeData dat;
			shader->GetParameter(DescLevel(BITMAPSHADER_FILENAME), dat, DESCFLAGS_GET_0);
			if(GeGetCurrentOS() == OPERATINGSYSTEM_WIN)
				dat.SetFilename(Utils::replaceAll(dat.GetFilename().GetString(), '/', '\\'));
			else
				dat.SetFilename(Utils::replaceAll(dat.GetFilename().GetString(), '\\', '/'));
			Filename f = dat.GetFilename();
			if(f.Content())
			{
				f = path1 + (matPrefix + dat.GetFilename().GetFileString());
				if(!GeFExist(f))
				{
					f = path1 + (dat.GetFilename().GetFileString());
					if(!GeFExist(f))
					{
						f = path2 + (matPrefix + dat.GetFilename().GetFileString());
						if(!GeFExist(f))
							f = path2 + (dat.GetFilename().GetFileString());
					}
				}
				if(!GeFExist(f))
					Utils::addLog("tex not found: " + f.GetString());
				dat.SetFilename(f);
				shader->SetParameter(DescLevel(BITMAPSHADER_FILENAME), dat, DESCFLAGS_SET_0);
				shader->Message(MSG_CHANGE);
			}
		}
		BaseShader* child = shader->GetDown();
		while(child)
		{
			retargetShader(doc, child, path1, path2, matPrefix);
			child = child->GetNext();
		}
	}
}
void retargetAllPaths(BaseDocument* doc, BaseContainer* bc, const Filename& path1, const Filename& path2, const String& matPrefix)
{
	if(bc)
	{
		BrowseContainer browse(bc);
		Int32 id = 0;
		GeData* data = NULL;
		while(browse.GetNext(&id, &data))
		{
			if(data->GetType() == DA_ALIASLINK)
			{
				BaseList2D* shader = data->GetLink(doc);
				if(shader && shader->IsInstanceOf(Xbase))
					retargetShader(doc, (BaseShader*)shader, path1, path2, matPrefix);

			}
			else if(data->GetType() == DA_CONTAINER)
			{
				retargetAllPaths(doc, data->GetContainer(), path1, path2, matPrefix);
			}
		}
	}
}

BaseMaterial* loadMaterialFile(const Filename& path, const String& matName, const Filename& modelTexFolder, const Filename& presetTexFolder, const String& matPrefix)
{
	BaseMaterial* mat = NULL;
	if(path.Content() && GeFExist(path))
	{
		BaseDocument* doc = LoadDocument(path, SCENEFILTER_MATERIALS, NULL);
		if(doc)
		{
			BaseMaterial* m = doc->SearchMaterial(matName);
			if(!m)
			{
				Utils::addLog("Material " + matName + " not found in " + path.GetString());
				m=doc->GetFirstMaterial();
			}
			if(m)
			{
				retargetAllPaths(doc, m->GetDataInstance(), modelTexFolder, presetTexFolder, matPrefix);
				mat = (BaseMaterial*) m->GetClone(COPYFLAGS_0, NULL);
			}

			BaseDocument::Free(doc);
		}
	}
	else
		Utils::addLog("file not found: " + path.GetString());
	return mat;
}
void changeRgb(BaseDocument* doc, BaseMaterial*mat, Anima::ModelExporterRenderMaterial* expMat, Anima::MeshExtModel* meshExt)
{
	if(mat && expMat && meshExt)
	{
		Anima::ModelInstance* mi = meshExt->GetModel();
		BaseShader* colorShader = NULL;
		switch(mat->GetType())
		{
			case Mmaterial:
				colorShader = (BaseShader*) mat->GetDataInstance()->GetLink(MATERIAL_COLOR_SHADER, doc);
				break;
			case VRAY_MAT:
				colorShader = (BaseShader*) mat->GetDataInstance()->GetLink(VRAYMATERIAL_COLOR_SHADER, doc);
				break;
		}
		if(colorShader)
		{
			AutoAlloc<AtomArray> arr;
			Utils::getAllSubShaderOfType(doc, colorShader, *arr, ID_COLORWEIGHTSHADER);
			Int32 maskPos=0;
			for(Int32 i=0; i< arr->GetCount(); i++)
			{
				BaseShader* weightShader = (BaseShader*) arr->GetIndex(i);
				QColor col = expMat->GetColor(mi, maskPos);
				Vector v(col.redF(), col.greenF(), col.blueF());
				weightShader->SetParameter(DescLevel(COLORWEIGHT_COLOR), GeData(v), DESCFLAGS_SET_0);
				weightShader->Message(MSG_CHANGE);
				maskPos++;
			}
		}
	}
}
QString getCurrentRenderer(BaseDocument* doc)
{
	QString renderer = "STANDARD";
	RenderData* rd = doc->GetActiveRenderData();
	switch(rd->GetDataInstance()->GetInt32(RDATA_RENDERENGINE))
	{
		case RDATA_RENDERENGINE_STANDARD: renderer="STANDARD"; break;
		case VRAY_VP: renderer="VRAY"; break;
	}
	return renderer;
}
void BuildMaterials(BaseDocument* doc, Anima::MeshExtCollection* meshList, Anima::GeomExtCollection* geomList)
{
	if(doc)
	{
		Utils::addLog("AnimaObject:: Building Materials");
		Anima::RenderPresetInfo *qualityInfo =  Anima::Context::Singleton.GetRenderInfo()->CurrentPreset;
		LayerObject* matLayer = Utils::getAnimaMatLayer(doc);
		Int32 matCount=0;
		Int32 matIdx=0;
		QString renderer = getCurrentRenderer(doc);
		Bool bSingleMaterial = Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial();

		for(Int32 i=0;i<meshList->InstanceCount();i++)
		{
			Anima::ModelExporterRender *modelExporterRender =NULL;
			Anima::ModelExporterRender *modelExporterRenderOnlyColor =NULL;
			Anima::MeshExtModel *meshExt = meshList->GetInstance(i);
		
			Ogre::MeshPtr mesh = meshExt->GetOgreMesh();
			Anima::ModelExporter *_modelExporter = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->GetExporter(Anima::C4D,12);
			Anima::ModelExporter *onlyColor_modelExporter = meshExt->GetModel()->GetModel()->GetExporter(Anima::C4D,12);

			// new preset material
			if (_modelExporter!= NULL)
			{
				modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)renderer,0);
				if(onlyColor_modelExporter)
					modelExporterRenderOnlyColor = onlyColor_modelExporter->GetRender((Anima::ModelExporterRenderType)renderer,0);
				else
					modelExporterRenderOnlyColor = modelExporterRender;
			}

			matCount = mesh->getNumSubMeshes();
			for(int t=0;t<matCount;t++)
			{
				String stName = ANIMA_MAT_PRE+LongToString(matIdx+t);
				Anima::MeshExtSubmeshInfo &subInfo = meshExt->GetSubInfo(t);
				QString partName = subInfo.name; //QString("%1").arg((t == 1) ? "Head" : " Body");
				//Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
				QString _matName = meshExt->getMaterialName(t);

				BaseMaterial* mat = NULL;
				Anima::ModelExporterRenderMaterial *expMat = NULL;
				Anima::ModelExporterRenderMaterial *expMatOnlyColor = NULL;
				if (modelExporterRender!=NULL)
				{
					expMat = modelExporterRender->FindMatBySource(_matName);
					expMatOnlyColor = modelExporterRenderOnlyColor->FindMatBySource(_matName);
					if(bSingleMaterial)
						expMat = modelExporterRender->FindMatBySource("All");

					String modelExternalFilePath = "";
					String presetExternalFilePath = "";

					if (expMatOnlyColor!=NULL)
						modelExternalFilePath = Utils::toC4DString(expMatOnlyColor->GetFullMeshExternalFilePath());
					if (expMat!=NULL)
						presetExternalFilePath = Utils::toC4DString(expMat->GetFullMeshExternalFilePath());
					else
						Utils::addLog("expmat null");

					String modelMatPath = modelExternalFilePath;
					if (GeGetCurrentOS() == OPERATINGSYSTEM_WIN)
						modelMatPath = Utils::replaceAll(modelMatPath, '/', '\\');
					else
						modelMatPath = Utils::replaceAll(modelMatPath, '\\', '/');

					
					if(!GeFExist(Filename(modelMatPath), TRUE))
						modelMatPath = Filename(modelMatPath).GetDirectory().GetString();
					Filename modelTexpath = Filename(modelMatPath) + "textures" + Utils::toC4DString(qualityInfo->Folder);

					String presetMatPath = presetExternalFilePath;
					if (GeGetCurrentOS() == OPERATINGSYSTEM_WIN)
						presetMatPath = Utils::replaceAll(presetMatPath, '/', '\\');
					else
						presetMatPath = Utils::replaceAll(presetMatPath, '\\', '/');
					Filename presetTexpath = Filename(presetMatPath).GetDirectory() + "textures" + Utils::toC4DString(qualityInfo->Folder);

					String matName = Utils::toC4DString(_matName);
					String matPrefix = "";
						
					Int32 pos = -1;
					if(matName.FindLast("_", &pos))
					{
						matPrefix = matName.SubStr(0, pos);
						matName = matName.SubStr(matPrefix.GetLength()+1, matName.GetLength() - matPrefix.GetLength());
					}

					mat = loadMaterialFile(Filename(presetMatPath), matName, modelTexpath, presetTexpath, matPrefix); 
					if(!mat)
					{
						mat = BaseMaterial::Alloc(Mmaterial);
						Utils::addLog("loadMaterialFile null");
					}
				}
				else
				{
					mat = BaseMaterial::Alloc(Mmaterial);
					Utils::addLog("modelExporterRender null");
				}

				if(mat)
				{
					mat->SetName(stName);
					BaseMaterial* oldmat = doc->SearchMaterial(stName);
					if(oldmat)
					{
						oldmat->Remove();
						BaseMaterial::Free(oldmat);
					}
					doc->InsertMaterial(mat);

					changeRgb(doc, mat, expMatOnlyColor, meshExt);

					mat->SetLayerObject(matLayer);
					mat->Message(MSG_CHANGE);

					if(bSingleMaterial)
					{
						mat->SetName(ANIMA_MAT_SINGLE);
						return;
					}
				}
			}
			matIdx+= matCount;
		}

	//Geometry stuff
		Int32 _matCount=0;
		for(Int32 i=0;i<geomList->Count();i++) 
			_matCount+=geomList->GetGeom(i)->GetMaterialNameCount();

		Int32 m_idx=0;
		for(Int32 i=0;i<geomList->Count();i++)
		{
			Anima::MeshExtGeomExt *ext = geomList->GetGeom(i);

			Anima::ModelExporterRender *modelExporterRender =NULL;
			Anima::ModelExporterParams *expPars = ext->GetExportParams();
			Anima::ModelExporter *modelExporter = expPars->GetExporter(Anima::C4D,12);
			if (modelExporter!= NULL)
					modelExporterRender = modelExporter->GetRender((Anima::ModelExporterRenderType)renderer,0);

			if (modelExporterRender!=NULL)
			{
				for(Int32 m = 0 ; m< ext->GetMaterialNameCount() ; m++)
				{
					String stName = ANIMA_MAT_PRE+LongToString(matIdx+m);
					Anima::RenderPresetInfo *qualityInfo =  Anima::Context::Singleton.GetRenderInfo()->CurrentPreset;

					QString partName=ext->GetMaterialName(m); 

					BaseMaterial* mat = NULL;
					Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource(partName);
					if (expMat!=NULL)
					{
						QString name = QString("%1_%2").arg(ext->GetName()).arg(partName);
								
						QString path = expMat->GetFullMeshExternalFilePath();

						QString matName=expMat->meshExternalName;
						int subIdx=-1;
						QStringList matNameList = matName.split(",");
						if (matNameList.count() == 2)
						{
							matName = matNameList[0];
							subIdx =  matNameList[1].toInt();
						}


						String modelExternalFilePath = "";
						String presetExternalFilePath = "";

						if (expMat!=NULL)
							presetExternalFilePath = Utils::toC4DString(expMat->GetFullMeshExternalFilePath());
						else
							Utils::addLog("expmat null");

						String modelMatPath = modelExternalFilePath;
						if (GeGetCurrentOS() == OPERATINGSYSTEM_WIN)
							modelMatPath = Utils::replaceAll(modelMatPath, '/', '\\');
						else
							modelMatPath = Utils::replaceAll(modelMatPath, '\\', '/');
						Filename modelTexpath = Filename(modelMatPath).GetDirectory() + "textures" + Utils::toC4DString(qualityInfo->Folder);

						String presetMatPath = presetExternalFilePath;
						if (GeGetCurrentOS() == OPERATINGSYSTEM_WIN)
							presetMatPath = Utils::replaceAll(presetMatPath, '/', '\\');
						else
							presetMatPath = Utils::replaceAll(presetMatPath, '\\', '/');
						Filename presetTexpath = Filename(presetMatPath).GetDirectory() + "textures" + Utils::toC4DString(qualityInfo->Folder);

						mat = loadMaterialFile(Filename(presetMatPath), Utils::toC4DString(matName), modelTexpath, presetTexpath, ""); 
						if(!mat)
						{
							mat = BaseMaterial::Alloc(Mmaterial);
							Utils::addLog("geom loadMaterialFile null");
						}
					}
					else
					{
						mat = BaseMaterial::Alloc(Mmaterial);
						Utils::addLog("geom expMat null");					
					}


					if(mat)
					{
						mat->SetName(stName);
						BaseMaterial* oldmat = doc->SearchMaterial(stName);
						if(oldmat)
						{
							oldmat->Remove();
							BaseMaterial::Free(oldmat);
						}
						doc->InsertMaterial(mat);

						mat->SetLayerObject(matLayer);
						mat->Message(MSG_CHANGE);

					}
				}
			}
			matIdx+= ext->GetMaterialNameCount();
		}

	}
}
BaseObject* AnimaObject::BuildAll(const Filename& f, BaseObject* op, Bool bEmbedded, Bool bForceFromFile, BaseDocument* doc, Bool resetScaleFactor)
{
	if((f.Content() && GeFExist(f)) || bForceFromFile || bEmbedded)
	{
		BaseObject* parent = BaseObject::Alloc(Onull);
		if(!parent) return NULL;
		parent->SetName("an(i)ma Object");
		Anima::_LogDebug(QString("Load start"),__FUNCTION__,__LINE__);
		QString path = Utils::toQString(f.GetString());
		Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("loading '%1'..").arg(path),4,true);
		Anima::ContextIO contextIO;
		Anima::Context::Singleton.GetGuiManager();
		Anima::Film &film = Anima::Context::Singleton.GetFilm();
		film.Clear();
		if(bEmbedded && !bForceFromFile)
			contextIO.Load(Utils::toQString(op->GetDataInstance()->GetString(ANIMAOBJECT_EMBEDDED_FILE)),path, FALSE, progressBar);
		else
		{
			String st="";
			AutoAlloc<BaseFile> bf;

			if(bf->Open(f))
			{
				VInt32 len = bf->GetLength();
				char* c = new char[len+1];
				c[len]=0;
				bf->ReadBytes(c, len);
				st = c;
				delete[] c;
			}

			op->GetDataInstance()->SetString(ANIMAOBJECT_EMBEDDED_FILE, st);
			if((f.Content() && GeFExist(f)))
				contextIO.Load(path, FALSE, progressBar);
		}
		if (Anima::Config::IsDemo())
			film.SetPlaybackBounds(true, film.GetStartPlaybackFrame(),film.GetFPS()*4);
	
		Real startTime = film.GetStartActiveTime();
		Real endTime = film.GetEndActiveTime();
		AnimaObject::g_endTime = endTime;
		if(doc)
		{
			doc->SetMinTime(BaseTime(startTime));
			doc->SetMaxTime(BaseTime(endTime));
			doc->SetLoopMinTime(BaseTime(startTime));
			doc->SetLoopMaxTime(BaseTime(endTime));
		}
//		GePrint(RealToString(startTime) + " to " + RealToString(endTime));
		Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(999999);
		Anima::Context::Singleton.GetPresetsExporterManager()->SetSingleMaterial(Anima::C4D,12,(Anima::ModelExporterRenderType)getCurrentRenderer(doc));
	
		BaseTime t;
		if(doc)
			t = doc->GetTime();
		film.SetPlayTime(t.Get());
		Anima::GfxContext::Singleton->UpdateAndContinue();
		Anima::MeshExporter* meshExporter = new Anima::MeshExporter();
		meshExporter->BuildCurrentTime();
		Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();
		Anima::GeomExtCollection &geomList = meshExporter->GetGeomExportColl();

		if(!(bEmbedded && !bForceFromFile))
			BuildMaterials(doc, &meshList, &geomList);

		Real scaleFactor = op->GetDataInstance()->GetFloat(ANIMAOBJECT_SCALE_FACTOR);
		if(resetScaleFactor || scaleFactor == 0.0)
		{
			scaleFactor = Utils::getUnitScaleFactor();
			op->GetDataInstance()->SetFloat(ANIMAOBJECT_SCALE_FACTOR,scaleFactor);
		}

//		GePrint(LongToString(meshList.InstanceCount()));
		foreach(Anima::FilmLayer *layer , film.GetFilmLayers())
		{
			foreach(Anima::Animation *anim , layer->GetAnimationList())
			{
				foreach(Anima::ModelInstance *mi , anim->GetModels())
				{
					Anima::MeshExtModel *ext = meshList.Get(mi);
					if (ext!=NULL)
					{
						BaseObject* obj = BaseObject::Alloc(ID_ANIMAMESHOBJECT);
						if(obj)
						{
							AnimaMeshObject* node = (AnimaMeshObject*) obj->GetNodeData();
							node->setScaleFactor(scaleFactor);
							node->buildModel(meshExporter, meshList.Get(mi), t, doc);
							obj->SetName(mi->GetName().toAscii().data());
							obj->InsertUnder(parent);
						}
						else
							return NULL;
					}
				}		
			}
		}		
//		GePrint("finished loading models");
		
		if(geomList.Count() > 0)
		{
			BaseObject* extra = BaseObject::Alloc(Onull);
			if(!extra) return NULL;
			extra->InsertUnder(parent);
			extra->SetName("Extra");
			for(Int32 i=0;i<geomList.Count();i++)
			{
				Anima::MeshExtGeomExt *ext = geomList.GetGeom(i);
			
				BaseObject* obj = BaseObject::Alloc(ID_ANIMAMESHOBJECT);
				if(obj)
				{
					AnimaMeshObject* node = (AnimaMeshObject*) obj->GetNodeData();
					node->setScaleFactor(scaleFactor);
					node->buildGeom(meshExporter, ext, t, doc);
					obj->SetName(ext->GetName().toAscii().data());
					obj->InsertUnder(extra);
				}
				else
					return NULL;
			}
		}
//		GePrint("finished loading geom");

		return parent;
	}
	else
		return NULL;
}
Bool AnimaObject::hasToShowBBox()
{
	BaseContainer* bc = ((BaseObject*)Get())->GetDataInstance();
	if(bc)
	{
		Int32 maxbbox = bc->GetInt32(ANIMAOBJECT_BBOX_MODE);
		if(++m_bboxCount > maxbbox)
			return TRUE;
	}
	return FALSE;
}
void AnimaObject::AddInfos(Int32 actors, Int32 shownActors, Int32 polys, Int32 shownPolys, Int32 verts, Int32 shownVerts)
{
	if(m_clearInfos)
	{
		m_clearInfos=FALSE;
		m_infos = AnimaInfos();
	}
	m_infos.actors+=actors;
	m_infos.shownActors+=shownActors;
	m_infos.polys+=polys;
	m_infos.shownPolys += shownPolys;
	m_infos.verts += verts;
	m_infos.shownVerts += shownVerts;
}

void AnimaObject::RebuildObject(BaseObject *op, BaseDocument* doc, Bool bForceFromFile, Bool resetScaleFactor)
{
	Bool bEmbedded = op->GetDataInstance()->GetBool(ANIMAOBJECT_EMBEDDED);
		
	Filename f = op->GetDataInstance()->GetFilename(ANIMAOBJECT_PATH);
	if((f.Content() && GeFExist(f)) || bForceFromFile || bEmbedded)
	{
		StopAllThreads();
		m_lastFilename = f;
		Utils::addLog("AnimaObject:: Rebuild Object " + f.GetString());
		BaseObject::Free(m_obj);
		m_obj = BuildAll(f, op, bEmbedded, bForceFromFile, doc, resetScaleFactor);
		Utils::adjustAllMaterialsDarkness(doc, op->GetDataInstance()->GetFloat(ANIMAOBJECT_DARKNESS));
		m_bCached = FALSE;
		if(doc)
			m_lastTime = doc->GetTime();
	}
	else
		Utils::addLog("AnimaObject:: File not found: " + f.GetString());
}
void UpdateTimeRecursive(BaseObject* obj, BaseTime time)
{
	if(!obj) return;
	if(obj->GetType() == ID_ANIMAMESHOBJECT)
	{
		AnimaMeshObject* mesh = (AnimaMeshObject*) obj->GetNodeData();
		if(mesh)
		{
			mesh->UpdateMesh(time);
		}
	}
	BaseObject* child = obj->GetDown();
	while(child)
	{
		UpdateTimeRecursive(child, time);
		child=child->GetNext();
	}
}
BaseObject* AnimaObject::GetVirtualObjects(BaseObject *op, HierarchyHelp *hh)
{
//	Utils::addLog("AnimaObject::getvirtual");
	if(!hh->GetDocument())
		return BaseObject::Alloc(Onull);
	if(Utils::findFirstObjectOfType(hh->GetDocument()->GetFirstObject(), ID_ANIMAOBJECT) != op)
		return BaseObject::Alloc(Onull);

	m_bboxCount = 0;
	m_clearInfos = TRUE;
	if(m_bCached)
	{
//		Utils::addLog("AnimaObject::cached");
		if(hh->GetDocument()->GetTime() != m_lastTime)
		{
//			Utils::addLog("AnimaObject::newtime");
			Anima::Film &film = Anima::Context::Singleton.GetFilm();
			film.SetPlayTime(hh->GetDocument()->GetTime().Get());
			Anima::GfxContext::Singleton->UpdateAndContinue();

			m_lastTime = hh->GetDocument()->GetTime();
		}		
		BaseObject* cache = op->GetCache(hh);
		if(cache)
		{
//			Utils::addLog("AnimaObject::return cache");
			return cache;
		}
		else
		{
			//sometimes the cache is empty even though there should be all the meshes inside (perhaps because of memory restrictions)
			//so copy the original objects again
			UpdateTimeRecursive(m_obj, m_lastTime);
			Utils::addLog("AnimaObject::UpdateTimeRecursive");
			return (BaseObject*)m_obj->GetClone(COPYFLAGS_0, NULL);
		}
	}
	if(m_obj)
	{
//		Utils::addLog("AnimaObject::notcached");
		m_bCached = TRUE;
		return (BaseObject*)m_obj->GetClone(COPYFLAGS_0, NULL);
	}
	return BaseObject::Alloc(Onull);
}

Bool AnimaObject::RegisterAnimaObject(void)
{
	return RegisterObjectPlugin(ID_ANIMAOBJECT,GeLoadString(IDS_ANIMAOBJECT),OBJECT_GENERATOR,AnimaObject::Alloc,"Oanima",AutoBitmap("anima.tif"),0);
}
