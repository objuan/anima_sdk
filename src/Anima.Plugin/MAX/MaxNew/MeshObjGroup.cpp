#include "stdafx.h"
#include "resource.h"

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
#include "UI/GUIManager.h"
#include "Core/ResourceManager.h"
#include "Gfx/GfxContext.h"
#include "Gfx/GfxBaseModel.h"
#include "Import/IO/ContextIO.h"
#include "Export/MeshExporter.h"
#include "Export/ParamsExporter.h"
#include "Export/MeshExt.h"

//int MeshObject::NumberOfRenderMeshes  (    ) 
//{
//	return 1;
//}
//		
//Mesh* MeshObject::GetMultipleRenderMesh  (  TimeValue  t,  INode *  inode,  View &  view,  BOOL &  needDelete,  int  meshNumber  ) 
//{
//	needDelete =false;
//	return &mesh;
//}
//
//void MeshObject::BuildMesh(TimeValue t)
//{
//	//if (buildMode == 1)
//	//	BuildMeshBox(t);
//	//else if (buildMode == 0)
//		//BuildMeshFull(t);
//
//}

void MeshObject::BuildNormals()
{
	//	mesh.buildNormals();

	Anima::_LogDebug(QString("BuildNormals 1"),__FUNCTION__,__LINE__);

	//// normals
	BitArray& faceSel = mesh.FaceSel();

	Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();
	// 'smoothThresold' list 
	QList<float> smoothThresoldList;
	for(int i=0;i<meshList.InstanceCount();i++)
	{
		Anima::MeshExt *meshExt = meshList.GetInstance(i);
		for(int s=0;s<meshExt->GetSubInfoCount();s++)
		{
			float ss=(89.0f / 180.0f) * 3.1415f;
			if ( meshExt->GetSubInfo(s).IsBody())
				ss = (60.0f / 180.0f) * 3.1415f;
			meshExt->GetSubInfo(s).smoothThresold = ss;

			if (!smoothThresoldList.contains(ss)) 
				smoothThresoldList.append(ss);
		}
	}

		Anima::_LogDebug(QString("BuildNormals 2"),__FUNCTION__,__LINE__);

	foreach(float smoothThresold , smoothThresoldList)
	{
		i_face=0;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::MeshExt *meshExt = meshList.GetInstance(i);
			for(int s=0;s<meshExt->GetSubInfoCount();s++)
			{
				Anima::MeshExtSubmeshInfo &info = meshExt->GetSubInfo(s);
				if (info.smoothThresold == smoothThresold)
				{
					for(int t=info.startTri;t<= info.endTri;t++)
						faceSel.Set(i_face+t);
				}
				else
				{
					for(int t=info.startTri;t<= info.endTri;t++)
						faceSel.Clear(i_face+t);
				}
			}
			i_face+=  meshExt->GetTriCount()/3;
		}
		mesh.AutoSmooth(smoothThresold,true);
	}
	Anima::_LogDebug(QString("BuildNormals 3"),__FUNCTION__,__LINE__);

	for(int i=0;i<totalFaceCount;i++)
		faceSel.Clear(i);

		Anima::_LogDebug(QString("BuildNormals 4"),__FUNCTION__,__LINE__);

	
}

void MeshObject::BuildMeshFull(TimeValue t)
{
	double sec = TicksToSec(t);
	if (oldTime == sec) return;
	oldTime = sec;

	Anima::_LogDebug(QString("BuildMesh start %1").arg(sec),__FUNCTION__,__LINE__);

	int updateMode; // 0 = rendering , 1 = manually
	if (pblock2 != NULL)
		pblock2->GetValue( anima_update_mode, t, updateMode, ivalid );
	else
		updateMode=0;

	//if (meshExporter!=NULL) delete meshExporter;

	/* float a;
	 pblock2->GetValue(anima_specular_head,t,a,ivalid);*/

	//float smoothHead=0;
	//pblock->GetValue(PB_SMOOTH_HEAD, t, smoothHead, ivalid);

	Anima::Film &film = Anima::Context::Singleton.GetFilm();
	film.SetFPS(true,30); // TODO
	film.SetCurrentFrame(sec * film.GetFPS());

	
	Anima::_LogDebug(QString("BuildMesh 1 "),__FUNCTION__,__LINE__);

	Anima::GfxContext::Singleton.UpdateAndWaitTask();

	Anima::_LogDebug(QString("BuildMesh 2"),__FUNCTION__,__LINE__);

	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	if (invalidate)
	{
		// rifaccio tutto

		Anima::_LogDebug(QString("BuildMesh invalid"),__FUNCTION__,__LINE__);

		if (meshExporter!=NULL) delete meshExporter;
		meshExporter = new Anima::MeshExporter();
		meshExporter->BuildCurrentFrame();

		Anima::_LogDebug(QString("BuildMesh invalid 1"),__FUNCTION__,__LINE__);

		invalidate=false;
		int family;
		float fp, fq, radius, scale_axis[3];
		int axis = 0, vts;
		int genUVs = TRUE;
		
		Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();

		i_vertex=0;
		i_txt_vertex=0;
		i_face=0;
		totalVertexCount=0;
		totalFaceCount=0;
		totalTxtVertexCount=0;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::MeshExt *meshExt = meshList.GetInstance(i);
			totalVertexCount+= meshExt->GetVertexCount();
			totalTxtVertexCount += meshExt->GetTxtVertexCount();
			totalFaceCount+= meshExt->GetTriCount()/3;
		}

		Anima::_LogDebug(QString("BuildMesh invalid 2"),__FUNCTION__,__LINE__);

		mesh.setNumVerts(totalVertexCount);
		mesh.setNumTVerts(totalTxtVertexCount);
		mesh.setNumFaces(totalFaceCount);
		mesh.setNumTVFaces(totalFaceCount);
		mesh.setSmoothFlags(true);

		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::MeshExt *meshExt = meshList.GetInstance(i);
			AddModel(meshExt);
			i_vertex+=  meshExt->GetVertexCount();
			i_txt_vertex += meshExt->GetTxtVertexCount();
			i_face+=  meshExt->GetTriCount()/3;
		}

		Anima::_LogDebug(QString("BuildMesh invalid 3"),__FUNCTION__,__LINE__);

		// ini 
		Anima::Context::Singleton.ForceUIUpdate();

		/*Anima::GfxContext::Singleton.OnResize();
		Anima::GfxContext::Singleton.Update();*/

		Anima::_LogDebug(QString("BuildMesh invalid 4"),__FUNCTION__,__LINE__);

		//BuildNormals();

		delete meshExporter;
		meshExporter=NULL;

		Anima::_LogDebug(QString("BuildMesh invalid end"),__FUNCTION__,__LINE__);


	}
	else
	{
		if (meshExporter == NULL)
		{
			meshExporter = new Anima::MeshExporter();
			meshExporter->BuildCurrentFrame();
		}

		if (meshExporter != NULL)
		{
			i_vertex=0;
			i_txt_vertex=0;
			i_face=0;
			meshExporter->UpdateCurrentFrame();
			Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();
			for(int i=0;i<meshList.InstanceCount();i++)
			{
				Anima::MeshExt *meshExt = meshList.GetInstance(i);
				UpdateModelFull(meshExt);
				i_vertex+=  meshExt->GetVertexCount();
				i_txt_vertex += meshExt->GetTxtVertexCount();
				i_face+=  meshExt->GetTriCount()/3;
			}
		}
	}
	mesh.InvalidateTopologyCache();

	Anima::_LogDebug(QString("BuildMesh end"),__FUNCTION__,__LINE__);
}

void MeshObject::AddModel(Anima::MeshExt *meshExt)
{
	Anima::ModelInstance *model = meshExt->GetModel();
	Ogre::MeshPtr ogreMesh = meshExt->GetOgreMesh();

	int vertexCount = meshExt->GetVertexCount(); 
	int vertexTxtCount = meshExt->GetTxtVertexCount(); 
	float *vertices = meshExt->GetVertices();
	float *normals = meshExt->GetNormals();
	float *uvList = meshExt->GetUVList();

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
		pos[0] = vertices[c];
		pos[2] = vertices[c+1];	// y = z
		pos[1] = -vertices[c+2];	// z = -y
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

	int baseMat = meshExporter->GetModelExportColl().GetMaterialNumber(meshExt);
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
}

void MeshObject::UpdateModelFull(Anima::MeshExt *meshExt)
{
	Anima::_LogDebug(QString("UpdateModel start"),__FUNCTION__,__LINE__);

	Anima::ModelInstance *model = meshExt->GetModel();
	Ogre::MeshPtr ogreMesh = meshExt->GetOgreMesh();

	int vertexCount = meshExt->GetVertexCount(); 
	float *vertices = meshExt->GetVertices();

	int c=0;
	float pos[3];
	for(int i=0;i<vertexCount;i++)
	{
		pos[0] = vertices[c++];
		pos[2] = vertices[c++];	// y = z
		pos[1] = -vertices[c++];	// z = -y
	
		mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);
	}
	Anima::_LogDebug(QString("UpdateModel end"),__FUNCTION__,__LINE__);
}



void MeshObject::BuildMeshBox(TimeValue t)
{
	double sec = TicksToSec(t);
	if (oldTime == sec) return;
	oldTime = sec;

	Anima::_LogDebug(QString("BuildMeshBox start %1").arg(sec),__FUNCTION__,__LINE__);

	int updateMode; // 0 = rendering , 1 = manually
	if (pblock2 != NULL)
		pblock2->GetValue( anima_update_mode, t, updateMode, ivalid );
	else
		updateMode=0;

	Anima::Film &film = Anima::Context::Singleton.GetFilm();
	film.SetFPS(true,30); // TODO
	film.SetCurrentFrame(sec * film.GetFPS());

	Anima::GfxContext::Singleton.UpdateAndWaitTask();

	ivalid.SetStart(t);// = FOREVER;
	ivalid.SetEnd(t);

	if (invalidate)
	{
		// rifaccio tutto

		if (meshExporter!=NULL) delete meshExporter;
		meshExporter = new Anima::MeshExporter();
		meshExporter->BuildCurrentFrame();

		invalidate=false;
		int family;
		float fp, fq, radius, scale_axis[3];
		int axis = 0, vts;
		int genUVs = TRUE;
		
		Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();

		i_vertex=0;
		i_txt_vertex=0;
		i_face=0;
		totalVertexCount=0;
		totalFaceCount=0;
		totalTxtVertexCount=0;
		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::MeshExt *meshExt = meshList.GetInstance(i);
			totalVertexCount+= meshExt->GetVertexCount();
			totalTxtVertexCount += meshExt->GetTxtVertexCount();
			totalFaceCount+= meshExt->GetTriCount()/3;
		}

		mesh.setNumVerts(totalVertexCount);
		mesh.setNumTVerts(totalTxtVertexCount);
		mesh.setNumFaces(totalFaceCount);
		mesh.setNumTVFaces(totalFaceCount);
		mesh.setSmoothFlags(true);

		for(int i=0;i<meshList.InstanceCount();i++)
		{
			Anima::MeshExt *meshExt = meshList.GetInstance(i);
			AddModel(meshExt);
			i_vertex+=  meshExt->GetVertexCount();
			i_txt_vertex += meshExt->GetTxtVertexCount();
			i_face+=  meshExt->GetTriCount()/3;
		}

		// ini 
		Anima::Context::Singleton.ForceUIUpdate();

		/*Anima::GfxContext::Singleton.OnResize();
		Anima::GfxContext::Singleton.Update();*/


		//BuildNormals();

		delete meshExporter;
		meshExporter=NULL;


	}
	else
	{
		if (meshExporter == NULL)
		{
			meshExporter = new Anima::MeshExporter();
			meshExporter->BuildCurrentFrame();
		}

		if (meshExporter != NULL)
		{
			i_vertex=0;
			i_txt_vertex=0;
			i_face=0;
			meshExporter->UpdateCurrentFrame();
			Anima::MeshExtCollection &meshList = meshExporter->GetModelExportColl();
			for(int i=0;i<meshList.InstanceCount();i++)
			{
				Anima::MeshExt *meshExt = meshList.GetInstance(i);
				UpdateModelGroup(meshExt);
				i_vertex+=  meshExt->GetVertexCount();
				i_txt_vertex += meshExt->GetTxtVertexCount();
				i_face+=  meshExt->GetTriCount()/3;
			}
		}
	}
	mesh.InvalidateTopologyCache();

	Anima::_LogDebug(QString("BuildMesh end"),__FUNCTION__,__LINE__);
}

void MeshObject::UpdateModelGroup(Anima::MeshExt *meshExt)
{
	Anima::_LogDebug(QString("UpdateModel start"),__FUNCTION__,__LINE__);

	Anima::ModelInstance *model = meshExt->GetModel();
	Ogre::MeshPtr ogreMesh = meshExt->GetOgreMesh();

	int vertexCount = meshExt->GetVertexCount(); 
	float *vertices = meshExt->GetVertices();

	int c=0;
	float pos[3];
	for(int i=0;i<vertexCount;i++)
	{
		pos[0] = vertices[c++];
		pos[2] = vertices[c++];	// y = z
		pos[1] = -vertices[c++];	// z = -y
	
		mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);
	}
	Anima::_LogDebug(QString("UpdateModel end"),__FUNCTION__,__LINE__);
}
