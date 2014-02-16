#include "stdafx.h"
#include "resource.h"

#include "MeshObj.h"
//#include "MeshObjExtra.h"
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
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxBaseModel.h"
#include "Core/IA/ModelIAActor.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"
#include "Export/PresetsExporter.h"

//QList<MeshObjectSubMesh *> main_meshList;
//Anima::MeshExporter *meshExporter;

QList<MSTR *> MeshObjectSubMesh::nameList;


void ScanNodes1(INode *parentNode,INode *node,QList<INode *> &nodes)
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
		ScanNodes1(parentNode,node->GetChildNode (i),nodes);
	}
}

void MeshObjectSubMesh::DeleteThis(){

}

// da embedded
void MeshObjectSubMesh::InitializeAll(TimeValue t)
{
	Interface *globalInt =  GetCOREInterface();
	double sec = TicksToSec(t);
	qDeleteAll(nameList);
	nameList.clear();

	//Anima::_LogDebug(QString("MeshObjectSubMesh InitializeAll %1").arg(sec),__FUNCTION__,__LINE__);
	Anima::Film &film = Anima::Context::Singleton.GetFilm();
	film.SetPlayTime(sec);
	Anima::GfxContext::Singleton->UpdateAndContinue();
	MeshObject::meshObject->main_modelList.clear();
	Anima::MeshExporter *meshExporter = MeshObject::meshObject->meshExporter;
	meshExporter->BuildCurrentTime();
	Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();

	// ---------

	QList<MeshObjectModelMaterial> &materialList = MeshObject::meshObject->GetModelMaterialList();
	materialList.clear();
	int materialPackSize = 100; // TODO PARAMS
	if(Anima::Context::Singleton.GetFilm().IsOldAnimFileVersion())
		materialPackSize = 999999;

	if (!Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial())
	{
		MeshObjectModelMaterial current;
		int c=0;
		int matIdx=0;
		int subIdx=0;
		for(int i=0,c=0;i<meshList.InstanceCount();i++,c++)
		{
			if (c == materialPackSize)
			{
				matIdx++;
				subIdx=0;
				c=0;
			}
			current.materialNumber = matIdx;
			current.materialSubIdx = subIdx;
			materialList.append(current);
			subIdx+= meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();
		}
		//if (c>0)
		//{
		//	matIdx++;
		//	current.materialNumber = matIdx;
		//	materialList.append(current);
		//}
	}
	else
	{
		MeshObjectModelMaterial current;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			current.materialNumber = 0;
			materialList.append(current);
		}
	}

	// -----------

	QList<INode *> nodes;
	ScanNodes1(globalInt->GetRootNode(),globalInt->GetRootNode(),nodes);

	QList<INode *> olist;
	for(int i=0;i<nodes.count();i++)
	{
		Object *obj = nodes[i]->GetObjOrWSMRef  () ;
		if (obj!=NULL)
		{
			if (obj->ClassID()==MeshObjectSubMesh_CLASS_ID )
			{
				olist.append(nodes[i]);
			}
		}
	}
		
	int idx=0;
	foreach(Anima::FilmLayer *layer , film.GetFilmLayers())
	{
		foreach(Anima::Animation *anim , layer->GetAnimationList())
		{
			foreach(Anima::ModelInstance *mi , anim->GetModels())
			{
				Anima::MeshExtModel *ext = meshList.Get(mi);
				if (ext!=NULL)
				{
					INode *node  = (INode *)olist[idx];
					Object *_obj = node->GetObjOrWSMRef  () ;
					MeshObjectSubMesh *obj = (MeshObjectSubMesh *)_obj;

					assert(obj);
					MeshObject::meshObject->main_modelList.append(obj);

				//	obj->Init(node);
				//	node->BoxMode (true);
				//	node->SetWireColor( RGB(255,0,0));
				
					obj->BuildModel(meshExporter,meshList.Get(mi),t);
					idx++;
				}
			}
		}
	}

		// GEOMETRIA FISSA

	INode *geomNode;

	MeshObject::meshObject->main_geomList.clear();

	int matBaseIdx=0;
	Anima::GeomExtCollection &geomList = meshExporter->GetGeomExportColl();
	for(int i=0;i<geomList.Count();i++)
	{
		Anima::MeshExtGeomExt *ext = geomList.GetGeom(i);

		INode *node  = (INode *)olist[idx];
		Object *_obj = node->GetObjOrWSMRef  () ;
		MeshObjectSubMesh *obj = (MeshObjectSubMesh *)_obj;
		//MeshObjectSubMesh *obj  = (MeshObjectSubMesh*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObjectSubMesh_CLASS_ID);
		assert(obj);

		MeshObject::meshObject->main_geomList.append(obj);
	
	//	obj->node = node;
		
		ext->MaterialBaseNumber = matBaseIdx;
		obj->BuildGeom(meshExporter,ext,t);
		matBaseIdx += ext->GetMaterialNameCount();

		idx++;
	}

	//Anima::_LogDebug(QString("MeshObjectSubMesh InitializeAll end%1").arg(sec),__FUNCTION__,__LINE__);
}

void MeshObjectSubMesh::BuildAll(TimeValue t,INode *parentNode)
{
	Interface *globalInt =  GetCOREInterface();

	double sec = TicksToSec(t);
	//if (oldTime == sec) return;
	//oldTime = sec;

	qDeleteAll(nameList);
	nameList.clear();

	//Anima::_LogDebug(QString("BuildAll start %1").arg(sec),__FUNCTION__,__LINE__);

	Anima::Film &film = Anima::Context::Singleton.GetFilm();
	//int fps = GetFrameRate();
	film.SetPlayTime(sec);
	//film.SetFPS(true,fps); // TODO
	//film.SetCurrentFrame(sec * film.GetFPS());

//	Anima::_LogDebug(QString("BuildAll 1 "),__FUNCTION__,__LINE__);

	Anima::GfxContext::Singleton->UpdateAndContinue();

	//Anima::_LogDebug(QString("BuildAll 2"),__FUNCTION__,__LINE__);

	// rifaccio tutto

//	Anima::_LogDebug(QString("BuildMesh invalid"),__FUNCTION__,__LINE__);

	// MODELLI

	MeshObject::meshObject->main_modelList.clear();
	Anima::MeshExporter *meshExporter = MeshObject::meshObject->meshExporter;
	meshExporter->BuildCurrentTime();

//Anima::_LogDebug(QString("BuildMesh invalid 1"),__FUNCTION__,__LINE__);

	Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();

	// init material pack
	QList<MeshObjectModelMaterial> &materialList = MeshObject::meshObject->GetModelMaterialList();
	materialList.clear();
	int materialPackSize = 100; // TODO PARAMS

	if (!Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial())
	{
		MeshObjectModelMaterial current;
		int c=0;
		int matIdx=0;
		int subIdx=0;
		for(int i=0,c=0;i<meshList.InstanceCount();i++,c++)
		{
			if (c == materialPackSize)
			{
				matIdx++;
				subIdx=0;
				c=0;
			}
			current.materialNumber = matIdx;
			current.materialSubIdx = subIdx;
			materialList.append(current);
			subIdx+= meshList.GetInstance(i)->GetOgreMesh()->getNumSubMeshes();
		}
		//if (c>0)
		//{
		//	matIdx++;
		//	current.materialNumber = matIdx;
		//	materialList.append(current);
		//}
	}
	else
	{
		MeshObjectModelMaterial current;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			current.materialNumber = 0;
			materialList.append(current);
		}
	}

	QList<INode *> animNodes;
	QList<INode *> groupNodes;
	foreach(Anima::FilmLayer *layer , film.GetFilmLayers())
	{
		//bool toAdd=false;
		//foreach(Anima::Animation *anim , layer->GetAnimationList())
		//{
		//	foreach(Anima::ModelInstance *mi , anim->GetModels())
		//		if (meshList.Get(mi)!=NULL)
		//		{
		//			toAdd=true;
		//			break;
		//		}
		//}
		//		
		////layer->GetAnimationList().GetModels().Count()> 0
		//if (toAdd)
		{
			MSTR gName(_QM(layer->GetName()));
			globalInt->MakeNameUnique(gName);
			DummyObject *d_obj = new DummyObject();
			d_obj->SetBox(Box3(Point3(-0.5,-0.5,-0.5),Point3(0.5,0.5,0.5)));
			INode *groupNode = globalInt->CreateObjectNode(d_obj);
			groupNode->SetName(gName);
			parentNode->AttachChild(groupNode,0);

			groupNodes.clear();
			foreach(Anima::Animation *anim , layer->GetAnimationList())
			{
				//INode *a_node = globalInt->CreateObjectNode (NULL);
			/*	DummyObject *dumObj = (DummyObject *)createInterface->CreateInstance(HELPER_CLASS_ID,Class_ID(DUMMY_CLASS_ID,0));
				assert(dumObj);
	*/
				TSTR aName(_QM(anim->GetName()));
				globalInt->MakeNameUnique(aName);
				DummyObject *d_obj = new DummyObject();
				d_obj->SetBox(Box3(Point3(-0.25,-0.25,-0.25),Point3(0.25,0.25,0.25)));
				INode *animNode = globalInt->CreateObjectNode(d_obj);
				animNode->SetName(aName);
				groupNode->AttachChild(animNode,0);

				animNodes.clear();
				foreach(Anima::ModelInstance *mi , anim->GetModels())
				{
					Anima::MeshExtModel *ext = meshList.Get(mi);
					if (ext!=NULL)
					{
						MeshObjectSubMesh *obj  = (MeshObjectSubMesh*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObjectSubMesh_CLASS_ID);
						assert(obj);

						MeshObject::meshObject->main_modelList.append(obj);
					
						INode *node = globalInt->CreateObjectNode (obj);
					//	obj->node= node;
						TSTR mName(_QM(mi->GetName()));
					//	globalInt->MakeNameUnique(mName);
						node->SetName (mName);
						//node->BoxMode (true);
						node->SetWireColor( RGB(255,0,0));
						animNode->AttachChild(node,0);

						animNodes.append(node);

						obj->BuildModel(meshExporter,meshList.Get(mi),t);
					}
				}
				
				//theHold.Begin();
				//for(int i=0;i<animNodes.count();i++) globalInt->SelectNode (animNodes[i], (i==0) ? 1 : 0);
				//MSTR *s  = new MSTR(anim->GetName().toAscii().data()); // TODO
				//nameList.append(s);
				//INode* animNode = globalInt->GroupNodes  (NULL, s,false);
				//for(int i=0;i<animNodes.count();i++) 
				//{
				//	animNode->AttachChild(animNodes[i]);
				//}
					
				//a_node->AttachChild(animNode);

				//parentNode->AttachChild(animNode);
				////layerNode->SetName (layer->GetName().toAscii().data());
				//TSTR undostr; undostr.printf("Select");
				//theHold.Accept(undostr);

				//groupNodes.append(animNode);
			}
		}
		
		//theHold.Begin();
		//for(int i=0;i<groupNodes.count();i++) globalInt->SelectNode (groupNodes[i], (i==0) ? 1 : 0);
		//MSTR *s  = new MSTR("gr"); // TODO
		//INode* groupNode = globalInt->GroupNodes  (NULL, s,false);
		//parentNode->AttachChild(groupNode);
		//for(int i=0;i<groupNodes.count();i++) 
		//{
		//	groupNode->AttachChild(groupNodes[i]);
		//}
		////layerNode->SetName (layer->GetName().toAscii().data());
		//TSTR undostr; undostr.printf("Select");
		//theHold.Accept(undostr);
	}

	// GEOMETRIA FISSA

	INode *geomNode;

	MeshObject::meshObject->main_geomList.clear();

	int matBaseIdx=0;
	Anima::GeomExtCollection &geomList = meshExporter->GetGeomExportColl();
	for(int i=0;i<geomList.Count();i++)
	{
		if (i == 0)
		{
			// metto il gruppo
			TSTR gName(_M("Extra"));
			globalInt->MakeNameUnique(gName);
			DummyObject *d_obj = new DummyObject();
			d_obj->SetBox(Box3(Point3(-0.5,-0.5,-0.5),Point3(0.5,0.5,0.5)));
			geomNode = globalInt->CreateObjectNode(d_obj);
			geomNode->SetName(gName);
			parentNode->AttachChild(geomNode,0);
		}
		Anima::MeshExtGeomExt *ext = geomList.GetGeom(i);

		MeshObjectSubMesh *obj  = (MeshObjectSubMesh*)CreateInstance(GEOMOBJECT_CLASS_ID, MeshObjectSubMesh_CLASS_ID);
		assert(obj);

		MeshObject::meshObject->main_geomList.append(obj);
	
		INode *node = globalInt->CreateObjectNode (obj);
		TSTR mName(_QM(ext->GetName()));
		globalInt->MakeNameUnique(mName);
		node->SetName (mName);
		geomNode->AttachChild(node,0);
	//	obj->node = node;
		
		ext->MaterialBaseNumber = matBaseIdx;
		obj->BuildGeom(meshExporter,ext,t);
		matBaseIdx += ext->GetMaterialNameCount();
	}

	//Anima::_LogDebug(QString("BuildMesh end"),__FUNCTION__,__LINE__);
}

void MeshObjectSubMesh::PostBuildAll(TimeValue t,INode *parentNode)
{
	if ( MeshObject::meshObject==NULL) return;
	int count = MeshObject::meshObject->main_modelList.count() + MeshObject::meshObject->main_geomList.count();
	int i = 0;
	Interface* in = GetCOREInterface();
	int  modelIdx = 0;
	foreach(MeshObjectSubMesh *mesh , MeshObject::meshObject->main_modelList)
	{
		in->ProgressUpdate((float)i++*100.0/(float)count,false,_M("Assigning Materials"));

		INode* node = FindNodeRef(mesh);
		if( node!=NULL)
			node->SetMtl(MeshObject::meshObject->GetMaterial(modelIdx));
		modelIdx++;
	}
	foreach(MeshObjectSubMesh *mesh , MeshObject::meshObject->main_geomList)
	{
		in->ProgressUpdate((float)i++*100.0/(float)count,false,_M("Assigning Materials"));
		INode* node = FindNodeRef(mesh);
		if( node!=NULL)
			node->SetMtl(MeshObject::meshObject->GetMaterialExtra());
	}
}

void *MeshObjectSubMeshClassDesc::Create(BOOL loading)
{
	//if (!loading )
	//{
		MeshObjectSubMesh *obj = new MeshObjectSubMesh(loading);
		return obj;
	/*}
	else
		return new DummyObject;*/
}

MeshObjectSubMesh::MeshObjectSubMesh(BOOL loading)
{
	build=false;
	meshExtModel=NULL;
	meshExtGeom=NULL;
	//bboxMode=false;
	initialized=false;
	smoothValid=false;
	visibilityFlag=-1; // non inizializzato
}

MeshObjectSubMesh::~MeshObjectSubMesh()
{
}

CreateMouseCallBack* MeshObjectSubMesh::GetCreateMouseCallBack()
{
	return NULL;
}

void MeshObjectSubMesh::Invalidate()
{
	ivalid = NEVER;
	mesh.InvalidateGeomCache ();
	INode* node = FindNodeRef(this);
	if (node!=NULL) node->InvalidateWS ();
}


void MeshObjectSubMesh::InvalidateGeom()
{

	mesh.InvalidateGeomCache ();
}

////void MeshObject::BuildNormals()
////{
////	//	mesh.buildNormals();
////
////	//// normals
////	BitArray& faceSel = mesh.FaceSel();
////
////	Anima::MeshExtCollection &meshList = meshExporter->GetExportColl();
////	// 'smoothThresold' list 
////	QList<float> smoothThresoldList;
////	for(int i=0;i<meshList.InstanceCount();i++)
////	{
////		Anima::MeshExt *meshExt = meshList.GetInstance(i);
////		for(int s=0;s<meshExt->GetSubInfoCount();s++)
////		{
////			float ss=(89.0f / 180.0f) * 3.1415f;
////			if ( meshExt->GetSubInfo(s).IsBody())
////				ss = (60.0f / 180.0f) * 3.1415f;
////			meshExt->GetSubInfo(s).smoothThresold = ss;
////
////			if (!smoothThresoldList.contains(ss)) 
////				smoothThresoldList.append(ss);
////		}
////	}
////
////	foreach(float smoothThresold , smoothThresoldList)
////	{
////		i_face=0;
////		for(int i=0;i<meshList.InstanceCount();i++)
////		{
////			Anima::MeshExt *meshExt = meshList.GetInstance(i);
////			for(int s=0;s<meshExt->GetSubInfoCount();s++)
////			{
////				Anima::MeshExtSubmeshInfo &info = meshExt->GetSubInfo(s);
////				if (info.smoothThresold == smoothThresold)
////				{
////					for(int t=info.startTri;t<= info.endTri;t++)
////						faceSel.Set(i_face+t);
////				}
////				else
////				{
////					for(int t=info.startTri;t<= info.endTri;t++)
////						faceSel.Clear(i_face+t);
////				}
////			}
////			i_face+=  meshExt->GetTriCount()/3;
////		}
////		mesh.AutoSmooth(smoothThresold,true);
////	}
////
////	for(int i=0;i<totalFaceCount;i++)
////		faceSel.Clear(i);

	
void MeshObjectSubMesh::EnsureSmooth()
{
	if(!smoothValid)
	{
		smoothValid=true;
		float smoothThresold=DegToRad(60);

		mesh.AutoSmooth(smoothThresold,FALSE,TRUE);

		///*int totalFaceCount=mesh.numFaces;

		//BitArray& faceSel = mesh.FaceSel();
		//for(int i=0;i<totalFaceCount;i++)
		//	faceSel.Set(i);

		//mesh.AutoSmooth(smoothThresold,1);

		//for(int i=0;i<totalFaceCount;i++)
		//	faceSel.Clear(i);*/
	}
}

void MeshObjectSubMesh::BuildMesh(TimeValue t)
{
	if (!initialized || MeshObject::meshObject == NULL) return;
	
//	MeshObject::meshObject->EnsureTime(t);

	//if(!build || model->IsBBoxMode() != bboxMode)
	if(!build)
	{
		if (meshExtModel != NULL)
		{
		//	Anima::ModelInstance *model = meshExtModel->GetModel();

		//	Anima::_LogDebug(QString("MeshObjectSubMesh BuildModel "),__FUNCTION__,__LINE__);

			BuildModel(NULL,NULL,t); // rifaccio
		}
		else
			BuildGeom(NULL,NULL,t); // rifaccio
	}
	else
	{
		//Anima::_LogDebug(QString("MeshObjectSubMesh Update "),__FUNCTION__,__LINE__);

		Update(t);
	}

	/*
	// visiblity
	Anima::MeshExt *meshExt = meshExtModel;
	if (meshExtModel!=NULL)
	{
		Anima::ModelInstance *mi = meshExtModel->GetModel();
		bool isInPlayTime =  TicksToSec(t) <= MeshObject::meshObject->endPlayTime;
		if (isInPlayTime && mi->GetGfx()->IsVisible())
		{
			//Anima::_LogDebug(QString("7"),__FUNCTION__,__LINE__);

			bool bboxMode=true;
			node->Hide(FALSE);
			if (mi->IsReady() || node->Selected ())
			{
				bboxMode = !( node->Selected ());
			//	visCount++;
			//bboxMode = !(visCount <= maxModelDetail || node->Selected ());
			}
			int b = bboxMode ? 1 : 0;
			if (node->GetBoxMode() != b  )
			{
				node->BoxMode(bboxMode);
				//this->Invalidate ();

				#ifndef MAX2012
					GetCOREInterface()->NodeInvalidateRect  ( node   );  
				#else
					node->InvalidateRect(lastUpdateTime);
				//	GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
				#endif
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
	*/
	// VIEW MODE
	//INode* node = FindNodeRef(this);
	//if (MeshObject::meshObject != NULL && node != NULL)
	//{
	//	bool bboxMode=true;
	//	Anima::ModelInstance *mi = meshExt->GetModel();
	//	if (mi->IsReady())
	//	{
	//		bboxMode = MeshObject::meshObject->AskBBOXMode(this);
	//	}
	//	node->BoxMode (bboxMode);
	//}
}

void MeshObjectSubMesh::BuildGeom(Anima::MeshExporter *_meshExporter, Anima::MeshExtGeomExt *_meshExt,TimeValue t)
{
	if (_meshExt!=NULL)
	{
		this->meshExtGeom=_meshExt;
		this->meshExporter=_meshExporter;
		ivalid = NEVER;
		initialized=true;
		//return;
	}

	double mScale = 1.0 / GetMasterScale(UNITS_METERS);

	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	Anima::MeshExt *meshExt = meshExtGeom;

	int vertexCount = meshExt->GetVertexCount(); 
	int vertexTxtCount = meshExt->GetTxtVertexCount(); 
	int faceCount = meshExt->GetTriCount()/3;
	float *vertices = meshExt->GetVertices();
	float *normals = meshExt->GetNormals();
	float *uvList = meshExt->GetUVList();

	int i_vertex=0;
	int i_txt_vertex=0;
	int i_face=0;

	mesh.setNumVerts(vertexCount);
	mesh.setNumTVerts(vertexTxtCount);
	mesh.setNumFaces(faceCount);
	mesh.setNumTVFaces(faceCount);
	mesh.setSmoothFlags(true);

	int c=0;
	int tc=0;
	float pos[3];
	float x,y;
	for(int i=0;i<vertexCount;i++,c+=3)
	{
		pos[0] = vertices[c] * mScale;
		pos[2] = vertices[c+1]* mScale ;	// y = z
		pos[1] = -vertices[c+2]* mScale ;	// z = -y
		mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);

		pos[0] = normals[c];
		pos[2] = normals[c+1];	// y = z
		pos[1] = -normals[c+2];	// z = -y
		mesh.setNormal(i_vertex + i ,Point3(pos[0],pos[1],pos[2]));

	}
	for(int i=0;i<vertexTxtCount;i++,c+=3)
	{
		x = uvList[tc++];
		y = 1-uvList[tc++];
		mesh.setTVert(i_txt_vertex + i,x,y,0);
	}

	int triCount = meshExt->GetTriCount()/3;
	size_t *triList  = meshExt->GetTriList();
	size_t *triUVList  = meshExt->GetUVTriList();
	size_t *triMatEndList  = meshExt->GetTriEndList();
	//size_t *tri_map_list = meshExt->GetTriMapList();
	size_t v[3];
	c=0;
	int matIdx=0;
	int endTriMat = triMatEndList[matIdx]/3;
	float valarray[2] = {0,0};
	BitArray& faceSel = mesh.FaceSel();

	int baseMat = meshExt->MaterialBaseNumber;
	//int baseMat = 0;
	int smoothMask=1;
	for(int t=0;t<triCount;t++)
	{
		if( t == 0 || t == endTriMat)
		{
			// cambio materiale
			if (t != 0)
			{
				endTriMat = triMatEndList[++matIdx]/3;
				smoothMask = 1 << matIdx;
			}
		}

		v[0] = i_vertex + triList[c+0];
		v[1] = i_vertex + triList[c+1];
		v[2] = i_vertex + triList[c+2];

		mesh.faces[i_face + t].setSmGroup(smoothMask);
		mesh.faces[i_face + t].setMatID(baseMat+matIdx);
		mesh.faces[i_face + t].setVerts(v[0],v[1],v[2]);

		v[0] = i_txt_vertex + triUVList[c+0];
		v[1] = i_txt_vertex + triUVList[c+1];
		v[2] = i_txt_vertex + triUVList[c+2];

		TVFace &tf = mesh.tvFace[i_face + t];
		tf.setTVerts(v[0],v[1],v[2]);	
	
		c+=3;
	}

	build=true;
	mesh.InvalidateTopologyCache();
}

void MeshObjectSubMesh::BuildModel(Anima::MeshExporter *_meshExporter, Anima::MeshExtModel *_meshExt,TimeValue t)
{
	//Anima::_LogDebug(QString("BuildModel %1").arg((long)_meshExt),__FUNCTION__,__LINE__);
	//Anima::_LogDebug(QString("BuildModel %1").arg((long)this->meshExtModel),__FUNCTION__,__LINE__);

	bool HaveSingleMaterial =  Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial();

	if (_meshExt!=NULL)
	{
		this->meshExtModel=_meshExt;
		this->meshExporter=_meshExporter;
		ivalid = NEVER;
		initialized=true;
		//return;
	}

	double mScale = 1.0 /GetMasterScale(UNITS_METERS);
	
	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	Anima::MeshExt *meshExt = meshExtModel;

	meshExt->UpdateTime( TicksToSec(t));
	INode* node = FindNodeRef(this);
	if( node==NULL) return;
	MeshObject::meshObject->SetVisibility(this,node,t);

	Anima::ModelInstance *model = meshExtModel->GetModel();
	Ogre::MeshPtr ogreMesh = meshExtModel->GetOgreMesh();

	if (true)//!bboxMode)
	{
		int vertexCount = meshExt->GetVertexCount(); 
		int vertexTxtCount = meshExt->GetTxtVertexCount(); 
		int faceCount = meshExt->GetTriCount()/3;
		float *vertices = meshExt->GetVertices();
		float *normals = meshExt->GetNormals();
		float *uvList = meshExt->GetUVList();

		Ogre::Vector3 modelPos = model->GetPosition() * mScale;
		INode* node = FindNodeRef(this);
		if( node==NULL) return;
		Point3 delta(modelPos.x,-modelPos.z,modelPos.y);
	
		Matrix3 ptm = Inverse(MeshObject::meshObject->animaNode->GetNodeTM(t)); 
		Matrix3 tmAxis = node->GetNodeTM(t);
	
		Matrix3 old_m = node->GetObjectTM (t) ;
		old_m = old_m * ptm; 

		Point3 old_p = old_m.GetTrans();
		node->Move(t, tmAxis, delta - old_p);

		int i_vertex=0;
		int i_txt_vertex=0;
		int i_face=0;

		mesh.setNumVerts(vertexCount);
		mesh.setNumTVerts(vertexTxtCount);
		mesh.setNumFaces(faceCount);
		mesh.setNumTVFaces(faceCount);
		mesh.setSmoothFlags(true);

		//MeshNormalSpec *pSpec = mesh.GetSpecifiedNormals();
		//if ((pSpec != NULL) && pSpec->GetNumFaces())
		//{
		//	int y=0;
		//}
		int c=0;
		int tc=0;
		float pos[3];
		float x,y;
		for(int i=0;i<vertexCount;i++,c+=3)
		{
			pos[0] = vertices[c]*mScale - delta.x;
			pos[2] = vertices[c+1]*mScale - delta.z;	// y = z
			pos[1] = -vertices[c+2]*mScale - delta.y;	// z = -y
			mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);

			pos[0] = normals[c];
			pos[2] = normals[c+1];	// y = z
			pos[1] = -normals[c+2];	// z = -y
			mesh.setNormal(i_vertex + i ,Point3(pos[0],pos[1],pos[2]));

		}
		for(int i=0;i<vertexTxtCount;i++,c+=3)
		{
			x = uvList[tc++];
			y = 1-uvList[tc++];
			mesh.setTVert(i_txt_vertex + i,x,y,0);
		}

		int triCount = meshExt->GetTriCount()/3;
		size_t *triList  = meshExt->GetTriList();
		size_t *triUVList  = meshExt->GetUVTriList();
		size_t *triMatEndList  = meshExt->GetTriEndList();
		size_t v[3];
		int matIdx=0;
		c=0;
		int endTriMat = triMatEndList[matIdx]/3;
		float valarray[2] = {0,0};
		BitArray& faceSel = mesh.FaceSel();

		Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();
		int modelIdx = meshList.meshList->indexOf(meshExtModel);

		//int baseMat = meshExporter->GetModelExportColl().GetInstanceMaterialNumber(_meshExt);
		int smoothMask=1;
		for(int t=0;t<triCount;t++)
		{
			if( t == 0 || t == endTriMat)
			{
				// cambio materiale
				if (t != 0)
				{
					endTriMat = triMatEndList[++matIdx]/3;
					smoothMask = 1 << matIdx;
				}
			}

			v[0] = i_vertex + triList[c+0];
			v[1] = i_vertex + triList[c+1];
			v[2] = i_vertex + triList[c+2];

			mesh.faces[i_face + t].setSmGroup(smoothMask);
			if (!HaveSingleMaterial)
				mesh.faces[i_face + t].setMatID(MeshObject::meshObject->GetMaterialSubIdx(modelIdx)+matIdx);
			else
				mesh.faces[i_face + t].setMatID(0);

			mesh.faces[i_face + t].setVerts(v[0],v[1],v[2]);

			v[0] = i_txt_vertex + triUVList[c+0];
			v[1] = i_txt_vertex + triUVList[c+1];
			v[2] = i_txt_vertex + triUVList[c+2];

			TVFace &tf = mesh.tvFace[i_face + t];
			tf.setTVerts(v[0],v[1],v[2]);	
		
			c+=3;
		}
	}

	build=true;
	mesh.InvalidateTopologyCache();
	mesh.EnableEdgeList(1);
	node->SetWireColor( RGB(255,255,255));
}

void MeshObjectSubMesh::Update(TimeValue t)
{
	//Anima::_LogDebug(QString("MeshObjectSubMesh start"),__FUNCTION__,__LINE__);
	if (MeshObject::meshObject==NULL) return;

	//mScale = 1.0 ;/// mScale;

	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	Anima::MeshExt *meshExt = meshExtModel;
	if (meshExtModel!=NULL)
	{
		Anima::ModelInstance *model = meshExtModel->GetModel();
	}
	else
	{
		meshExt = meshExtGeom;
	}

	double mScale = 1.0 / GetMasterScale(UNITS_METERS);
	
	if (meshExt == meshExtGeom)//!model->IsBBoxMode()) // vecchio sistema
	{
		meshExt->UpdateTime( TicksToSec(t));

	//	Ogre::MeshPtr ogreMesh = meshExt->GetOgreMesh();

		int vertexCount = meshExt->GetVertexCount(); 
		float *vertices = meshExt->GetVertices();

		int c=0;
		float pos[3];
		for(int i=0;i<vertexCount;i++)
		{
			pos[0] = vertices[c++]* mScale;
			pos[2] = vertices[c++]*mScale;	// y = z
			pos[1] = -vertices[c++]*mScale;	// z = -y
		
			//Point3 p = mesh.getVert(i);
			mesh.setVert( i,pos[0],pos[1],pos[2]);
		}
	}
	else
	{	
		meshExt->UpdateTime( TicksToSec(t));
		
		// TODO disattivare flag
		Anima::ModelInstance *model = meshExtModel->GetModel();
		Ogre::Vector3 modelPos = meshExt->GetPosition()*mScale;
		INode* node = FindNodeRef(this);
		if( node==NULL) return;

		MeshObject::meshObject->SetVisibility(this,node,t);

	//	Point3 delta(modelPos.x * mScale,-modelPos.z * mScale,modelPos.y * mScale);
		Point3 delta(modelPos.x ,-modelPos.z ,modelPos.y );
		// posizione non assoluta ma relativa al nodo di root

		Matrix3 ptm = Inverse(MeshObject::meshObject->animaNode->GetNodeTM(t)); 
		Matrix3 tmAxis = node->GetNodeTM(t);
	
		Matrix3 old_m = node->GetObjectTM (t) ;
		old_m = old_m * ptm; 

		Point3 old_p = old_m.GetTrans();

		node->Move(t, tmAxis, delta - old_p);
		//node->Move(t, tmAxis, delta - old_p);

		// ----

		int vertexCount = meshExt->GetVertexCount(); 
		float *vertices = meshExt->GetVertices();

		int c=0;
		float pos[3];
		for(int i=0;i<vertexCount;i++)
		{
			pos[0] = vertices[c++]*mScale - delta.x;
			pos[2] = vertices[c++]*mScale - delta.z;	// y = z
			pos[1] = -vertices[c++]*mScale- delta.y;	// z = -y
		
			//Point3 p = mesh.getVert(i);
			mesh.setVert( i,pos[0] ,pos[1],pos[2]);
		}
	}
	/*else
	{
		Ogre::AxisAlignedBox bbox = model->GetModel()->GetBound()->GetBBox();
		Ogre::Vector3 pos = model->GetPosition();
		Ogre::Vector3 pmin = bbox.getMinimum();
		Ogre::Vector3 pmax = bbox.getMaximum();
		pmin = Ogre::Vector3 (pmin.x+pos.x,pmin.z-pos.z,pmin.y+pos.y);
		pmax = Ogre::Vector3 (pmax.x+pos.x,pmax.z-pos.z,pmax.y+pos.y);

		mesh.setVert(0, Point3(pmin.x, pmin.y, pmin.z));
		mesh.setVert(1, Point3(pmax.x, pmin.y, pmin.z));
		mesh.setVert(2, Point3(pmin.x, pmax.y, pmin.z));
		mesh.setVert(3, Point3(pmax.x, pmax.y, pmin.z));
		mesh.setVert(4, Point3(pmin.x, pmin.y, pmax.z));
		mesh.setVert(5, Point3(pmax.x, pmin.y, pmax.z));
		mesh.setVert(6, Point3(pmin.x, pmax.y, pmax.z));
		mesh.setVert(7, Point3(pmax.x, pmax.y, pmax.z));
	}*/

	mesh.InvalidateTopologyCache();

	//INode* node = FindNodeRef(this);
	////node->BoxMode (bboxMode);
	//if (model->IsReady())
	//	node->SetWireColor( RGB(255,255,255));
	//else
	//	node->SetWireColor( RGB(255,0,0));

	//Anima::_LogDebug(QString("MeshObjectSubMesh end"),__FUNCTION__,__LINE__);
}
void MeshObjectSubMesh::GetInfo(bool &boxMode,int &vertices,int &tri)
{
	if (meshExtModel!=NULL)
	{
		INode* node = FindNodeRef(this);
		if (node!=NULL)
		{
			boxMode = node->GetBoxMode() == 1;
			vertices= mesh.getNumVerts();
			tri= mesh.getNumFaces();
		}
		else
		{
			boxMode=false;
			vertices=0;
			tri=0;
		}
	}
	else
	{
		boxMode=false;
		vertices=0;
		tri=0;
	}
}

// =============================================

extern MeshObjectClassDesc *_meshObjectClassDesc;

void MeshObjectSubMesh::BeginEditParams(IObjParam *ip,ULONG flags,Animatable *prev)
{
	SimpleObject::BeginEditParams(ip,flags,prev);
	_meshObjectClassDesc->BeginEditParams(ip, MeshObject::meshObject, flags, prev);
}
		
void MeshObjectSubMesh::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{		
	SimpleObject::EndEditParams(ip,flags,next);
	_meshObjectClassDesc->EndEditParams(ip,  MeshObject::meshObject, flags, next);
}