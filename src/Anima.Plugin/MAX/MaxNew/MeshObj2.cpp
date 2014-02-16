#include "stdafx.h"
#include "resource.h"
#include "decomp.h"

#include "MeshObj.h"
#include "MAXAnima.h"
#include <MeshNormalSpec.h> 

#include "Core/Context.h"
#include "Core/Film.h"
#include "Core/FilmLayer.h"
#include "Core/Animation.h"
#include "Core/ModelInstance.h"
#include "Core/Model.h"
#include "Core/ModelLibrary.h"
#include "Core/Config.h"
#include "Task/TaskQueue.h"
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxPerformanceManager.h"
#include "Gfx/GfxBaseModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"
#include "Export/PresetsExporter.h"

void UpdateLabelInfo();

//INode *MeshObject::GetNode()
//{
//	return NULL;
//}

void MeshObject::RebuildMaterial()
{
	BuildMaterial();
	UpdateMaterialValues();
}

DWORD WINAPI BuildMesh_FN(LPVOID arg)
{
	return 0;
}
//
//INode *MeshObject::FindAnimaNode(INode *node)
//{
//	Object *obj = node->GetObjOrWSMRef  () ;
//	if (obj != NULL)
//	{
//		if (obj->ClassID()==MeshObject_CLASS_ID  )
//		{
//			return node;
//		}
//	}
//	for(int i=0;i<node->NumberOfChildren ();i++)
//	{
//		INode *n = FindAnimaNode(node->GetChildNode (i));
//		if (n != NULL)
//			return n;
//	}
//	return NULL;
//}
//INode *MeshObject::FindAnimaNode()
//{
//	Interface *globalInt =  GetCOREInterface();
//	return FindAnimaNode(globalInt->GetRootNode());
//}

void MeshObject::BuildMesh(TimeValue t) // nuovo sistema
{
	Anima::_LogDebug(QString("MeshObject BuildMesh sec %1").arg(TicksToSec(t)),__FUNCTION__,__LINE__);
	if (loading || initializing || clearing) 
	{
		return;
	}

	if (!loaded)
	{
		initializing=true;
		// forzo il load
		Interface *globalInt =  GetCOREInterface();

		INode* animaNode = FindNodeRef(this);

		/*INode* animaNode = FindAnimaNode();*/
		if(animaNode == NULL)
		{
			return;
		}

		MeshObject::meshObject->animaNode=animaNode;

		LoadFromEmbedded(t);

		loaded=true;
		initializing=false;
		loading=false;
		lastUpdateTime= SecToTicks(999999999999);
		//Clear();
		//ivalid = NEVER;
		//MeshObject::Load(animaPath,animaNode,false);
		//return;
	}
	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	EnsureTime(t);
/*
	t = lastUpdateTime;

	double sec = TicksToSec(t);
//	if (oldTime == sec) return;
//	oldTime = sec;

	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	isInPlayTime = sec <= endPlayTime;
	if (!isInPlayTime)
	{
		if (meshExporter!=NULL)
			SetVisibility();
		return;	
	}

	Anima::Film &film = Anima::Context::Singleton.GetFilm();
	//int fps = GetFrameRate();
	film.SetPlayTime(sec);

	Anima::_LogDebug(QString("MeshObject SetPlayTime sec %1").arg(sec),__FUNCTION__,__LINE__);

	//film.SetFPS(true,fps); // TODO
	//film.SetCurrentFrame(sec * film.GetFPS());

	if (meshExporter!=NULL)
	{
		//Anima::GfxContext::Singleton->UpdateAndWaitTask();
		//int maxModelDetail; // LINK del numero
		//pblock2->GetValue(anima_max_model_value,0,maxModelDetail,ivalid);
		//Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(maxModelDetail);

		Anima::GfxContext::Singleton->UpdateAndContinue();
		meshExporter->UpdateCurrentTime();
		SetVisibility();
	}
	*/
}

void MeshObject::EnsureTime(TimeValue t)
{
	if (t != lastUpdateTime)
	{
		lastUpdateTime = t;

		double sec = TicksToSec(t);
	//	if (oldTime == sec) return;
	//	oldTime = sec;

		//ivalid.SetStart(t);// = FOREVER;
		//ivalid.SetEnd(t);

		isInPlayTime = sec <= endPlayTime;
		if (!isInPlayTime)
		{
			if (meshExporter!=NULL)
				SetVisibility();
			return;	
		}

		Anima::Film &film = Anima::Context::Singleton.GetFilm();
		//int fps = GetFrameRate();
		film.SetPlayTime(sec);

		Anima::_LogDebug(QString("MeshObject SetPlayTime sec %1").arg(sec),__FUNCTION__,__LINE__);

		//film.SetFPS(true,fps); // TODO
		//film.SetCurrentFrame(sec * film.GetFPS());

		if (meshExporter!=NULL)
		{
			//Anima::GfxContext::Singleton->UpdateAndWaitTask();
			//int maxModelDetail; // LINK del numero
			//pblock2->GetValue(anima_max_model_value,0,maxModelDetail,ivalid);
			//Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(maxModelDetail);

			Anima::GfxContext::Singleton->UpdateAndContinue();
			meshExporter->UpdateCurrentTime();
			SetVisibility();
		}
	}
}

void ScanNodes(INode *parentNode,INode *node,QList<INode *> &nodes)
{
	Object *obj = node->GetObjOrWSMRef  () ;

	if (obj != NULL)
	{
		//if (obj->ClassID()==MeshObject_CLASS_ID || obj->ClassID()==MeshObjectSubMesh_CLASS_ID )
		if ( obj->ClassID()==MeshObjectSubMesh_CLASS_ID )
		{
			INode *n = node;
			do
			{
				if (nodes.indexOf(n) == -1) 
				{
					//MCHAR *name = n->GetName();
					nodes.append(n);
				}
				else
					break;

				n = n->GetParentNode ();
			}
			while(n != parentNode);
		}
	}
	for(int i=0;i<node->NumberOfChildren ();i++)
	{
		ScanNodes(parentNode,node->GetChildNode (i),nodes);
	}
}

void MeshObject::_Clear()
{
	clearing=true;
	Interface *globalInt =  GetCOREInterface();

	MtlBaseLib *lib = globalInt->GetSceneMtls();
	int idx = lib->FindMtlByName(MSTR("an(i)maMaterial")); // TODO BETTER
	if (idx != -1)
	{
		lib->RemoveEntry (idx); // SI PIANTAVA SENZA DELETE NEL 2009
	}
	idx = lib->FindMtlByName(MSTR("an(i)maMaterialExtra")); // TODO BETTER
	if (idx != -1)
	{
		lib->RemoveEntry (idx); // SI PIANTAVA SENZA DELETE NEL 2009
	}

	QList<INode *> nodes;
	ScanNodes(globalInt->GetRootNode(),globalInt->GetRootNode(),nodes);

	QList<Object *> olist;
	for(int i=0;i<nodes.count();i++)
	{
		Object *obj = nodes[i]->GetObjOrWSMRef  () ;
		if (obj!=NULL)
		{
			if (obj->ClassID()==MeshObjectSubMesh_CLASS_ID )
			{
				olist.append(obj);
			}
		}
	}

	for(int i=0;i<nodes.count();i++)
	{
		MCHAR *name = nodes[i]->GetName();
		// tutti tranne la radice
		/*if (nodes[i]->GetParentNode () == globalInt->GetRootNode())
		{
			if (animaNode != 
		}*/
		if (nodes[i] != animaNode)
		{
			theHold.Begin();
			int del = globalInt->DeleteNode(nodes[i],FALSE);
			theHold.Accept(_M("Delete"));
		}
	}
	//theHold.Begin();
 //   globalInt->DeleteNode(animaNode,FALSE);
 //   theHold.Accept(_M("Delete"));
	//for(int i=0;i<olist.count();i++)
	//{
	//	olist[i]->DeleteThis();
	//}
	if (meshExporter != NULL) 
	{
		delete meshExporter;
		meshExporter=NULL;
	}
	clearing=false;
	//main_meshList.clear();
	//animaNode=NULL;
}

void MeshObject::UpdateMaxModel()
{
	//int maxModelDetail; // LINK del numero
	//pblock2->GetValue(anima_max_model_value,0,maxModelDetail,ivalid);
	//Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(maxModelDetail);
	//Anima::GfxContext::Singleton->UpdateAndContinue();
	SetVisibility();
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
	GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime()); 
}

bool MeshObject::SetVisibility()
{
	bool changed=false;
	int maxModelDetail;
	pblock2->GetValue(anima_max_model_value,0,maxModelDetail,ivalid);

	Interface *globalInt =  GetCOREInterface();
	int visCount=0;
	for(int i=0;i<main_modelList.count();i++)
	{
		MeshObjectSubMesh *mesh = main_modelList[i];
		INode*node  = FindNodeRef(mesh);
		if (node != NULL)
		{
			bool bboxMode=true;
			Anima::ModelInstance *mi = mesh->meshExtModel->GetModel();
		//	if (mi->IsRenderVisibleFromRoot())
			if (isInPlayTime && mi->GetGfx()->IsVisible())
			{
				node->Hide(FALSE);
				if (mi->IsReady() || node->Selected ())
				{
					visCount++;
					bboxMode = !(visCount <= maxModelDetail || node->Selected ());
				}
				int b = bboxMode ? 1 : 0;
				if (node->GetBoxMode() != b  )
				{
					changed=true;
					node->BoxMode (bboxMode);
					mesh->Invalidate ();
					globalInt->NodeInvalidateRect  ( node   );  
				}
				if (mi->IsReady())
					node->SetWireColor( RGB(255,255,255));
				else
					node->SetWireColor( RGB(255,0,0));
			}
			else
			{
				if (!isInPlayTime)
				{
					node->SetRenderable(false);
					node->SetMotBlur(false);
				}
				node->Hide(TRUE);
			}
		}
	}

	return changed;
	////	Interface *globalInt =  GetCOREInterface();
	//////int visCount=0;
	////for(int i=0;i<main_meshList.count();i++)
	////{
	////	MeshObjectSubMesh *mesh = main_meshList[i];
	////	INode*node  = FindNodeRef(mesh);
	////	if (node != NULL)
	////	{
	////		bool bboxMode=true;
	////		Anima::ModelInstance *mi = mesh->meshExt->GetModel();
	////		if (mi->IsVisible())
	////		{
	////			node->Hide(FALSE);
	////			if (mesh->bboxMode != mi->IsBBoxMode())
	////			{
	////				// invalido
	////				mesh->Invalidate();
	////				//node->BoxMode (bboxMode);
	////			}
	////		/*	bboxMode = mi->IsBBoxMode();
	////			node->BoxMode (bboxMode);*/

	////		///*	if (mi->IsReady() || node->Selected ())
	////		//	{
	////		//		visCount++;
	////		//		bboxMode = !(visCount < maxModelDetail || node->Selected ());
	////		//	}*/
	////			///*if (node->GetBoxMode() != bboxMode  )
	////			//{
	////			//	node->BoxMode (bboxMode);
	////			//	mesh->InvalidateGeom ();
	////			//	globalInt->NodeInvalidateRect  ( node   );  
	////			//}*/
	////		}
	////		else
	////			node->Hide(TRUE);
	////	}
	////}
	//globalInt->ForceCompleteRedraw ();
}

void MeshObject::OnTaskManagerChanged(Anima::Task *task)
{
	if (!loaded || loading || initializing) return;

	int wc = Anima::Context::Singleton.GetGfxBuildQueue()->WaitCount();
	taskDone++;
	if (wc == 0 || taskDone >= 4)
	{
		taskDone=0;
		// invalido
		toBuildVisibility=true;

	/*	 Anima::GfxContext::Singleton->UpdateAndContinue();
		SetVisibility();*/
		//GetCOREInterface()->ForceCompleteRedraw (); NO

	//	GetCOREInterface7()->RedrawViewportsLater (GetCOREInterface()->GetTime());

		NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
		GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime()); 
	}
}

void __stdcall On_OnTaskManagerChanged(Anima::Task *task)
{
	if (MeshObject::meshObject!=NULL)
		MeshObject::meshObject->OnTaskManagerChanged(task);
}

void  MeshObject::OnRefresh ()
{
	if (!loaded || loading || initializing) return;

	if (toBuildVisibility)
	{
		UpdateLabelInfo();

		toBuildVisibility=false;
		// invalido
	    Anima::GfxContext::Singleton->UpdateAndContinue();
		bool ch = SetVisibility();
		//if (ch) 
		//{
		//	toBuildVisibility = true;
		//	//GetCOREInterface7()->RedrawViewportsLater (GetCOREInterface()->GetTime());
		////	GetCOREInterface()->ForceCompleteRedraw(); 
		//}
	}
}

void MeshObject::OnPreRender()
{
    BOOL doRender;
    pblock2->GetValue(anima_renderable_value,0,doRender,FOREVER);
	BOOL doSmoothing;
    pblock2->GetValue(anima_smoothing_value,0,doSmoothing,FOREVER);
	int blurMode=0;
    pblock2->GetValue(anima_blur_mode,0,blurMode,FOREVER);

	animaNode->SetRenderable(doRender);
	animaNode->SetMotBlur(blurMode);
	foreach(MeshObjectSubMesh *m,  main_modelList)
	{
		INode* node = FindNodeRef(m);
		if (node!=NULL)
		{
			if (doSmoothing)
			{
				m->EnsureSmooth();
			}
			node->SetRenderable(doRender);
			node->SetMotBlur(blurMode);
		}
	}
	foreach(MeshObjectSubMesh *m,  main_geomList)
	{
		INode* node = FindNodeRef(m);
		if (node!=NULL)
		{
			if (doSmoothing)
			{
				m->EnsureSmooth();
			}
			node->SetRenderable(doRender);
			node->SetMotBlur(blurMode);
		}
	}

	Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("Building.."),2,true);
	progressBar->PushPhase("..Geometry",1);
	progressBar->PushPhase("..IK",1);

	Anima::Context::Singleton.GetGfxBuildQueue()->SetRenderMode(true); // disabilito multithread
	Anima::GfxContext::Singleton->UpdateAndWaitTask();
	delete progressBar;
}

void MeshObject::OnPostRender()
{
	Anima::Context::Singleton.GetGfxBuildQueue()->SetRenderMode(false); // abilito multithread
}

void MeshObject::OnSelectChanged()
{
//	INode* animaNode = GetNode(); //GetAnimaNode()
	if (animaNode == NULL) return;

	/*QList<INode *> nodes;
	ScanNodes(animaNode,animaNode,nodes);
	QList<INode *> new_sel_nodes;
	for(int i=0;i<nodes.count();i++)
	{
		if (nodes[i]->Selected())
		{
			Object *obj = nodes[i]->GetObjOrWSMRef  () ;
			if (obj!=NULL)
			{
				if (obj->ClassID()==MeshObjectSubMesh_CLASS_ID )
				{
					new_sel_nodes.append(nodes[i]);
				}
			}
		}
	}*/
	QList<INode *> new_sel_nodes;
	for(int i=0;i<main_modelList.count();i++)
	{
		if (main_modelList[i]->GetNode()->Selected())
		{
			new_sel_nodes.append(main_modelList[i]->GetNode());
		}
	}

	bool changed = false;
	if (new_sel_nodes.count() != selectedNodes.count()) changed=true;
	else
	{
		foreach(INode * n , new_sel_nodes)
		{
			if (!selectedNodes.contains(n)) changed=true;
		}
	}
	if (changed)
	{
		SetVisibility();
		GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime()); 
	}
	selectedNodes = new_sel_nodes;
}
			
extern int GetMaxVersion();

void ResetScale(INode *node,TimeValue t)
{
//	Control *tmControl = node->GetTMController();
//	BOOL lookAt = tmControl->GetRollController() ? TRUE : FALSE;
	Matrix3 ntm = node->GetNodeTM(t);
	Matrix3 ptm = node->GetParentTM(t);
	Matrix3 rtm = ntm * Inverse(ptm);
	Matrix3 otm(1);
	Quat rot;
	// Grab the transform
	Point3 trans = rtm.GetTrans();
	// Clear the transform on the matrix
	rtm.NoTrans();

	// We're only doing scale - save out the
	// rotation so we can put it back
	AffineParts parts;
	decomp_affine(rtm, &parts);
	rot = parts.q;

	// Build the offset transform matrix

	otm.PreTranslate(node->GetObjOffsetPos());
	if (node->GetObjOffsetRot()!=IdentQuat()) {
		PreRotateMatrix(otm,node->GetObjOffsetRot());
	}

	Point3 tS(1,1,1);
	if ( node->GetObjOffsetScale().s != tS ) {
		ApplyScaling(otm,node->GetObjOffsetScale());
	}
	// Apply the relative transform matrix to the offset

	otm = otm * rtm;
	decomp_affine(otm, &parts);
	node->SetObjOffsetPos(parts.t);
	node->SetObjOffsetScale(ScaleValue(parts.k*parts.f,parts.u));

	// Now set the transform controller with a matrix
	// that has no rotation or scale
	rtm.IdentityMatrix();
	rtm.SetTrans(trans);
	//if (!lookAt) {
	PreRotateMatrix(rtm,rot);
	//}

	// But first, want to keep children stationary.

	Matrix3 ttm = rtm*ptm;
	for (int i=0; i<node->NumberOfChildren(); i++) {
		Control *tmc = node->GetChildNode(i)->GetTMController();
		Matrix3 oldtm = node->GetChildNode(i)->GetNodeTM(t);
		SetXFormPacket pk(oldtm,ttm);
		tmc->SetValue(t,&pk);
	}
	SetXFormPacket pckt(rtm);

	//tmControl->SetValue(t,&pckt);

}

int MeshObject::LoadFromEmbedded(TimeValue t)
{
	if (embeddedFile =="") {
				
		MessageBox(NULL, _T("Ricazzo"), _T("WARNING!"), MB_OK);
		return 0;
	}

	Anima::_LogDebug(QString("Load LoadFromEmbedded"),__FUNCTION__,__LINE__);
	if (meshExporter == NULL) 
		meshExporter = new Anima::MeshExporter();

	Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("loading '%1'..").arg(QFileInfo(animaPath).fileName()),4,true);

	Anima::Context::Singleton.Clear();
	bool ok= true;
	try
	{
		Anima::ContextIO contextIO;
	
		ok = contextIO.Load(embeddedFile,loadedAnimaPath,false,progressBar); // carico nell'ambiente

		Anima::Film &film = Anima::Context::Singleton.GetFilm();
		if (!connected)
		{
			connected=true;
			Anima::Context::Singleton.GetGfxBuildQueue()->Register_TaskQueueHandler(On_OnTaskManagerChanged);
		}
			
		Interface *globalInt =  GetCOREInterface();
		Interval currentInt =  globalInt->GetAnimRange();
		if (Anima::Config::IsDemo())
			film.SetPlaybackBounds(true, film.GetStartPlaybackFrame(),film.GetFPS());

		double startTime = film.GetStartActiveTime();
		double endTime = film.GetEndActiveTime();
		TimeValue s(SecToTicks (startTime));
		TimeValue e(SecToTicks (endTime));
		endPlayTime = endTime;

		Anima::Context::Singleton.GetPresetsExporterManager()->SetSingleMaterial(Anima::ModelExporterType::MAX,GetMaxVersion(),(Anima::ModelExporterRenderType)current_render_type);

		progressBar->PushPhase("Building geometry",1);

		Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(999999);
		MeshObjectSubMesh::InitializeAll(t);

		BuildMaterial();
		UpdateMaterialValues();

		UpdateLabelInfo();
		
		progressBar->PushPhase("Building material",1);

	}
	catch(Anima::CoreException &e)
	{
		Anima::_LogDebug(e.Message(),__FUNCTION__,__LINE__);
		ok=false;
	}
	Anima::_LogDebug(QString("Load end"),__FUNCTION__,__LINE__);
	delete progressBar;
	return ( ok ) ? 0 : -1;
}

int MeshObject::LoadFromGUI(const QString &animaPath,INode* animaNode)
{
	this->animaPath=animaPath;
	this->animaNode=animaNode;
	if (animaNode == NULL)
	{
		Interface *globalInt =  GetCOREInterface();
	
		MessageBox(NULL, _T("error cazzo"), _T("WARNING!"), MB_OK);
				
		mesh.setNumVerts(0);
		mesh.setNumFaces(0);
		ivalid = NEVER;
		loading = false;
		loaded = false;
		lastUpdateTime= SecToTicks(999999999999);
		//GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
		return 1;
	}
	loading=true;
	initializing = false;
	lastUpdateTime= SecToTicks(999999999999);

	//loaded = true;
	Anima::_LogDebug(QString("Load start"),__FUNCTION__,__LINE__);

	//if (Anima::Config::IsDemo() && embeddedFile =="")
	//{
	//	MessageBox(NULL, _T("an(i)ma 1.0 Trial Edition\r\n\r\nThis software is designed to provide everyone with an opportunity to fully experience the latest version of aXYZ design® an(i)ma 3D Character animation application.\r\n\r\nThis trial version will allow you to do rendering tests and animations for a duration of 1 second.\r\n\r\nOther than the animation duration limit, this trial version has full functionality."), _T("WARNING!"), MB_OK);
	//}
			
	if (meshExporter == NULL) 
		meshExporter = new Anima::MeshExporter();

	//Interface *globalInt =  GetCOREInterface();
//	meshExporter=NULL;

	
	mesh.setNumVerts(0);
	mesh.setNumFaces(0);
//	oldTime=-1;
	ivalid = NEVER;
	Anima::ProgressBar *progressBar = Anima::Context::Singleton.GetGuiManager().OpenProgressBar(QString("loading '%1'..").arg(QFileInfo(animaPath).fileName()),4,true);
	
	Anima::Context::Singleton.Clear();
	bool ok= true;
	try
	{
		Anima::ContextIO contextIO;

		Anima::_LogDebug(QString("AnimaPath = '%1'").arg(animaPath),__FUNCTION__,__LINE__);
		if (embeddedFile !="")
			Anima::_LogDebug(QString("embeddedFile YES"),__FUNCTION__,__LINE__);

		//if (embeddedFile != "" && !loadFromAnimaFile )
		//{
		//	ok = contextIO.Load(embeddedFile,animaPath,false,progressBar); // carico nell'ambiente
		//	loadedFromEmbedded=true;
		//}
		//else
		{
			try
			{
				Anima::AnimaFileInfo *fileInfo = contextIO.PreloadFileInfo(animaPath);
				if (!fileInfo->HaveCollision)
				{
					// avviso attesa
					MessageBox(NULL, _T("The .ani scene file you are trying to load does not have any collision data saved.\r\n\If you proceed, collisions are going to be calculated while loading.\r\n\This process may take some time."), _T("WARNING!"), MB_OK);
					//GetCOREInterface()->DisplayTempPrompt("This file don't have collisions, please wait for rebuild");
				}
				delete fileInfo;
			}
			catch(...)
			{
			}
			ok = contextIO.Load(animaPath,false,progressBar); // carico nell'ambiente
		}

		invalidate=true;

		if (ok)
		{
			embeddedFile =  contextIO.GetSave(animaPath,true,NULL); 
			Anima::Film &film = Anima::Context::Singleton.GetFilm();
			if (!connected)
			{
				connected=true;
				//GetCOREInterface()->RegisterRedrawViewsCallback  (  this  )  ;
				Anima::Context::Singleton.GetGfxBuildQueue()->Register_TaskQueueHandler(On_OnTaskManagerChanged);
				//QObject::connect(&film, SIGNAL(FPSTickEvent(int )), this, SLOT(OnFPSTickEvent(int )));
			}
			
			Interface *globalInt =  GetCOREInterface();

			Interval currentInt =  globalInt->GetAnimRange();
			
			if (Anima::Config::IsDemo())
				film.SetPlaybackBounds(true, film.GetStartPlaybackFrame(),film.GetFPS());
		
			double startTime = film.GetStartActiveTime();
			double endTime = film.GetEndActiveTime();
			TimeValue s(SecToTicks (startTime));
			TimeValue e(SecToTicks (endTime));
			endPlayTime = endTime;

		//	s = MIN( s, currentInt.Start () );
		//	e = MAX( e, currentInt.End () );

			//globalInt->SetAnimRange(Interval(s,e));

			// 1 time
			Anima::Context::Singleton.GetPresetsExporterManager()->SetSingleMaterial(Anima::ModelExporterType::MAX,GetMaxVersion(),(Anima::ModelExporterRenderType)current_render_type);

			if (animaNode!=NULL)
			{
				progressBar->PushPhase("Building geometry",1);

			//	BuildMesh(globalInt->GetTime()); // forzo
				// NUOVO

				Anima::GfxContext::Singleton->GetPerformanceManager()->SetMaxModelCount(999999);
				MeshObjectSubMesh::BuildAll(globalInt->GetTime(),animaNode);

				UpdateLabelInfo();
			//	Anima::GfxContext::Singleton->UpdateAndWaitTask();

				progressBar->PushPhase("Building material",1);
			}
			BuildMaterial();
			UpdateMaterialValues();
	
		}
		else
			CheckMaterial();
		//pblock->SetValue();
		loaded=true;
		loading=false;
		ivalid = NEVER;

		//if (loadFromAnimaFile)
		{
			// scala, solo per i nuovi load

			double scale = 1.0 / GetMasterScale(UNITS_METERS);

			Matrix3 tm(1);
			Point3 pos = animaNode->GetObjOffsetPos();
			tm.PreTranslate(pos);
			Quat quat = animaNode->GetObjOffsetRot();
			PreRotateMatrix(tm, quat);
			//ScaleValue scaleValue = animaNode->GetObjOffsetScale();
			ApplyScaling(tm, Point3(scale,scale,scale));

			animaNode->SetNodeTM(TIME_NegInfinity,tm);
			animaNode->SetNodeTM(TIME_PosInfinity,tm);

			// togliere ???
			NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
			//GetCOREInterface()->ForceCompleteRedraw(); 
			GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
		}
	}
	catch(Anima::CoreException &e)
	{
		Anima::_LogDebug(e.Message(),__FUNCTION__,__LINE__);

		ok=false;
	}
	Anima::_LogDebug(QString("Load end"),__FUNCTION__,__LINE__);

	loaded=true;
	delete progressBar;
	return ( ok ) ? 0 : -1;
}

Anima::ModelExporterRenderType GetCurrentRender(Interface *globalInt);

QString MeshObject::GetSceneDesc()
{
	int actualActor=0;
	int actualTri=0;
	int actualVert=0;

	int triNumber=0;
	int vertexNumber=0;
	bool boxMode;
	int vn,tn;

	int actorNumber = main_modelList.count();

	for(int i=0;i<actorNumber;i++)
	{
		main_modelList[i]-> MeshObjectSubMesh::GetInfo(boxMode,vn,tn);
		if (!boxMode)
		{
			actualActor++;
			actualTri+= tn;
			actualVert+= vn;
		}
		vertexNumber+= vn;
		triNumber+= tn;
	}

	QString current_render_type = GetCurrentRender(  GetCOREInterface());
	QString s=QString("Actors: %2 of %1\nPolys:  %4 of %3\nVerts:   %6 of %5\nRenderer: %7\nPreset:      %8\nLicense:    %9")
		.arg(actorNumber).arg(actualActor).arg(triNumber).arg(actualTri)
		.arg(vertexNumber).arg(actualVert).arg(current_render_type).arg(Anima::Context::Singleton.GetPresetsExporterManager()->GetCurrentPreset()->Name)
		.arg( Anima::Config::IsDemo() ? "DEMO" :Anima::Config::GetLicense()->LicenseType);
	return s;
}