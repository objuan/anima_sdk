#include "stdafx.h"
#include "resource.h"

//#include "IViewportManager.h"
//#include "IHardwareRenderer.h"
#include <icurvctl.h> 
#include <bitmap.h>
#include <gamma.h>

#include "MeshObj.h"
#include "MAXAnima.h"

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
#include "Export/PresetsExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"
//
//enum ModelExporterRenderType
//{
//	DEFAULT = 0,
//	VRAY = 1,
//	MENTAL_RAY = 2
//};

QString map[] = {"Default Scanline Renderer","STANDARD","mental ray Renderer","MENTAL RAY","vray Renderer","VRAY","vray","VRAY","v-ray","VRAY","iray Renderer","IRAY","Nvidia Mental Ray","NVIDIA MENTAL RAY"};

static Class_ID bakeShellClassID(BAKE_SHELL_CLASS_ID,0);
bool sendMaterialEvent=true;
Anima::ModelExporterRenderType GetCurrentRender(Interface *globalInt)
{
	sendMaterialEvent=false;
	Renderer *ren = globalInt->GetProductionRenderer ();
	Class_ID renClass = ren->ClassID();
	MSTR s;
	ren->GetClassName (s);
	QString ms = _MQ(s).toUpper();
	Anima::_LogDebug(QString("Render: '%1'").arg(ms),__FUNCTION__,__LINE__);

	/*for(int i=0;i<14;i+=2)
	{
		if (ms == map[i] || ms.startsWith(map[i], Qt::CaseInsensitive)) 
		{
			sendMaterialEvent=true;
			return map[i+1];
		}
	}*/
	sendMaterialEvent=true;
	return ms;
//#ifdef MAX2014
//	return "NVIDIA MENTAL RAY";
//#else
//	return map[1];
//#endif
}

void MeshObject::CheckMaterial()
{
	//if (objMaterial== NULL || !sendMaterialEvent) return;
	if (modelMaterialList.count() == 0 || !sendMaterialEvent) return;

 	Anima::ModelExporterRenderType new_current_render_type = GetCurrentRender(  GetCOREInterface());

	if (current_render_type != new_current_render_type)
	{
		current_render_type = new_current_render_type;
		BuildMaterial();
		UpdateMaterialValues();
	}
}

// ===========================

Texmap* MakeTex(const QString &name) {
	Texmap *txm; 

	BitmapTex *bmt = NewDefaultBitmapTex();
	//bmt->SetMapName
	bmt->SetMapName(_QM(name));


//	bmt->SetAlphaAsMono((par.texflags&TEX_ALPHA_SOURCE)?1:0);
//	bmt->SetAlphaSource((par.texflags&TEX_DONT_USE_ALPHA)?ALPHA_NONE:ALPHA_FILE);
//	bmt->SetFilterType((par.texflags&TEX_SAT)?FILTER_SAT:FILTER_PYR);
		
//	StdUVGen *uv = bmt->GetUVGen();
	//uv->SetUOffs(par.uoffset,0);
	//uv->SetVOffs(-par.voffset,0);
	//uv->SetUScl(par.uscale,0);
	//uv->SetVScl(par.vscale,0);
//	uv->SetAng(-((float)atan2(par.ang_sin, par.ang_cos)),0);
	//uv->SetBlur(par.texblur+1.0f,0);
		
	//uv->SetTextureTiling(tile);
		
	TextureOutput *txout = bmt->GetTexout();
	//txout->SetInvert(par.texflags&TEX_INVERT?1:0);
	txm = bmt;

	//	if (map.p.tex.texflags&TEX_TINT) {
	//		// map.p.tex.col1, col2	: stuff into Mix	
	//		MultiTex* mix = NewDefaultMixTex();
	//		mix->SetColor(0,ColorFrom24(map.p.tex.col1));
	//		mix->SetColor(1,ColorFrom24(map.p.tex.col2));
	//		mix->SetSubTexmap(2, txm);
	//		txm = mix;			
	//		}
	//	else if (map.p.tex.texflags&TEX_RGB_TINT) {
	//		// map.p.tex.rcol,gcol,bcol : stuf into tint
	//		}
	//	}
	//else {  
	//	// kind == 1 :  Reflection Map
	//	BitmapTex *bmt = NewDefaultBitmapTex();
	//	bmt->SetMapName(TSTR(map.name));
	//	StdUVGen *uv = bmt->GetUVGen();

	//	// TBD: REFLECTION BLUR SETTING:
	//	uv->SetBlurOffs((float)smtl->refblur/400.0f+.001f,0);
	//	bmt->InitSlotType(MAPSLOT_ENVIRON);
	//	txm = bmt;
	//	}


	return txm;
}

#define GNORMAL_CLASS_ID	Class_ID(0x243e22c6, 0x63f6a014)

#ifndef MAX2012 
#ifndef MAX2013
#ifndef MAX2014
	enum { gnormal_params };
		enum { 
			gn_mult_spin, gn_bmult_spin,
			gn_map_normal, gn_map_bump,
			gn_map1on, gn_map2on,
			gn_method,
			gn_flip_red, gn_flip_green, gn_swap_rg,
		};
#endif
#endif
		#endif
StdMat *New3DSDefaultMtl(MSTR &name,const char *txtPath,const QString &normal_txtPath,int matIdx,Anima::MeshExtSubmeshInfo *subInfo) {
	StdMat *m = NewDefaultStdMat();
	m->SetName(name);

	float val = 150.0f / 255.0f;
	m->SetAmbient(Color(val,val,val),0);
	m->SetDiffuse(Color(val,val,val),0);

	// txt di diffuse
	Texmap* txm = MakeTex(txtPath);
	m->SetSubTexmap(ID_DI,txm);
	float amt = 1;
	m->SetTexmapAmt(ID_DI, amt, 0);
	m->EnableMap(ID_DI,true);

	// txt normal
	if (normal_txtPath!="")
	{
		Texmap *n_map  = (Texmap*)CreateInstance(TEXMAP_CLASS_ID, GNORMAL_CLASS_ID);
		m->SetSubTexmap(ID_BU,n_map);
		float amt = 30.0f / 100.0f;
		m->SetTexmapAmt(ID_BU, amt, 0);
		m->EnableMap(ID_BU,true);
		// 
		Texmap* txm = MakeTex(normal_txtPath.toAscii().constData());
		n_map->SetSubTexmap(0,txm);
		//IParamBlock2 *parm = n_map->GetParamBlock(0);

		//float gnMult=2;
		////parm->GetValue( gn_mult_spin, 0, gnMult, ivalid);
		//parm->SetValue(gn_mult_spin,0,gnMult);
	}

	if (subInfo!=NULL)
	{
		if (subInfo->IsHead())
		{
			m->SetSpecular(Color(1.0f,1.0f,1.0f),0);
			m->SetShininess(40.0f/100.0f,0); // specular
			m->SetShinStr(20.f/100.0f,0); // glossines
		}
		else if (subInfo->IsBody())
		{
			m->SetSpecular(Color(1.0f,1.0f,1.0f),0);
			m->SetShininess(0,0);// specular
			m->SetShinStr(0,0); // glossines
		}
	}
	else
	{
		m->SetSpecular(Color(1.0f,1.0f,1.0f),0);
		m->SetShininess(0,0);// specular
		m->SetShinStr(0,0); // glossines
	}

	m->SetShading(SHADE_PHONG);
	m->SetSoften (TRUE);

	m->SetFaceMap(FALSE);
	return m;
}


void ChangePath(ISubMap *m,const QString &modelName,const QString &rootRenderPath,const QString &modelRenderPath, const QString &bmpQuality )
{
	for(int s=0;s<m->NumSubTexmaps ();s++)
	{
		Texmap *txt = m->GetSubTexmap(s);
		if (txt!=NULL)
		{
			MSTR sname;
			txt->GetClassName (sname);

			if (sname ==_M("Bitmap"))
			{
				BitmapTex *btm = (BitmapTex*) txt;

				//const char *path= _MQ(btm->GetMapName());
				// controllo se ce'
				QString filePath = _MQ(btm->GetMapName());//QString("%1").arg(path);
				if (!QFile(filePath).exists())
				{
					QString fileName = QFileInfo(filePath).fileName();
					if (modelName=="")
						filePath = QString("%1/textures/%2/%3").arg(modelRenderPath).arg(bmpQuality).arg(fileName);
					else
					{
						int idx = fileName.lastIndexOf("-");
						if (idx!=-1)
						{
							idx = fileName.lastIndexOf("-",idx-1);
							if (idx!=-1)
							{
								QString suffix = fileName.mid(idx);
								// primo tentativo da rootRenderPath poi da modelRenderPath
								filePath = QString("%1/textures/%2/%3%4").arg(modelRenderPath).arg(bmpQuality).arg(modelName).arg(suffix);
								if (!QFile(filePath).exists())
								{
									filePath = QString("%1/textures/%2/%3%4").arg(rootRenderPath).arg(bmpQuality).arg(modelName).arg(suffix);
								}
							}
							else
								filePath = QString("%1/textures/%2/%3").arg(rootRenderPath).arg(bmpQuality).arg(fileName);
						}
						else 
							filePath = QString("%1/textures/%2/%3").arg(rootRenderPath).arg(bmpQuality).arg(fileName);
					}

					if (QFile(filePath).exists())
					{
						btm->SetMapName(_QM(filePath));
					}
				}
			}
			ChangePath(txt,modelName,rootRenderPath,modelRenderPath, bmpQuality);
		}
	}
}

void ChangeRgb(Anima::ModelInstance *mi,Anima::ModelExporterRenderMaterial *mat,ISubMap *m,float newValue,int maskPos=-1 )
{
	int num = m->NumSubTexmaps ();
	for(int s=0;s<num;s++)
	{
		Texmap *txt = m->GetSubTexmap(s);
		if (txt!=NULL)
		{
			MSTR sname;
			MSTR slot_name;
			txt->GetClassName (sname);
			slot_name=_M("");
			slot_name = txt->GetSubTexmapSlotName(s);

			//if (slot_name == MSTR("Diffuse") )
			{
				//if (sname ==MSTR("Bitmap") && 
				//	(mat->meshEditableChannels.count() == 0 || mat->meshEditableChannels.contains(s)) )
				if (sname ==_M("Composite"))
				{
					MultiTex *multi = (MultiTex *)dynamic_cast<MultiTex *>(txt);
					//int num = multi->NumSubTexmaps ();

					//for(int i=0;i<num;i++)
					{
						ChangeRgb(mi,mat,multi,newValue,0);
						maskPos=-1;
						//Texmap *txt =  multi->GetSubTexmap (i) ;
						//if (sname ==MSTR("Bitmap"))
						//{
						//	BitmapTex *btm = (BitmapTex*) txt;

						//	TextureOutput *out = btm->GetTexout();
						//}
					}
				}
				else if (sname ==_M("VRayCompTex"))
				{
					ChangeRgb(mi,mat,txt,newValue,0);
					maskPos=-1;
				}
				else if (sname ==_M("Normal Bump"))
				{
					ChangeRgb(mi,mat,txt,newValue,0);
				}
				else if (sname ==_M("Mask"))
				{
					// Texmap
					Texmap *multi = (Texmap *)dynamic_cast<Texmap *>(txt);
					//int num = multi->NumSubTexmaps ();
					//for(int i=0;i<num;i++)
					{
						ChangeRgb(mi,mat,multi,newValue,maskPos++);
					}
				}
				else if (sname ==_M("Bitmap"))
				{
					BitmapTex *btm = (BitmapTex*) txt;

					FPInterfaceDesc  *desc = btm->GetDesc();

					slot_name = btm->GetMapName();

					const MCHAR *path= btm->GetMapName();
					QString filePath = QString("%1").arg(_MQ(path));
					if (filePath!="")
					{
						QString fn = QFileInfo(filePath).baseName().toUpper();
						if (fn.endsWith("-D") || fn.endsWith("-S"))
						{
							TextureOutput *out = btm->GetTexout();
							out->GetClassName (sname);
							{
								StdTexoutGen *gen =(StdTexoutGen  *)out;
								//gen->SetInvert(true);
								//gen->SetClamp(true);
								float rgbVal = newValue;
								if(fn.endsWith("-S") && gammaMgr.IsEnabled() && gammaMgr.GetFileInGamma() < 2.21f && gammaMgr.GetFileInGamma() > 1.79f)
									rgbVal*=2.2;
								gen->SetRGBAmt(rgbVal,TIME_NegInfinity);
								gen->SetRGBAmt(rgbVal,TIME_PosInfinity);

								ULONG f;
								BOOL map1 =   gen->GetFlag (TEXOUT_COLOR_MAP);
								BOOL map2 =   gen->GetFlag (TEXOUT_COLOR_MAP_RGB);
								if (map1 && map2)
								{
									if (map1 == 1 && maskPos!=-1) 
									{
										// scalo il colore 
										QColor color = mat->GetColor(mi,maskPos);// QColor::fromRgb(255,125,0);
										float factor[3];
										factor[0] = (float)color.red() / 255.0f; // 0 -1 
										factor[1] = (float)color.green() / 255.0f; // 0 -1 
										factor[2] = (float)color.blue() / 255.0f; // 0 -1 

										ICurveCtl* ccRGB_1 = (ICurveCtl*)gen->SubAnim(1); //reference to RGB curve control
										for (int i=0; i<ccRGB_1->GetNumCurves(); i++) 
										{                         
										   ICurve* cRGB = ccRGB_1->GetControlCurve(i);
	
										   int np = cRGB->GetNumPts();
										   for(int p=1;p<np;p++)
										   {
												TimeValue s(SecToTicks (0));
											//	TimeValue e(SecToTicks (1));
												CurvePoint cp1 = cRGB->GetPoint(s,p);
												//CurvePoint cp2 = cRGB->GetPoint(e,p);

												CurvePoint *np = new CurvePoint(cp1);

												np->p.y = factor[i];

												cRGB->SetPoint(s,p,np);
												//cRGB->SetPoint(e,p,&cp2);
												int y=0;
										   }
										}
									}
									else
									{
									   ICurveCtl* ccMono_1 = (ICurveCtl*)gen->SubAnim(2); //reference to Mono curve control
									}
								}

							//	gen->SetFlag(TEXOUT_COLOR_MAP,1);
								//gen->SetFlag(TEXOUT_COLOR_MAP_RGB,1);
								//gen->SetOutputLevel(TIME_NegInfinity,0.5f);
								//gen->SetOutputLevel(TIME_PosInfinity,0.5f);
							}
						}
						else if (fn.endsWith("-N") || fn.endsWith("-M1") || fn.endsWith("-M2") || fn.endsWith("-A"))
						{
							if(gammaMgr.IsEnabled() && gammaMgr.GetFileInGamma() < 2.21f && gammaMgr.GetFileInGamma() > 1.79f)
							{
								TextureOutput *out = btm->GetTexout();
								StdTexoutGen *gen =(StdTexoutGen  *)out;
								gen->SetRGBAmt(2.2,TIME_NegInfinity);
								gen->SetRGBAmt(2.2,TIME_PosInfinity);
							}
						}
					}
				}
				else
					ChangeRgb(mi,mat,txt,newValue,0);
			}
			//ChangeRgb(mi,mat,txt,newValue,maskPos);
		}
	}
}

Mtl *LoadMaterial(int *base_m_idx,MultiMtl *objMaterial,Anima::MeshExt *meshExt,Anima::ModelExporter *modelExporter,Anima::ModelExporterRender *modelExporterRender,const QString &materialName,int materialNameIdx)
{
	Interface *globalInt =  GetCOREInterface();
	Anima::RenderPresetInfo *qualityInfo =  Anima::Context::Singleton.GetRenderInfo()->CurrentPreset;

	int matIdx = *base_m_idx;
	Mtl *ret_mat=NULL;
	QString partName=materialName;
	//Anima::MeshExtSubmeshInfo &subInfo = meshExt->GetSubInfo(materialNameIdx);
	//partName = subInfo.name; 

	Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource(materialName);
	if (expMat!=NULL)
	{
		QString name = QString("%1_%2").arg(meshExt->GetName()).arg(partName);
				
		QString path = expMat->GetFullMeshExternalFilePath();
		int ret = globalInt->LoadMaterialLib(_QM(path));
		MtlBaseLib &base_lib= globalInt->GetMaterialLibrary();

		QString matName=expMat->meshExternalName;
		int subIdx=-1;
		QStringList matNameList = matName.split(",");
		if (matNameList.count() == 2)
		{
			matName = matNameList[0];
			subIdx =  matNameList[1].toInt();
		}

		int idx = base_lib.FindMtlByName(MSTR(_QM(matName))); 
		if (idx != -1)
		{
			Mtl *mat = (Mtl *)(base_lib)[idx];
			if (subIdx == -1)
			{
				ChangePath(mat,"",modelExporter->GetRenderPath(),modelExporter->GetRenderPath(),qualityInfo->Folder);

				ret_mat= mat;
				objMaterial->SetSubMtl(matIdx,mat);
			}
			else
			{
				MultiMtl *multi = NULL;
				if(mat->ClassID() == bakeShellClassID)
					multi = (MultiMtl *)mat->GetSubMtl(0);
				else
					multi = (MultiMtl *)mat;
				int i = multi->NumSubMtls ();
				Mtl *m = multi->GetSubMtl(subIdx);
				///MSTR s1 = multi->GetSubMtlSlotName(subIdx);

				// aggiusto le textures
				ChangePath(m,"",modelExporter->GetRenderPath(),modelExporter->GetRenderPath(),qualityInfo->Folder);

				//loadMtls.AddMtl(m);			
				objMaterial->SetSubMtl(matIdx,m);
				ret_mat = m;
			}
		}
		else
		{
			//// metto un materiale vuoto
			QString name = QString("%1_%2").arg(meshExt->GetName()).arg(partName);

			StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",-1,NULL);
			//loadMtls.AddMtl(m);			
			ret_mat = m;

			int val=200;
			if (matIdx == 1)
			{
				m->SetAmbient(Color(val,0,0),0);
				m->SetDiffuse(Color(val,0,0),0);
			}
			else  if (matIdx == 2)
			{
				m->SetAmbient(Color(0,val,0),0);
				m->SetDiffuse(Color(0,val,0),0);
			}
			else  if (matIdx == 3)
			{
				m->SetAmbient(Color(0,val,val),0);
				m->SetDiffuse(Color(0,val,val),0);
			}	
			else  if (matIdx == 4)
			{
				m->SetAmbient(Color(0,0,val),0);
				m->SetDiffuse(Color(0,0,val),0);
			}
			else  if (matIdx == 5)
			{
				m->SetAmbient(Color(val,0,val),0);
				m->SetDiffuse(Color(val,0,val),0);
			}

			objMaterial->SetSubMtl(matIdx,m);
		}

		//base_lib.DeleteAll ();
	}
	else
	{
		//// metto un materiale vuoto
		QString name = QString("%1_%2").arg(meshExt->GetName()).arg(partName);

		StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",-1,NULL);
		//loadMtls.AddMtl(m);			
		ret_mat = m;
		
		objMaterial->SetSubMtl(matIdx,m);
	}
	*base_m_idx  = *base_m_idx +1;
	return ret_mat;
}

int GetMaxVersion()
{
	int maxVersion=9;

	//DWORD ver = Get3DSMAXVersion();

	//DWORD rel = GET_MAX_RELEASE(Get3DSMAXVersion());

	//if (rel == 9000) maxVersion  = 9;

#ifdef MAX2008
	maxVersion = 2008;
#endif
#ifdef MAX2009
	maxVersion = 2009;
#endif
#ifdef MAX2010
	maxVersion = 2010;
#endif
#ifdef MAX2011
	maxVersion = 2011;
#endif
#ifdef MAX2012
	maxVersion = 2012;
#endif
#ifdef MAX2013
	maxVersion = 2013;
#endif
#ifdef MAX2014
	maxVersion = 2014;
#endif
	return maxVersion;
}

void MeshObject::ClearAnimaMaterial(){

	Interface *globalInt =  GetCOREInterface();
	// clear old materials
	MtlBaseLib *lib = globalInt->GetSceneMtls();

	int number=0;
	int idx = -1;
	do
	{
		QString name = QString("%1").arg("an(i)maMaterial");
		if (number>0)
			name = QString("%1 %2").arg("an(i)maMaterial").arg(number+1);

		idx = lib->FindMtlByName(MSTR(_QM(name))); // TODO BETTER
		if (idx != -1)
			lib->RemoveEntry (idx); // SI PIANTAVA SENZA DELETE NEL 2009
		number++;
	}
	while(idx != -1);
}

void MeshObject::BuildMaterial()
{
	//MessageBox(NULL, _T("BuildMaterial"), _T("WARNING!"), MB_OK);

//	if (globalInt ==NULL) return; // uso quello già presente
//	Anima::_LogDebug(QString("BuildMaterial start"),__FUNCTION__,__LINE__);

	Anima::MeshExporter &_meshExporter = *MeshObject::meshObject->meshExporter;
	//_meshExporter.BuildCurrentFrame(0);

	// NEW , Get quality Info

	Anima::RenderPresetInfo *qualityInfo =  Anima::Context::Singleton.GetRenderInfo()->CurrentPreset;

	IBitmapProxyManager* bitmapProxy = GetBitmapProxyManager();
	if(bitmapProxy)
	{
		if(qualityInfo->Mode == "Proxies")
		{
			bitmapProxy->SetGlobalProxyEnable(TRUE);
		}

		if(qualityInfo->Downscale == "Eighth")
			bitmapProxy->SetGlobalProxySizeFactor(8);
		else if(qualityInfo->Downscale == "Quarter")
			bitmapProxy->SetGlobalProxySizeFactor(4);
		else if(qualityInfo->Downscale == "Half")
			bitmapProxy->SetGlobalProxySizeFactor(2);
		else if(qualityInfo->Downscale == "Full")
			bitmapProxy->SetGlobalProxySizeFactor(1);
	}
	// materials && textures

//	MtlBaseLib &lib = Interface::GetMaterialLibrary()  
	MtlList loadMtls;
	//Tab<QString *> loadMtlMesh;

	//MtlBaseLib* GetSceneMtls()

	Interface *globalInt =  GetCOREInterface();

	Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	
	// compute materials number

	// clear old materials

	MtlBaseLib *lib = globalInt->GetSceneMtls();

	ClearAnimaMaterial();
	//int idx = lib->FindMtlByName(MSTR("an(i)maMaterial")); // TODO BETTER
	//if (idx != -1)
	//	lib->RemoveEntry (idx); // SI PIANTAVA SENZA DELETE NEL 2009

	////if (idx == -1)
	//{
	//	objMaterial = NewDefaultMultiMtl();
	//	QString name = QString("%1").arg("an(i)maMaterial");
	//	objMaterial->SetName(name.toAscii().constData());
	//	lib->AddMtl(objMaterial);
	//}
	///*else
	//{
	//	objMaterial = (MultiMtl *)(*lib)[idx];
	//	objMaterial->SetNumSubMtls(0);
	//}*/

	// create materials ..

	if (current_render_type == "") current_render_type = GetCurrentRender(  GetCOREInterface());

	if (!Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial())
	{
		int matScanIdx=-1;
		int subIdx=0;
		MultiMtl *mtl=NULL;
		Mtl *bakeshell=NULL;
		QList<MeshObjectModelMaterial> &materialList = this->GetModelMaterialList();
		for(int i=0;i<materialList.count();i++)
		{
			if (materialList[i].materialNumber != matScanIdx)
			{
				if (mtl!=NULL && subIdx!=0) mtl->SetNumSubMtls(subIdx);
				subIdx=0;

				mtl = NewDefaultMultiMtl();
				QString name = QString("%1").arg("an(i)maMaterial");
				if (materialList[i].materialNumber>0)
					name = QString("%1 %2").arg("an(i)maMaterial").arg(materialList[i].materialNumber+1);
				mtl->SetName(MSTR(_QM(name)));

				matScanIdx= materialList[i].materialNumber;


				if(Anima::Context::Singleton.GetPresetsExporterManager()->GetViewportMode() != Anima::PresetsExporter_ViewportMode_TEXTURED)
				{
					bakeshell = (Mtl*)CreateInstance(MATERIAL_CLASS_ID, bakeShellClassID);
					bakeshell->SetName(_M("an(i)maMaterial"));
					bakeshell->SetSubMtl(0, mtl);
					IParamBlock2* pb = bakeshell->GetParamBlock(0);
					pb->SetValue(0, 0, 1);//set viewport to other material
					pb->SetValue(1, 0, 0);//set render to anima mat
					lib->AddMtl(bakeshell);
				}
				else
					lib->AddMtl(mtl);
			}
			if(bakeshell)
				materialList[i].objMaterial=bakeshell;
			else
				materialList[i].objMaterial=mtl;

			//materialList[i].materialSubIdx = subIdx;
			subIdx+=  meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();
		}
		if (mtl!=NULL && subIdx!=0) mtl->SetNumSubMtls(subIdx);

		//int matCount=0;
		//// multimaterial
		//for(int i=0;i<meshList.InstanceCount();i++) 
		//	matCount+= meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();

		//objMaterial->SetNumSubMtls(matCount);
		//int matIdx=0;
		int baseMatIdx=0;
		int modelIdx=0;
		MultiMtl *objMaterial=NULL;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			modelIdx=i;
			
			if(modelMaterialList[i].objMaterial->ClassID() == bakeShellClassID)
				objMaterial = (MultiMtl*)materialList[modelIdx].objMaterial->GetSubMtl(0);
			else
				objMaterial = (MultiMtl*)materialList[modelIdx].objMaterial;

		//	Anima::_LogDebug(QString("model %1 %2").arg(materialList[i].materialNumber).arg(materialList[i].materialSubIdx),__FUNCTION__,__LINE__);


			GetCOREInterface()->ProgressUpdate((float)i*100.0/(float)meshList.InstanceCount(),false,_M("Building Materials"));

			Anima::ModelExporterRender *modelExporterRender =NULL;
			Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
		
			//Ogre::MeshPtr mesh = meshExt->GetOgreMesh();

			Anima::ModelExporter *_modelExporter = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());
			// 
			//Anima::ModelExporter *_modelExporter = meshExt->GetModel()->GetModel()->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());

			//Anima::ModelExporterParams *expPars = meshExt->GetModel()->GetModel()->GetExportParams();
		//	Anima::ModelExporter *_modelExporter = expPars->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());

			Anima::LogDebug(QString("BuildMaterial current_render_type=%1").arg(current_render_type));

			if (_modelExporter!= NULL)
				modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);

			//int matCount = meshExt->GetSubInfoCount();
			int matCount = meshExt->getNumSubMeshes();
			//Anima::_LogDebug(QString("matCount %1").arg(matCount),__FUNCTION__,__LINE__);

			for(int t=0;t<matCount;t++)
			{
				Anima::MeshExtSubmeshInfo &subInfo = meshExt->GetSubInfo(t);
				QString partName = subInfo.name; //QString("%1").arg((t == 1) ? "Head" : " Body");
				//Ogre::SubMesh *subMesh = mesh->getSubMesh(t);
			
				// ho i dati del materiale di ogre

				if (modelExporterRender!=NULL && qualityInfo->Folder !="")
				{
					Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource(meshExt->getMaterialName(t));
					if (expMat!=NULL)
					{
						QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(partName);
						
						{
							QString path = expMat->GetFullMeshExternalFilePath();
							int ret = globalInt->LoadMaterialLib(MSTR(_QM(path)));
							MtlBaseLib &base_lib= globalInt->GetMaterialLibrary();

							QString matName=expMat->meshExternalName;
							int subIdx=-1;
							QStringList matNameList = matName.split(",");
							if (matNameList.count() == 2)
							{
								matName = matNameList[0];
								subIdx =  matNameList[1].toInt();
							}

							int idx = base_lib.FindMtlByName(MSTR(_QM(matName))); 
							if (idx == -1)
							{
								if (base_lib.NumSubs () == 1)
									idx = 0;
							}
							if (idx != -1)
							{
								Mtl *mat = (Mtl *)(base_lib)[idx];
								if (subIdx == -1)
								{
									//objMaterial->SetSubMtl(matIdx+t,mat);
									objMaterial->SetSubMtl(materialList[modelIdx].materialSubIdx+t,mat);
								}
								else
								{
									MultiMtl *multi = NULL;
									if(mat->ClassID() == bakeShellClassID)
										multi = (MultiMtl *)mat->GetSubMtl(0);
									else
										multi = (MultiMtl *)mat;

									int i = multi->NumSubMtls ();
									Mtl *m = multi->GetSubMtl(subIdx);
									MSTR submatName;
									multi->GetSubMtlName(subIdx, submatName);

									// aggiusto le textures
									ChangePath(m,meshExt->GetModel()->GetModel()->GetName(),
										expMat->materialFolderPath, //_modelExporter->GetRenderPath(),
										Anima::Context::Singleton.GetModelLibrary().GetModelRenderDir(meshExt->GetModel()->GetModel()), qualityInfo->Folder);
									//ChangePath(m,_modelExporter->GetRenderPath(), qualityInfo->Folder);

									loadMtls.AddMtl(m);	

									//Anima::_LogDebug(QString("SetSubMtlAndName %1 %2 %3").arg(materialList[modelIdx].materialSubIdx).arg(t).arg((char *)submatName),__FUNCTION__,__LINE__);

									objMaterial->SetSubMtlAndName(materialList[modelIdx].materialSubIdx+t,m, submatName);
								}
							}
							else
							{
								// metto un materiale vuoto
								QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(partName);

								StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",t,&subInfo);
								loadMtls.AddMtl(m);			

								objMaterial->SetSubMtl(materialList[modelIdx].materialSubIdx+t,m);
							}
							base_lib.DeleteAll ();
						}
					}
					else
					{
						//// metto un materiale vuoto
						QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(partName);

						StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",t,&subInfo);
						loadMtls.AddMtl(m);			

						objMaterial->SetSubMtl(materialList[modelIdx].materialSubIdx+t,m);
					}
				}
				else
				{
					// metto un materiale vuoto
					QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(partName);

					StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",t,&subInfo);
					loadMtls.AddMtl(m);			

					objMaterial->SetSubMtl(materialList[modelIdx].materialSubIdx+t,m);
				}

			//	loadMtlMesh.Append(1,&name,10);

			//	LWImageID  imageID =  imglist->load(imagePath.toAscii().constData());
			//	int fsize = QFile(defPath).size();
			//	size = LoadSurf(defPath.toAscii().constData(),1,mapPath, data,fsize );
			//	local->surface( local->data, name.toAscii().constData(), NULL, size, data );
			}
			//matIdx+= matCount;
		}
	}
	else
	{
		MultiMtl *objMaterial = NewDefaultMultiMtl();
		QString name = QString("%1").arg("an(i)maMaterial");
		objMaterial->SetName(MSTR(_QM(name)));
		lib->AddMtl(objMaterial);

		QList<MeshObjectModelMaterial> &materialList = this->GetModelMaterialList();
		for(int i=0;i<materialList.count();i++)
		{
			materialList[i].objMaterial=objMaterial;
		}

		objMaterial->SetNumSubMtls(1);

		// single material
		Anima::ModelExporter *_modelExporter = Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());
		if (_modelExporter!= NULL)
		{
			Anima::ModelExporterRender *modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);
			Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource("All");
			if (expMat!=NULL)
			{
				QString name =  QString("Anima_%1").arg(Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->Name);

				QString path = expMat->GetFullMeshExternalFilePath();
				int ret = globalInt->LoadMaterialLib(MSTR(_QM(path)));
				MtlBaseLib &base_lib= globalInt->GetMaterialLibrary();

				QString matName=expMat->meshExternalName;
				int subIdx=-1;
				QStringList matNameList = matName.split(",");
				if (matNameList.count() == 2)
				{
					matName = matNameList[0];
					subIdx =  matNameList[1].toInt();
				}

				int idx = base_lib.FindMtlByName(MSTR(_QM(matName))); 
				if (idx == -1)
				{
					if (base_lib.NumSubs () == 1)
						idx = 0;
				}
				if (idx != -1)
				{
					Mtl *mat = (Mtl *)(base_lib)[idx];
					if (subIdx == -1)
					{
						objMaterial->SetSubMtl(0,mat);
					}
					else
					{
						MultiMtl *multi = (MultiMtl *)mat;

						int i = multi->NumSubMtls ();
						Mtl *m = multi->GetSubMtl(subIdx);
						MSTR submatName;
						multi->GetSubMtlName(subIdx, submatName);

						// aggiusto le textures
						ChangePath(m,"",_modelExporter->GetRenderPath(),_modelExporter->GetRenderPath(), "");

						loadMtls.AddMtl(m);			
						objMaterial->SetSubMtlAndName(0,m, submatName);
					}
				}
				else
				{
					// metto un materiale vuoto
				//	QString name = QString("%1_%2").arg(meshExt->GetModel()->GetModel()->GetName()).arg(partName);
					QString name =  QString("Anima_%1").arg(Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->Name);

					StdMat *m = New3DSDefaultMtl(MSTR(_QM(name)),"","",0,NULL);
					loadMtls.AddMtl(m);			
					objMaterial->SetSubMtl(0,m);
				}
				base_lib.DeleteAll ();
			}
		}
	}

	// ========================================================
	// ================== geometrie fisse ===================

	int idx = lib->FindMtlByName(MSTR(_M("an(i)maMaterialExtra)"))); // TODO BETTER
	if (idx != -1)
	{
		lib->RemoveEntry (idx); // SI PIANTAVA SENZA DELETE NEL 2009
	}
	objMaterialExtra = NewDefaultMultiMtl();
	QString name = QString("%1").arg("an(i)maMaterialExtra");
	objMaterialExtra->SetName(MSTR(_QM(name)));
	lib->AddMtl(objMaterialExtra);

	// ---

	QList<Mtl *> loadedMatList;
	Anima::GeomExtCollection &geomList = meshExporter->GetGeomExportColl();
	int _matCount=0;
	for(int i=0;i<geomList.Count();i++) _matCount+=geomList.GetGeom(i)->GetMaterialNameCount();

	objMaterialExtra->SetNumSubMtls(_matCount);
	int m_idx=0;
	for(int i=0;i<geomList.Count();i++)
	{
		Anima::MeshExtGeomExt *ext = geomList.GetGeom(i);

		Anima::ModelExporterRender *modelExporterRender =NULL;
		Anima::ModelExporterParams *expPars = ext->GetExportParams();
		Anima::ModelExporter *_modelExporter = expPars->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());
		if (_modelExporter!= NULL)
				modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);

		if (modelExporterRender!=NULL)
		{
			for(int m = 0 ; m< ext->GetMaterialNameCount() ; m++)
			{
				Mtl *mtl = LoadMaterial(&m_idx,objMaterialExtra,ext,_modelExporter,modelExporterRender,ext->GetMaterialName(m),m);
				loadedMatList.append(mtl);
			}
		}
	}

	//objMaterialExtra->SetNumSubMtls(loadedMatList.count());
	//for(int m=0;m<loadedMatList.count();m++)
	//{
	//	objMaterialExtra->SetSubMtl(m,loadedMatList[m]);
	//}

	bitmapProxy->GenerateProxies(1);
	MeshObjectSubMesh::PostBuildAll(globalInt->GetTime(),animaNode);
}


void MeshObject::UpdateMaterialValues()
{
	if (Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial()) return;

	//if (objMaterial==NULL) 
	//{
	//	RefList refList = GetRefList();
	//	RefListItem* item = refList.FirstItem();
	//	while (item != NULL) {
	//		ReferenceMaker* refMaker = item->maker;
	//		if (!refMaker)
	//			continue;

	//		INode *node = dynamic_cast<INode *>(refMaker);
	//		if (node!=NULL)
	//			objMaterial = (MultiMtl *)node->GetMtl();
	//		item = item->next;
	//	}
	//}
	if (modelMaterialList.count() == 0  || MeshObject::meshObject ==NULL) return;

	Anima::MeshExporter &_meshExporter = *MeshObject::meshObject->meshExporter;
	//Anima::MeshExporter _meshExporter;
	//_meshExporter.BuildCurrentFrame(0);

	Anima::MeshExtCollection &meshList = _meshExporter.GetModelExportColl();
	int _matCount=0;
	for(int i=0;i<meshList.InstanceCount();i++) _matCount+= meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();
	// TODO CHECK
	//if (objMaterial->NumSubMtls() != _matCount)
	//{
	//	return; // ERORE
	//}

	int specular_head,specular_body,rgb_value;
	if(pblock2 != NULL)
	{
		//pblock2->GetValue(anima_specular_head,0,specular_head,ivalid);
		//pblock2->GetValue(anima_specular_body,0,specular_body,ivalid);
		pblock2->GetValue(anima_rgb_value,0,rgb_value,ivalid);
	}
	else
	{
		rgb_value = 1; // default
	}

	//float head_f = ((float)specular_head -1.0f) / 4.0f;
	//float body_f = ((float)specular_body -1.0f) / 4.0f;
	float rgb_f = 2.f * (((float)rgb_value + 5.0f) / 10.0f); // 0 -- 2

	//int matIdx=0;
	QList<MeshObjectModelMaterial> &modelMaterialList = this->GetModelMaterialList();
	for(int i=0;i<meshList.InstanceCount();i++)
	{
		Anima::ModelExporterRender *modelExporterRender =NULL;
		Anima::MeshExtModel *meshExt = meshList.GetInstance(i);
	//	Ogre::MeshPtr mesh = meshExt->GetOgreMesh();
			
		Anima::ModelExporter *_modelExporter = meshExt->GetModel()->GetModel()->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());

	//	Anima::ModelExporterParams *expPars = meshExt->GetModel()->GetModel()->GetExportParams();
	//	Anima::ModelExporter *_modelExporter = expPars->GetExporter(Anima::ModelExporterType::MAX,GetMaxVersion());
		if (_modelExporter!= NULL)
			modelExporterRender = _modelExporter->GetRender((Anima::ModelExporterRenderType)current_render_type,0);

		//matIdx=0;
		int matCount = meshExt->getNumSubMeshes();
		for(int t=0;t<matCount;t++)
		{
			Anima::MeshExtSubmeshInfo &subInfo = meshExt->GetSubInfo(t);
			QString partName = subInfo.name; 
		//	Ogre::SubMesh *subMesh = mesh->getSubMesh(t);

			if (modelExporterRender!=NULL)
			{
				Anima::ModelExporterRenderMaterial *expMat = modelExporterRender->FindMatBySource(meshExt->getMaterialName(t));
				if (expMat!=NULL)
				{
//					Mtl *mtl = objMaterial->GetSubMtl (matIdx) ;
					int matIdx =   modelMaterialList[i].materialSubIdx +  t;
					Mtl *mtl = NULL;
					if(modelMaterialList[i].objMaterial->ClassID() == bakeShellClassID)
						mtl = modelMaterialList[i].objMaterial->GetSubMtl(0)->GetSubMtl (matIdx) ;
					else
						mtl = modelMaterialList[i].objMaterial->GetSubMtl (matIdx) ;
					ChangeRgb(meshExt->GetModel(), expMat,mtl,rgb_f);
				}
			}
		//	matIdx++;
		}
//
			
		/*Texmap *d_map  = mtl->GetSubTexmap(ID_DI);

		if (d_map!=NULL)
		{
			ChangeRgb(d_map,rgb_f);*/
			//IParamBlock2 *parm = d_map->GetParamBlock(0);

			////d_map->
			////float gnMult=2;
			//parm->GetValue( gn_mult_spin, 0, gnMult, ivalid);
			//parm->SetValue(gn_mult_spin,0,gnMult);

			////if (name.endsWith("_Head"))
			////{
			////	mtl->SetShinStr((20.f + head_f * 20.0f) /100.0f,0); // glossines
			////	mtl->SetShininess((40.f + head_f * 10.0f) /100.0f,0); // specular

			////	float gnMult =(1.f + detail_f * 2.0f);
			////	parm->SetValue(gn_mult_spin,0,gnMult);
			////}
			////else if (name.endsWith("_Body"))
			////{
			////	mtl->SetShinStr((0.f + body_f * 30.0f) /100.0f,0); // glossines
			////	mtl->SetShininess((0.f + body_f * 20.0f) /100.0f,0); // specular

			////	float gnMult =(1.f + detail_f * 4.0f);
			////	parm->SetValue(gn_mult_spin,0,gnMult);
			////}
		////}
	}
}
