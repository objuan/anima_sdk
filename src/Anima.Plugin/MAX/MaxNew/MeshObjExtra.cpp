#include "stdafx.h"
#include "resource.h"

#include "MeshObj.h"
#include "MeshObjExtra.h"
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

void MeshObjectGeomMesh::BuildMesh(TimeValue t)
{
}

// =============================================

extern MeshObjectClassDesc *_meshObjectClassDesc;

// ----------------

void *MeshObjectGeomClassDesc::Create(BOOL loading)
{
	return new MeshObjectGeomMesh();
}

void MeshObjectGeomMesh::BeginEditParams(IObjParam *ip,ULONG flags,Animatable *prev)
{
	SimpleObject2::BeginEditParams(ip,flags,prev);
	_meshObjectClassDesc->BeginEditParams(ip, MeshObject::meshObject, flags, prev);
}
		
void MeshObjectGeomMesh::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{		
	SimpleObject2::EndEditParams(ip,flags,next);
	_meshObjectClassDesc->EndEditParams(ip,  MeshObject::meshObject, flags, next);
}

CreateMouseCallBack* MeshObjectGeomMesh::GetCreateMouseCallBack() 
{
	return NULL;
}

void *MeshObjectDummyClassDesc::Create(BOOL loading)
{
	return new MeshObjectDummyMesh();
}

void MeshObjectDummyMesh::InitNodeName (TSTR &s)
{
	s = "AnimaDummy";
}

void MeshObjectDummyMesh::BeginEditParams(IObjParam *ip,ULONG flags,Animatable *prev)
{
	DummyObject::BeginEditParams(ip,flags,prev);
	_meshObjectClassDesc->BeginEditParams(ip, MeshObject::meshObject, flags, prev);
}
		
void MeshObjectDummyMesh::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{		
	DummyObject::EndEditParams(ip,flags,next);
	_meshObjectClassDesc->EndEditParams(ip,  MeshObject::meshObject, flags, next);
}

CreateMouseCallBack* MeshObjectDummyMesh::GetCreateMouseCallBack() 
{
	return NULL;
}
RefResult MeshObjectDummyMesh::NotifyRefChanged  (  Interval  iv,  RefTargetHandle  hTarg,  PartID &  partID,   RefMessage  msg ) 
{
	return REF_SUCCEED ;
}
