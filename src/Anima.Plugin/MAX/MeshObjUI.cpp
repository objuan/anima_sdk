#include "stdafx.h"
#include "resource.h"

#include "notify.h"

#ifdef MAX2009_SDK
#include "ISceneEventManager.h"
#endif

#include "MeshObj.h"
//#include "MeshObjExtra.h"
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
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"

// ===========================

//IParamMap *MeshObject::pmapParam = NULL;
//IObjParam *MeshObject::ip        = NULL;
char nameBuff[1000];

#define PBLOCK_REF    0

MeshObject *MeshObject::meshObject = NULL; 

MeshObjectClassDesc meshObjectClassDesc;
MeshObjectSubMeshClassDesc meshObjecSubMeshtClassDesc;
//MeshObjectDummyClassDesc meshObjectDummyClassDesc;
//MeshObjectGeomClassDesc meshObjectGeomClassDesc;
//extern HINSTANCE plugin_hInstance;

MeshObjectClassDesc *_meshObjectClassDesc;
char programPath[1000];

ClassDesc *__stdcall MAX_API_AnimaObjectClass(HINSTANCE hInstance,const char*_programPath)
{
	strcpy(programPath,_programPath);
	return &meshObjectClassDesc;
}

ClassDesc *__stdcall MAX_API_AnimaObjectSubMeshClass(HINSTANCE hInstance)
{
	return &meshObjecSubMeshtClassDesc;
}

//ClassDesc *__stdcall MAX_API_AnimaObjectDummyClass(HINSTANCE hInstance)
//{
//	return &meshObjectDummyClassDesc;
//}
//
//ClassDesc *__stdcall MAX_API_AnimaObjectGeomClass(HINSTANCE hInstance)
//{
//	return &meshObjectGeomClassDesc;
//}

class PSCM_Accessor : public PBAccessor
{
private:
	MeshObject *obj;
public:
	PSCM_Accessor(){obj=NULL;}
	
	void Init(MeshObject *obj){this->obj=obj;}
	void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval &valid){};
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t);

};

void *MeshObjectClassDesc::Create(BOOL loading)
{
	//  AddInterface(&blobmesh_interface);
	//return new MeshObject(loading);
		

	return MAXAnima::Singleton()->CreateObject(programPath,this,loading);
}

// -------------------------

HWND hwndLblInfo=NULL;
HWND hwndLblFileName=NULL;
char labelInfoBuffer[1000];
char labelFileNameBuffer[1000];
void UpdateLabelInfo()
{
	if(MeshObject::meshObject!=NULL)
	if(hwndLblInfo!=NULL)
	{
		QString desc = MeshObject::meshObject->GetSceneDesc();
		for(int i=0;i<desc.length();i++) labelInfoBuffer[i] = desc[i].toAscii();
		labelInfoBuffer[desc.length()] = '\0';
		SendMessage(hwndLblInfo, WM_SETTEXT, 0, (LPARAM) labelInfoBuffer);
	}
}
void UpdateFileNameInfo()
{
	if(MeshObject::meshObject!=NULL)
	if(hwndLblFileName!=NULL)
	{
		QString name = QFileInfo(MeshObject::meshObject->animaPath).fileName();
		for(int i=0;i<name.length();i++) labelFileNameBuffer[i] = name[i].toAscii();
		labelFileNameBuffer[name.length()] = '\0';
		SendMessage(hwndLblFileName, WM_SETTEXT, 0, (LPARAM) labelFileNameBuffer);
	}
}
void PSCM_Accessor::Set(PB2Value &v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)
{
	switch(id)
	{
	//	case anima_specular_body:
		case anima_rgb_value:
	//	case anima_specular_head:   

			if (obj!=NULL)
				obj->UpdateMaterialValues();

			break;

		case anima_max_model_value:

			if (obj!=NULL)
				obj->UpdateMaxModel();

			UpdateLabelInfo();
			break;

		case anima_filepath: 

			MeshObject::meshObject->Clear();

			UpdateLabelInfo();
			//MeshObject::meshObject->Load(v.s,FindNodeRef(MeshObject::meshObject));

			//MeshObject::meshObject->Load();
		//	map->SetCubeMapFile(v.s); 
			break;

	}
}

// --------------------------------
PSCM_Accessor accessor;//=new PSCM_Accessor(this);

QList<HWND> defaultWindList;
Anima::ModelExporterRenderType current_type = "";

void clearObject()
{
	MeshObject::meshObject->Clear();
	//MeshObject::meshObject->disposed = true;
	//Interface *globalInt =  GetCOREInterface();
	//INode* animaNode = FindNodeRef(MeshObject::meshObject);
	//MeshObject::meshObject->Clear();
	//theHold.Begin();
	//int del = globalInt->DeleteNode(animaNode,FALSE);
	//theHold.Accept(_M("Delete"));
	//MeshObject::meshObject=NULL;
}

void OnDlgCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	bool load=false;
	if (id == IDC_BUTTON_REBUILD)
	{
		QString embeddedFile =MeshObject::meshObject->embeddedFile;
		int yy=0;

		if (embeddedFile!="" && MeshObject::meshObject->animaPath!="")
		{
			clearObject();

			//MeshObject *obj  = (MeshObject*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObject_CLASS_ID);
			//assert(obj);
			//INode *node = CreateRootNode(importInt,globalInt);
			//if (!node) return ;

			MeshObject::meshObject->Load(MeshObject::meshObject->animaPath,FindNodeRef(MeshObject::meshObject),false,true);
		}

	}
    else if (id == IDC_BUTTON_FILE_BROWSE)
	{
		load=true;
	}
	else if (id == IDC_BUTTON_RELOAD)
	{
		try
		{
			
		//MeshObject::meshObject->LoadFromGUI(MeshObject::meshObject->animaPath,FindNodeRef(MeshObject::meshObject));
			Anima::_LogDebug(QString("Reload %1").arg(MeshObject::meshObject->animaPath),__FUNCTION__,__LINE__);

			if (QFile(MeshObject::meshObject->animaPath).exists())
			{
				clearObject();
				MeshObject::meshObject->Load(MeshObject::meshObject->animaPath,FindNodeRef(MeshObject::meshObject),true,true);
			}
			else
			{
				Anima::_LogDebug("File not found",__FUNCTION__,__LINE__);
				MessageBox(NULL, _T(QString("The path to the original .ani scene has changed (%1) .\r\n\ Please use the Browser window to navigate and locate the new file.").arg(MeshObject::meshObject->animaPath).toAscii().data()), _T("WARNING!"), MB_OK);
				load=true;
			}
			UpdateLabelInfo();
		}
		catch(Anima::CoreException &e)
		{
			MessageBox(NULL, _T(QString("The path to the original .ani scene has changed (%1) .\r\n\ Please use the Browser window to navigate and locate the new file.").arg(MeshObject::meshObject->animaPath).toAscii().data()), _T("WARNING!"), MB_OK);

			Anima::_LogDebug(e.Message(),__FUNCTION__,__LINE__);
			load=true;
		}
	
	//	Anima::CommandManager::Singleton.Load(fileName);
	}

	if (load)
	{
		OPENFILENAME ofn;
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		strcpy(szFileName,MeshObject::meshObject->animaPath.toAscii().data());
		ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = "AXYZ Anima Files (*.ani)\0*.ani\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle		= "Load a 'Anima' File";
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;//|OFN_ALLOWMULTISELECT;
		ofn.lpstrDefExt = "ani";

		if(GetOpenFileName(&ofn))
		{
			// Do something usefull with the filename stored in szFileName 
			clearObject();
			//MeshObject::meshObject->LoadFromGUI(szFileName,FindNodeRef(MeshObject::meshObject));
			MeshObject::meshObject->Load(szFileName,FindNodeRef(MeshObject::meshObject),true,true);

			UpdateFileNameInfo();
			UpdateLabelInfo();
		}

		//QString path = QFileDialog::getOpenFileName(NULL  ,"Load a 'Anima' File","","Anima (*.ani)");
		//if (path!="")
		{
			//MeshObject::meshObject->Clear();
		//	MeshObject::meshObject->Load(path,FindNodeRef(MeshObject::meshObject));

		/*	QString name = QFileInfo(path).fileName();
			HWND hwndLbl = GetDlgItem(hwnd, IDC_FILE_NAME);
			LPARAM a = (LPARAM)name.toAscii().data();
			SendMessage(hwndLbl, WM_SETTEXT, 0, (LPARAM) a);*/
		}
	}
}

class ComboDlgProc : public ParamMap2UserDlgProc 
{
	int p;
	int i;
	int index;
	IRollupWindow *pRollup;
	bool show;
	HWND hwndCombo;
	HWND hwndLbl;
	MeshObject* meshObj ;
	INode *node ;
	Anima::ModelExporterRenderType currentRenderer;
	QList<Anima::ModelExporterRenderType> render_list;
	public:
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg) 
			{
				case WM_INITDIALOG:

					meshObj = (MeshObject*)map->GetParamBlock()->GetOwner();
					//node = meshObj->GetNode();

					if (meshObj!=NULL)
					{
						//QString name = QFileInfo(meshObj->animaPath).fileName();
						
						hwndLbl = GetDlgItem(hWnd, IDC_FILE_NAME);
						if(hwndLbl!=NULL)
						{
							hwndLblFileName = hwndLbl;
							UpdateFileNameInfo();
							//LPARAM a = (LPARAM)name.toAscii().data();
							//SendMessage(hwndLbl, WM_SETTEXT, 0, (LPARAM) a);
						}
				
						QString sceneDesc=meshObj->GetSceneDesc();
						hwndLbl = GetDlgItem(hWnd, IDC_STATIC_SCENE_INFO);
						if (hwndLbl!=NULL)
						{
							hwndLblInfo = hwndLbl;
							UpdateLabelInfo();
							//LPARAM a1 = (LPARAM)sceneDesc.toAscii().data();
							////SendMessage(hwndLblInfo, WM_SETTEXT, 0, (LPARAM) a1);
							//char bb[10];
							//strcpy(bb,"jkjhjh\0");
							//SendMessage(hwndLblInfo, WM_SETTEXT, 0, (LPARAM) bb);
						}
					}

					//  load the shader dropdown
					////hwndCombo = GetDlgItem(hWnd, IDC_COMBO_PRESET);
					////SendMessage(hwndCombo, CB_RESETCONTENT, 0L, 0L);

					////meshObj = (MeshObject*)map->GetParamBlock()->GetOwner();

					////index = 0;
					////Anima::ModelExporter::GetModelExporterRenderTypeList(Anima::ModelExporterType::MAX,0,render_list);
					////i=0;
					////foreach(Anima::ModelExporterRenderType type , render_list)
					////{
					////	QString &dsc = Anima::ModelExporter::GetRenderTypeDesc(type);
					////	LPARAM a = (LPARAM)dsc.toAscii().data();
					//////	int n = SendMessage(hwndCombo, CB_ADDSTRING, 0L, a );
					////	int n = ComboBox_InsertString(hwndCombo,-1,a);

					////	SendMessage(hwndCombo, CB_SETITEMDATA, n, i);
					////	if (type == meshObj->current_render_type)
					////		index = i;
					////	i++;
					////}

					//// //EnableWindow(GetDlgItem(hWnd, IDC_MTLID_NAMES_COMBO), false);

					////	//p=10;
					//// //SendMessage(hwndCombo, CB_SETMINVISIBLE, (WPARAM)p, 0);
					//// SendMessage(hwndCombo, CB_SETCURSEL, (WPARAM)index, 0);

					return TRUE;

				case WM_COMMAND:
					 switch (LOWORD(wParam))
					 {
						case IDC_BUTTON_FILE_BROWSE:
						case IDC_BUTTON_RELOAD:
						case IDC_BUTTON_REBUILD:
							HANDLE_MSG(hWnd,WM_COMMAND,OnDlgCommand);
							break;
						case IDC_COMBO_PRESET:
							switch( HIWORD(wParam) )
							{
								 HANDLE_MSG(hWnd,WM_COMMAND,OnDlgCommand);

								//case WM_COMMAND:

								//	// push
								//	wndCombo = GetDlgItem(hWnd, IDC_COMBO_PRESET);

								//	break;

								case CBN_DROPDOWN:
									{
									return TRUE;
									}

								case CBN_SELCHANGE: { // select a new item from the combo box

			//					/*	int sel = (int)SendMessage((HWND)lParam, CB_GETITEMDATA, SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0), 0);
			//						if(sel>=0)
			//						{
			//							meshObj = (MeshObject*)map->GetParamBlock()->GetOwner();

			//							pRollup = meshObj->ip->GetCommandPanelRollup();

			//							Anima::ModelExporter::GetModelExporterRenderTypeList(Anima::ModelExporterType::MAX,0,render_list);
			//
			//							meshObj->current_render_type =render_list[sel];

			//							show = meshObj->current_render_type == Anima::ModelExporterRenderType_DEFAULT;
			//							if (show)
			//							{
			//								for(int i=0;i<pRollup->GetNumPanels();i++)
			//								{
			//									if (defaultWindList.contains(pRollup->GetPanelDlg(i)))
			//									{
			//										pRollup->Show(i);
			//									}
			//								}
			//							}
			//							else
			//							{
			//								for(int i=0;i<pRollup->GetNumPanels();i++)
			//								{
			//									if (defaultWindList.contains(pRollup->GetPanelDlg(i)))
			//									{
			//										pRollup->Hide(i);
			//									}
			//								}
			//							}

			//							pRollup->UpdateLayout ();
			//							meshObj->RebuildMaterial();
			//						}
			//						*/
									return TRUE;
								}
							}
					 }
			}
			return FALSE;
		}
		void DeleteThis() { }
};

class CommonDlgProc : public ParamMap2UserDlgProc 
{
public:
	INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
				case WM_INITDIALOG:
					defaultWindList.append(hWnd);
		}
		return FALSE;
	}
	void DeleteThis() { }
};

static ComboDlgProc comboDlgProc;
static CommonDlgProc commonDlgProc;

ParamBlockDesc2 obj_param_blk(anima_params, _T("anima parameters"), 0, &meshObjectClassDesc, P_AUTO_CONSTRUCT + P_AUTO_UI+P_MULTIMAP, PBLOCK_REF,
//
6,

//anima_spec_params,ID_SPECULAR_PANEL, IDS_PARAM_SPEC, 0, 0, &commonDlgProc,

anima_rgb_params,ID_DETAIL_PANEL, IDS_PARAM_DETAIL, 0, 0, &commonDlgProc,
anima_display_params,ID_DISPLAY_PANEL, IDS_DISPLAY_PROPERTIES, 0, 0, &commonDlgProc,
anima_render_params,ID_RENDERING_PANEL, IDS_RENDERING, 0, 0, &comboDlgProc,
anima_update_params,ID_UPDATE_PANEL, IDS_PARAM_UPDATE, 0, 0, &comboDlgProc,
anima_info,ID_INFO_PANEL, IDS_PARAM_INFO, 0, 0, NULL,
anima_info,ID_SCENE_INFO_PANEL, IDS_SCENE_PARAM_INFO, 0, 0, &comboDlgProc,

// params
//anima_specular_head, _T("spec head"), TYPE_INT, P_ANIMATABLE, IDS_SPEC_HEAD,
//	p_default, 1,
//	p_ui, anima_spec_params, TYPE_SLIDER, EDITTYPE_INT, IDC_SPEC_HEAD_EDIT, IDC_SPEC_HEAD_SLIDER, 10,
//	p_range, 1, 5,
//	p_accessor,    &accessor,
//	end,
//
//anima_specular_body, _T("spec body"), TYPE_INT, P_ANIMATABLE, IDS_SPEC_BODY,
//	p_default, 1,
//	p_ui, anima_spec_params,TYPE_SLIDER, EDITTYPE_INT, IDC_SPEC_BODY_EDIT, IDC_SPEC_BODY_SLIDER, 10,
//	p_range, 1, 5,
//	p_accessor,    &accessor,
//	end,
//
//anima_detail, _T("detail"), TYPE_INT, P_ANIMATABLE, IDS_DETAIL,
//	p_default, 1,
//	p_ui, anima_detail_params,TYPE_SLIDER, EDITTYPE_INT, IDC_DETAIL_EDIT, IDC_DETAIL_SLIDER, 10,
//	p_range, 1, 5,
//	p_accessor,    &accessor,
//	end,

anima_rgb_value, _T("Texture RGB Level"), TYPE_INT, P_ANIMATABLE, IDS_DETAIL,
	p_default, 0,
	p_ui, anima_rgb_params,TYPE_SLIDER, EDITTYPE_INT, IDC_DETAIL_EDIT, IDC_DETAIL_SLIDER, 10,
	p_range, -5, 5,
	p_accessor,    &accessor,
	end,

anima_max_model_value, _T("Max Model"), TYPE_INT, P_ANIMATABLE, IDS_MAX_MODEL,
	p_default, 99,
	p_ui, anima_display_params,TYPE_SLIDER, EDITTYPE_INT, IDC_MODEL_EDIT, IDC_MODEL_SLIDER, 10,
	p_range, 0, 999,
	p_accessor,    &accessor,
	end,

  anima_renderable_value,   _T("Renderable"),   TYPE_BOOL,     0,          IDS_RENDERABLE,
      p_default,    TRUE,
      p_ui,          anima_render_params, TYPE_SINGLECHEKBOX,    IDC_RENDERABLE, 
      end, 

  anima_smoothing_value,   _T("Smoothing"),   TYPE_BOOL,     0,          IDS_SMOOTHING,
      p_default,    TRUE,
      p_ui,          anima_render_params, TYPE_SINGLECHEKBOX,    IDC_SMOOTING, 
      end, 

  //anima_embedded_mode,   _T("Embedded"),   TYPE_BOOL,     0,          IDS_EMBEDDED,
  //    p_default,    TRUE,
  //    p_ui,          anima_update_params, TYPE_SINGLECHEKBOX,    IDC_EMBEDDED, 
  //    end, 

anima_blur_mode, _T("Object Blur"), TYPE_INT,	0,	IDS_BLURMODE,
	p_default,		0,
	p_range,		0,	2,
	p_ui,anima_render_params,	TYPE_RADIO, 3, IDC_RADIO_B_NONE, IDC_RADIO_B_OBJ,IDC_RADIO_B_IMG,
	//p_accessor,    accessor,
	end,

 // anima_update_mode, _T("Update Options"), TYPE_INT,	0,	IDS_UPDATE_MODE,
	//p_default,		0,
	//p_range,		0,	1,
	//p_ui,anima_update_params,	TYPE_RADIO, 2, IDC_UPDATE_MODE_1, IDC_UPDATE_MODE_2,
	////p_accessor,    accessor,
	//end,

//anima_filepath,	_T("."),   TYPE_FILENAME,	0,	IDS_DS_FILENAME,
//		p_ui, anima_update_params,TYPE_FILEOPENBUTTON, IDC_FILE,
//		p_file_types,	IDS_CM_FILE_TYPES,
//		p_caption,		IDS_ANIMAFILENAME,
//		p_accessor,    &accessor,
//		end,
//

/*
anima_visible_mode,	_T("visibleMode"),   TYPE_BOOL,			0,	IDS_HIDE,
	p_default,		FALSE,
	p_ui, 			TYPE_SINGLECHEKBOX, 	IDC_HIDE, 
	end,
*/
	end
);

//void NOTIFYPROC_POST_RENDERER_CHANGE(void *param, NotifyInfo *info)  
//{
//	((MeshObject *)param)->CheckMaterial();
//}

MeshObject::MeshObject(MeshObjectClassDesc *classDesc,BOOL loading)
{
//	meshObjectClassDesc = &_meshObjectClassDesc;
	disposed=false;
	//if (MeshObject::meshObject != NULL)
	//{
	//	// c'era già
	//	int y=0;
	//	MeshObject::meshObject->Clear();
	//	delete MeshObject::meshObject;
	//	MeshObject::meshObject=NULL;
	//}
	accessor.Init(this);

	MeshObject::meshObject=this;
//	lastUpdateTime= SecToTicks(999999999999);
	_meshObjectClassDesc = classDesc;

	current_render_type = "";//Anima::ModelExporterRenderType_DEFAULT;

	meshObjectClassDesc.MakeAutoParamBlocks(this);
	//pblock2->SetValue(anima_specular_head,0,2);

//	objMaterial=NULL;
	matBuilded=false;
	meshExporter=NULL;
	invalidate=false;
	//ivalid.SetInfinite();
	taskDone=0;
//	maxModelDetail = 99;
	toBuildVisibility=false;

	meshExporter = NULL;
	loaded=false;
	loading=false;
	initializing=false;
	clearing=false;
	connected=false;
	embeddedFile="";
	// notifiche
//	RegisterNotification (&NOTIFYPROC_POST_RENDERER_CHANGE,this,NOTIFY_POST_RENDERER_CHANGE);
}

MeshObject::~MeshObject()
{
	accessor.Init(NULL);
	if (meshExporter!=NULL) delete meshExporter;
	if (MeshObject::meshObject==this)
		meshExporter=NULL;
	//meshObject=NULL;
}
void MeshObject::DeleteThis() 
{
	if (MeshObject::meshObject==this)
		MeshObject::meshObject = NULL;
	delete this;
	/*if (meshExporter!=NULL) delete meshExporter;
	meshExporter=NULL;
	FreeCaches ();*/
}

#define CURRENT_VERSION_CHUNK   1004
#define CURRENT_RENDER_TYPE_CHUNK   1005
#define CURRENT_RENDER_TYPE_LEN_CHUNK   1006
#define PATH_CHUNK            1010
#define PATH_LEN_CHUNK            1011
#define EMBEDDED_CHUNK            1015
#define EMBEDDED_LEN_CHUNK           1020

IOResult MeshObject::Load(ILoad *iload) 
{	
	char *buff;
	QString _n;
	QString filePath;
	int i ;
	ULONG nb;
	IOResult res;
	int embeddedLen = 0;
	int pathLen = 0;
	int renderTypeLen = 0;
	embeddedFile=""; // per compatibilità
	//TCHAR* buffer ;
	char *cbuffer;
	int version=0;

	while ( IO_OK == (res = iload->OpenChunk()) )
	{
      switch ( iload->CurChunkID() )
      {
	  case CURRENT_VERSION_CHUNK:
			res = iload->Read(&version, sizeof(int), &nb);
			break;
		case CURRENT_RENDER_TYPE_LEN_CHUNK:
			res = iload->Read(&renderTypeLen, sizeof(int), &nb);
			break;
		case PATH_LEN_CHUNK:
			res = iload->Read(&pathLen, sizeof(int), &nb);
		break;
		case EMBEDDED_LEN_CHUNK:
			res = iload->Read(&embeddedLen, sizeof(int), &nb);
		break;

        case CURRENT_RENDER_TYPE_CHUNK: 

			if (version >=1 && renderTypeLen>0)
			{
				cbuffer = new char[renderTypeLen];
				res = iload->Read(cbuffer, sizeof(char)*renderTypeLen, &nb);
				current_render_type = QString::fromUtf8(cbuffer,renderTypeLen);
				delete cbuffer;
			}
            break;

         case PATH_CHUNK:
            
			if (version >=1 && pathLen>0)
			{
				cbuffer = new char[pathLen];
				res = iload->Read(cbuffer, sizeof(char)*pathLen, &nb);
				filePath = QString::fromUtf8(cbuffer,pathLen);

				Anima::_LogDebug(QString("load filePath = '%1'").arg(filePath),__FUNCTION__,__LINE__);
				animaPath = filePath;
				delete cbuffer;
			}
            break;

		case EMBEDDED_CHUNK: 

			if (version >=1 && embeddedLen>0)
			{
				cbuffer = new char[embeddedLen];
				res = iload->Read(cbuffer, sizeof(char)*embeddedLen, &nb);
				embeddedFile = QString::fromUtf8(cbuffer,embeddedLen);

				//Anima::_LogDebug(QString("load embeddedFile = '%1'").arg(embeddedFile),__FUNCTION__,__LINE__);
				Anima::_LogDebug(QString("load embeddedFile "),__FUNCTION__,__LINE__);
				loaded=false;
				loading=false;
				delete cbuffer;
			}
			break;

      }
      iload->CloseChunk();
  	
      if (res != IO_OK)
         return res;
   }
	// 
//   Load(filePath,FindNodeRef(this),false);

   return IO_OK;

	//int smoothHead=0;
	//pblock->GetValue(PB_SMOOTH_HEAD, 0, smoothHead, ivalid);

	//iload->RegisterPostLoadCallback(
	//	new ParamBlockPLCB(NULL,0,&curVersion,this,0));

	//int smoothHead=0;
	//pblock2->GetValue(anima_specular_head,0,smoothHead,ivalid);

	//return IO_OK;
}

IOResult MeshObject::Save(ISave* isave) 
{
	ULONG nb;
	IOResult res;
	QString _current_render_type = current_render_type;
	int version = 1;
	int len;
	QByteArray ba;
	char *buffer;

	isave->BeginChunk(CURRENT_VERSION_CHUNK);
	if ((res = isave->Write(&version, sizeof(int), &nb)) != IO_OK) return res;
	isave->EndChunk();

	ba = _current_render_type.toUtf8();
	buffer = ba.data();
	len = ba.length();

	isave->BeginChunk(CURRENT_RENDER_TYPE_LEN_CHUNK);
	if ((res = isave->Write(&len, sizeof(int), &nb)) != IO_OK) return res;
	isave->EndChunk();
	isave->BeginChunk(CURRENT_RENDER_TYPE_CHUNK);
	if ((res = isave->Write(buffer, sizeof(char)*(len), &nb)) != IO_OK) return res;
	isave->EndChunk();

	Anima::_LogDebug(QString("save animaPath = '%1'").arg(animaPath),__FUNCTION__,__LINE__);

	ba = animaPath.toUtf8();
	buffer = ba.data();
	len = ba.length();

	isave->BeginChunk(PATH_LEN_CHUNK);
	if ((res = isave->Write(&len, sizeof(int), &nb)) != IO_OK) return res;
	isave->EndChunk();
	isave->BeginChunk(PATH_CHUNK);
	if ((res = isave->Write(buffer, sizeof(char)*(len), &nb)) != IO_OK) return res;
	isave->EndChunk();

	BOOL saveEmbedded = true;
   // pblock2->GetValue(anima_embedded_mode,0,saveEmbedded,FOREVER);

	if (saveEmbedded)
	{
		//Anima::_LogDebug(QString("save embeddedFile = '%1'").arg(embeddedFile),__FUNCTION__,__LINE__);
		Anima::_LogDebug(QString("save embeddedFile"),__FUNCTION__,__LINE__);

		//TSTR s_embeddedFile(embeddedFile.toAscii().data());
		ba = embeddedFile.toUtf8();
		buffer = ba.data();
		len = ba.length();
		
		isave->BeginChunk(EMBEDDED_LEN_CHUNK);
		if ((res = isave->Write(&len, sizeof(int), &nb)) != IO_OK) return res;
		isave->EndChunk();
		isave->BeginChunk(EMBEDDED_CHUNK);
		if ((res = isave->Write(buffer, sizeof( char )*(len), &nb)) != IO_OK) return res;
		isave->EndChunk();
	}

	/*
	isave->BeginChunk(CURRENT_RENDER_CHUNK);
//	isave->Write(&_current_render_type, sizeof(int), &nb);
	char buff[1000];
	strcpy(buff,_current_render_type.toAscii().data());
	buff[_current_render_type.count()] = '|';
	buff[_current_render_type.count()+1] = '\0';
	isave->WriteCString(buff);
	isave->EndChunk();

	isave->BeginChunk(PATH_CHUNK);
	//char buff[1000];
	strcpy(buff,animaPath.toAscii().data());
	buff[animaPath.count()] = '|';
	buff[animaPath.count()+1] = '\0';
	isave->WriteCString(buff);
	isave->EndChunk();

	QByteArray arr = embeddedFile.toAscii();

	int len = arr.count ();
    isave->BeginChunk(EMBEDDED_CHUNK_LEN);
    isave->Write(&len,sizeof(len),&nb);
    isave->EndChunk();

	isave->BeginChunk(EMBEDDED_CHUNK);
	const char *emb_buff = arr.data();
	isave->Write(emb_buff,len,&nb);
	isave->EndChunk();
	//char *buff = new char[embeddedFile.length()];
	//strcpy(buff,animaPath.toAscii().data());
	//buff[embeddedFile.length().count()] = '|';
	//buff[embeddedFile.length().count()+1] = '\0';
	//isave->WriteCString(buff);
	//isave->EndChunk();
	//delete buff;
*/
	return IO_OK;
}

//TCHAR *GetString(int id)
//   {
//   static TCHAR buf[256];
//
//   if (hInstance)
//   {
//      TCHAR *c =  LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
//	  return c;
//   }
//   return NULL;
//   }

static HWND hAnimaPars;

void MeshObject::BeginEditParams(IObjParam *ip,ULONG flags,Animatable *prev)
{
	SimpleObject::BeginEditParams(ip,flags,prev);
//	this->ip = ip;
	meshObjectClassDesc.BeginEditParams(ip, this, flags, prev);

	//IRollupWindow *pRollup;
	//bool show;
	//pRollup = ip->GetCommandPanelRollup();
	//show = current_render_type == Anima::ModelExporterRenderType_DEFAULT;
	//if (show)
	//{
	//	for(int i=0;i<pRollup->GetNumPanels();i++)
	//	{
	//		if (defaultWindList.contains(pRollup->GetPanelDlg(i)))
	//		{
	//			pRollup->Show(i);
	//		}
	//	}
	//}
	//else
	//{
	//	for(int i=0;i<pRollup->GetNumPanels();i++)
	//	{
	//		if (defaultWindList.contains(pRollup->GetPanelDlg(i)))
	//		{
	//			pRollup->Hide(i);
	//		}
	//	}
	//}

	//pRollup->UpdateLayout ();
}
		
void MeshObject::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{		
	SimpleObject::EndEditParams(ip,flags,next);
//	this->ip = NULL;

	/*if (flags&END_EDIT_REMOVEUI ) {		
		DestroyCPParamMap(pmapParam);
		pmapParam  = NULL;		
		}*/
	meshObjectClassDesc.EndEditParams(ip, this, flags, next);
}

BOOL MeshObject::HasUVW() { 
	return FALSE;
	}

void MeshObject::SetGenUVW(BOOL sw) {  
	if (sw==HasUVW()) return;	
}


// ==================================================================================
// ==================================================================================

class HedraObjCreateCallBack : public CreateMouseCallBack {
	IPoint2 sp0;
	MeshObject *ob;
	Point3 p0;
	public:
		int proc( ViewExp *vpt,int msg, int point, int flags, IPoint2 m, Matrix3& mat);
		void SetObj(MeshObject *obj) {ob = obj;}
	};

int HedraObjCreateCallBack::proc(ViewExp *vpt,int msg, int point, int flags, IPoint2 m, Matrix3& mat ) {
	float r;
	return TRUE;
}

static HedraObjCreateCallBack hedraCreateCB;

CreateMouseCallBack* MeshObject::GetCreateMouseCallBack() 
{
	hedraCreateCB.SetObj(this);
	return(&hedraCreateCB);
}


BOOL MeshObject::OKtoDisplay(TimeValue t) 
{
	return TRUE;
}

void MeshObject::InvalidateUI() 
{
//	if (pmapParam) pmapParam->Invalidate();
}

RefTargetHandle MeshObject::Clone(RemapDir& remap) 
{
	MeshObject* newob = new MeshObject(NULL,false);	
	newob->ReplaceReference(0,remap.CloneRef(pblock));
	newob->ivalid.SetEmpty();	
	BaseClone(this, newob, remap);
	return(newob);
}

