#include "AnimaIncludes.h"
#include "AnimaMeshObject.h"
#include "AnimaObject.h"
#include "c4d.h"
#include "c4d_symbols.h"
#include "Utils.h"
#include "oanima.h"

#ifndef C4D_R13
String LongToString(Int32 a){
	return String(QString("%1").arg(a).toAscii().data());
}
#endif

AnimaMeshObject::AnimaMeshObject() : m_lastTime(MAXREALs)
{
	m_obj = NULL;
	meshExtModel = NULL;
	meshExtGeom = NULL;
	m_lastShowBBox = FALSE;
	m_lastOverrideMat = NULL;
}

AnimaMeshObject::~AnimaMeshObject()
{
	PolygonObject::Free(m_obj);
}

Bool AnimaMeshObject::Init(GeListNode *node)
{
	BaseObject		*op		= (BaseObject*)node;
	return TRUE;
}

Bool AnimaMeshObject::Message(GeListNode *node, Int32 type, void *t_data)
{
	return TRUE;
}
void AnimaMeshObject::setScaleFactor(Real r)
{
	m_unitScaleFactor = r;
}

BaseObject* createBoundingBox(const Vector& mp, const Vector& rad, const Matrix& mg)
{
	PolygonObject* bbox = PolygonObject::Alloc(8, 6);
	if(!bbox) return NULL;
	bbox->SetMg(mg);
	Vector* points = bbox->GetPointW();
	CPolygon* polys = bbox->GetPolygonW();
	points[0].x = mp.x-rad.x;
	points[0].y = mp.y-rad.y;
	points[0].z = mp.z-rad.z;

	points[1].x = mp.x-rad.x;
	points[1].y = mp.y-rad.y;
	points[1].z = mp.z+rad.z;

	points[2].x = mp.x+rad.x;
	points[2].y = mp.y-rad.y;
	points[2].z = mp.z+rad.z;

	points[3].x = mp.x+rad.x;
	points[3].y = mp.y-rad.y;
	points[3].z = mp.z-rad.z;

	points[4].x = mp.x-rad.x;
	points[4].y = mp.y+rad.y;
	points[4].z = mp.z-rad.z;

	points[5].x = mp.x-rad.x;
	points[5].y = mp.y+rad.y;
	points[5].z = mp.z+rad.z;

	points[6].x = mp.x+rad.x;
	points[6].y = mp.y+rad.y;
	points[6].z = mp.z+rad.z;

	points[7].x = mp.x+rad.x;
	points[7].y = mp.y+rad.y;
	points[7].z = mp.z-rad.z;

	polys[0] = CPolygon(0,1,2,3);
	polys[1] = CPolygon(0,4,5,1);
	polys[2] = CPolygon(1,5,6,2);
	polys[3] = CPolygon(2,6,7,3);
	polys[4] = CPolygon(0,4,7,3);
	polys[5] = CPolygon(4,5,6,7);
	bbox->Message(MSG_UPDATE);
	return bbox;
}

void setOverrideMat(BaseObject* obj, BaseMaterial* mat)
{
	if(!obj) return;
	TextureTag* tex = NULL;
	Int32 i=0;
	while(tex = (TextureTag*) obj->GetTag(Ttexture, i))
	{
		Utils::addLog(tex->GetName());
		if(tex->GetName().Compare(OVERRIDE_MAT_TAGNAME) == 0)
		{
			if(mat)
			{
				tex->SetMaterial(mat);
				tex->SetParameter(TEXTURETAG_RESTRICTION, GeData(""), DESCFLAGS_SET_0);
				Utils::addLog("ja");
			}
			else
			{
				tex->SetMaterial(mat);
				tex->SetParameter(TEXTURETAG_RESTRICTION, GeData(OVERRIDE_MAT_TAGNAME), DESCFLAGS_SET_0);
				Utils::addLog("nein");
			}
		}
		i++;
	}
}
BaseObject* AnimaMeshObject::GetVirtualObjects(BaseObject *op, HierarchyHelp *hh)
{
	if(m_obj && hh->GetDocument() && hh->GetDocument()->GetTime().Get() < AnimaObject::g_endTime)
	{
		Bool bShowBBox = FALSE;
		AnimaObject* parent = NULL;
		BaseMaterial* overrideMat = NULL;
		Int32 actorOrMesh = meshExtModel ? 1 : 0;

		Bool bIsRender = FALSE;
#if API_VERSION < 12000
		if(hh->GetBuildFlags() & BUILDFLAGS_INTERNALRENDERER || hh->GetBuildFlags() & BUILDFLAGS_EXTERNALRENDERER)
			bIsRender=TRUE;
#else
		if(hh->GetBuildFlags() == BUILDFLAGS_INTERNALRENDERER || hh->GetBuildFlags() == BUILDFLAGS_EXTERNALRENDERER)
			bIsRender=TRUE;
#endif

		if(op->GetCacheParent() && op->GetCacheParent()->GetType() == ID_ANIMAOBJECT)
		{
			overrideMat = (BaseMaterial*)op->GetCacheParent()->GetDataInstance()->GetLink(ANIMAOBJECT_MATERIAL_OVERRIDE, hh->GetDocument(), Mbase);
			parent = (AnimaObject*)op->GetCacheParent()->GetNodeData();
			if(meshExtModel && !bIsRender)
				bShowBBox = parent->hasToShowBBox();
		}
		if(m_bCached)
		{
			if(hh->GetDocument()->GetTime() == m_lastTime && (bShowBBox == m_lastShowBBox) && !bIsRender)
			{
				if(bShowBBox && parent)
					parent->AddInfos(0, 0, m_obj->GetPolygonCount(), 0, m_obj->GetPointCount(), 0);
				else if(parent)
				{
					if(op->GetCache(hh) && op->GetCache(hh)->GetRenderMode() == MODE_OFF)
						parent->AddInfos(actorOrMesh, 0, m_obj->GetPolygonCount(), 0, m_obj->GetPointCount(), 0);
					else
						parent->AddInfos(actorOrMesh, actorOrMesh, m_obj->GetPolygonCount(), m_obj->GetPolygonCount(), m_obj->GetPointCount(), m_obj->GetPointCount());
				}

				if(overrideMat != m_lastOverrideMat)
				{
					setOverrideMat(m_obj, overrideMat);
					m_lastOverrideMat = overrideMat;
					return (BaseObject*) m_obj->GetClone(COPYFLAGS_0, 0);
				}
				return op->GetCache(hh);
			}
			else
			{
				UpdateMesh(hh->GetDocument()->GetTime());
			}
		}
		m_lastShowBBox = bShowBBox;
		m_bCached = TRUE;
		if(bShowBBox)
		{
			if(meshExtModel && meshExtModel->GetModel() && !meshExtModel->GetModel()->GetGfx()->IsVisible())
				return BaseObject::Alloc(Onull);
			if(parent)
				parent->AddInfos(actorOrMesh, 0, m_obj->GetPolygonCount(), 0, m_obj->GetPointCount(), 0);
			return createBoundingBox(m_obj->GetMp(), m_obj->GetRad(), m_obj->GetMg());
		}


		if(overrideMat != m_lastOverrideMat)
		{
			setOverrideMat(m_obj, overrideMat);
			m_lastOverrideMat = overrideMat;
		}
		BaseObject* obj = (BaseObject*)m_obj->GetClone(COPYFLAGS_0, 0);
		m_lastTime = hh->GetDocument()->GetTime();
		Bool bHidden = FALSE;
		if(meshExtModel)
		{
			Anima::ModelInstance *model = meshExtModel->GetModel();
			if(model && !model->GetGfx()->IsVisible())
			{
				obj->SetEditorMode(MODE_OFF);
				obj->SetRenderMode(MODE_OFF);
				bHidden=TRUE;
			}
		}
		if(parent)
		{
			if(bHidden)
				parent->AddInfos(actorOrMesh, 0, m_obj->GetPolygonCount(), 0, m_obj->GetPointCount(), 0);
			else
				parent->AddInfos(actorOrMesh, actorOrMesh, m_obj->GetPolygonCount(), m_obj->GetPolygonCount(), m_obj->GetPointCount(), m_obj->GetPointCount());
		}		

		return obj;
	}
	return BaseObject::Alloc(Onull);
}

Bool AnimaMeshObject::CopyTo(NodeData* dest, GeListNode* snode, GeListNode* dnode, COPYFLAGS flags, AliasTrans* trn)
{
	if(this->m_obj)
	{
		PolygonObject::Free(((AnimaMeshObject*)dest)->m_obj);
		((AnimaMeshObject*)dest)->m_obj = (PolygonObject*) this->m_obj->GetClone(flags, trn);
	}
	((AnimaMeshObject*)dest)->meshExtModel =  this->meshExtModel;
	((AnimaMeshObject*)dest)->meshExtGeom =  this->meshExtGeom;
	((AnimaMeshObject*)dest)->m_unitScaleFactor =  this->m_unitScaleFactor;
	return TRUE;
}
void AnimaMeshObject::UpdateMesh(BaseTime t)
{
	Anima::MeshExt *meshExt = this->meshExtModel;
	if(!meshExt)
		meshExt = this->meshExtGeom;
	if(!meshExt)
	{
		Utils::addLog("AnimaMeshObject::UpdateMesh:: meshext not found");
		return;
	}
	if (meshExt == meshExtGeom)
	{
		meshExt->UpdateTime(t.Get());

		Int32 vertexCount = meshExt->GetVertexCount(); 
		SReal *vertices = meshExt->GetVertices();

		CPolygon* polys = m_obj->GetPolygonW();
		Vector* points = m_obj->GetPointW();

		Int32 i_vertex=0;
		Int32 i_txt_vertex=0;
		Int32 i_face=0;

		int c=0;
		int tc=0;
		float x,y,z;
		for(int i=0;i<vertexCount;i++)
		{
			x = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor;
			y = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor;	// y = z
			z = -vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor;	// z = -y
			points[i] = Vector(x, y, z);

//			pos[0] = normals[c];
//			pos[2] = normals[c+1];	// y = z
//			pos[1] = -normals[c+2];	// z = -y
//			mesh.setNormal(i_vertex + i ,Point3(pos[0],pos[1],pos[2]));
		}
	}
	else
	{	
		meshExt->UpdateTime(t.Get());
	
		Anima::ModelInstance *model = meshExtModel->GetModel();
		Vector modelPos = Utils::toC4DVector(model->GetPosition()) * IMPORTS_SCALE_FACTOR * m_unitScaleFactor;
		modelPos.z = -modelPos.z;
		Vector delta(modelPos.x,modelPos.z,modelPos.y);
		m_obj->SetAbsPos(modelPos);

		// ----

		Int32 vertexCount = meshExt->GetVertexCount(); 
		SReal *vertices = meshExt->GetVertices();
		Vector* points = m_obj->GetPointW();

		Int32 c=0;
		SReal pos[3];
		SReal x,y,z;
		for(Int32 i=0;i<vertexCount;i++)
		{
			x = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.x;
			y = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.z;	// y = z
			z = -vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.y;	// z = -y
			points[i] = Vector(x, y, z);
		}
	}




	m_obj->Message(MSG_UPDATE);
}

BaseMaterial* getMaterialById(Int32 matId, BaseDocument* doc)
{
	if(doc)
	{
		String matName = ANIMA_MAT_PRE+LongToString(matId);
		if(Anima::Context::Singleton.GetPresetsExporterManager()->HaveSingleMaterial())
		{
			matName = ANIMA_MAT_SINGLE;
		}
		return doc->SearchMaterial(matName);
	}
	return NULL;
}
void AnimaMeshObject::buildModel(Anima::MeshExporter* meshExporter, Anima::MeshExtModel* meshExtModel, BaseTime t, BaseDocument* doc)
{
	if(meshExtModel)
	{
		this->meshExtModel = meshExtModel;
		this->meshExtGeom = NULL;

		Anima::MeshExt *meshExt = meshExtModel;
		Anima::ModelInstance *model = meshExtModel->GetModel();
		Ogre::MeshPtr ogreMesh = meshExtModel->GetOgreMesh();
		meshExtModel->UpdateTime(t.Get());

		Int32 vertexCount = meshExt->GetVertexCount(); 
		Int32 vertexTxtCount = meshExt->GetTxtVertexCount();
		Int32 faceCount = meshExt->GetTriCount()/3;
		SReal *vertices = meshExt->GetVertices();
		SReal *normals = meshExt->GetNormals();
		SReal *uvList = meshExt->GetUVList();
		size_t *uvTriList = meshExt->GetUVTriList();

		Vector modelPos = Utils::toC4DVector(model->GetPosition()) * IMPORTS_SCALE_FACTOR;
		modelPos.z = -modelPos.z;
		Vector delta(modelPos.x,modelPos.z,modelPos.y);

		PolygonObject* obj = PolygonObject::Alloc(vertexCount, faceCount);
		if(!obj) return;
//		VariableTag* normalsTag = obj->MakeVariableTag(Tnormal, faceCount, NULL);
//		if(!normalsTag) return;
//		WORD* normalsVal = normalsTag->GetDataAddressW();
		BaseTag* phong = obj->MakeTag(Tphong);
		phong->SetParameter(DescID(PHONGTAG_PHONG_ANGLELIMIT), GeData(TRUE), DESCFLAGS_SET_0);
		phong->SetParameter(DescID(PHONGTAG_PHONG_ANGLE), GeData(1.396263), DESCFLAGS_SET_0);
		phong->SetParameter(DescID(PHONGTAG_PHONG_USEEDGES), GeData(TRUE), DESCFLAGS_SET_0);
		UVWTag* uvwTag = UVWTag::Alloc(faceCount);
		if(!uvwTag) return;
		obj->InsertTag(uvwTag);
		UVWHandle uvwHandle=uvwTag->GetDataAddressW();

//		GePrint("    model: " + LongToString(faceCount) + " faces, " + LongToString(vertexCount) + " vertices, " + LongToString(vertexTxtCount) + " uvs");
		obj->SetAbsPos(modelPos);
		CPolygon* polys = obj->GetPolygonW();
		Vector* points = obj->GetPointW();

		int c=0;
		int tc=0;
		float x,y,z;
		for(int i=0;i<vertexCount;i++)
		{
			x = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.x;
			y = vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.z;	// y = z
			z = -vertices[c++]*IMPORTS_SCALE_FACTOR * m_unitScaleFactor - delta.y;	// z = -y
			//mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);
			points[i] = Vector(x, y, z);
//			GePrint(RealToString(x) + " " + RealToString(y) + " " + RealToString(z));
//			pos[0] = normals[c];
//			pos[2] = normals[c+1];	// y = z
//			pos[1] = -normals[c+2];	// z = -y
//			mesh.setNormal(i_vertex + i ,Point3(pos[0],pos[1],pos[2]));
		}

		SReal ax, ay, bx, by, cx, cy;
		for(int i=0;i<faceCount;i++)
		{
			ax = uvList[2*uvTriList[3*i+0]+0];
			ay = uvList[2*uvTriList[3*i+0]+1];
			bx = uvList[2*uvTriList[3*i+1]+0];
			by = uvList[2*uvTriList[3*i+1]+1];
			cx = uvList[2*uvTriList[3*i+2]+0];
			cy = uvList[2*uvTriList[3*i+2]+1];
#if API_VERSION < 11500
			uvwTag->Set(i, UVWStruct(Vector(ax, ay, 0), Vector(cx, cy, 0), Vector(bx, by, 0))); //flip c and b cause normals are inverted
#else
			UVWTag::Set(uvwHandle, i, UVWStruct(Vector(ax, ay, 0), Vector(cx, cy, 0), Vector(bx, by, 0))); //flip c and b cause normals are inverted
#endif
		}

		uvwTag->Message(MSG_UPDATE);

		int triCount = meshExt->GetTriCount()/3;
		size_t *triList  = meshExt->GetTriList();
		size_t *triMatEndList  = meshExt->GetTriEndList();
		int i_vertex = 0;
		size_t v[3];
		c=0;
		Int32 matIdx = 0;
		Int32 baseMat = meshExporter->GetModelExportColl().GetInstanceMaterialNumber(meshExtModel);
		SelectionTag* selTag = (SelectionTag*)obj->MakeTag(Tpolygonselection);
		if(!selTag) return;
		selTag->SetName(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
		BaseSelect* polySelection = selTag->GetBaseSelect();
		int endTriMat = triMatEndList[matIdx]/3;

	//Add override MaterialTag
		TextureTag* texTag = (TextureTag*)obj->MakeTag(Ttexture);
		if(!texTag) return;
		GeData gdata;
		gdata.SetInt32(TEXTURETAG_PROJECTION_UVW);
		texTag->SetParameter(DescLevel(TEXTURETAG_PROJECTION), gdata, DESCFLAGS_SET_0);
		gdata.SetString(OVERRIDE_MAT_TAGNAME);
		texTag->SetParameter(DescLevel(TEXTURETAG_RESTRICTION), gdata, DESCFLAGS_SET_0);
		texTag->SetName(OVERRIDE_MAT_TAGNAME);
		texTag->Message(MSG_CHANGE);

	//Texture Tag stuff
		texTag = (TextureTag*)obj->MakeTag(Ttexture);
		if(!texTag) return;
		gdata.SetInt32(TEXTURETAG_PROJECTION_UVW);
		texTag->SetParameter(DescLevel(TEXTURETAG_PROJECTION), gdata, DESCFLAGS_SET_0);
		gdata.SetString(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
		texTag->SetParameter(DescLevel(TEXTURETAG_RESTRICTION), gdata, DESCFLAGS_SET_0);
		texTag->Message(MSG_CHANGE);
		texTag->SetMaterial(getMaterialById(baseMat+matIdx, doc));

		for(int t=0;t<triCount;t++)
		{
			if(t == 0 || t == endTriMat)
			{
				if (t != 0)
				{
					endTriMat = triMatEndList[++matIdx]/3;
					selTag = (SelectionTag*)obj->MakeTag(Tpolygonselection);
					if(!selTag) return;
					selTag->SetName(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
					polySelection = selTag->GetBaseSelect();

				//Texture Tag stuff
					TextureTag* texTag = (TextureTag*)obj->MakeTag(Ttexture);
					if(!texTag) return;
					gdata.SetInt32(TEXTURETAG_PROJECTION_UVW);
					texTag->SetParameter(DescLevel(TEXTURETAG_PROJECTION), gdata, DESCFLAGS_SET_0);
					gdata.SetString(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
					texTag->SetParameter(DescLevel(TEXTURETAG_RESTRICTION), gdata, DESCFLAGS_SET_0);
					texTag->Message(MSG_CHANGE);
					texTag->SetMaterial(getMaterialById(baseMat+matIdx, doc));
				}
			}

			v[0] = i_vertex + triList[c+0];
			v[1] = i_vertex + triList[c+1];
			v[2] = i_vertex + triList[c+2];
			polys[t] = CPolygon(v[0], v[2], v[1]); //flip c and b cause normals are inverted
			polySelection->Select(t);
/*
//different normal tag in r11 and r12, and the index is not right, anima saves per vertex and c4d saves per polygon
			WORD* n = normalsVal + t*12;
			n[0] = normals[c].x;
			n[1] = normals[c].y;
			n[2] = normals[c].z;
			n[3] = normals[c+1].x;
			n[4] = normals[c+1].y;
			n[5] = normals[c+1].z;
			n[6] = normals[c+2].x;
			n[7] = normals[c+2].y;
			n[8] = normals[c+2].z;
			n[9] = normals[c+2].x;
			n[10] = normals[c+2].y;
			n[11] = normals[c+2].z;
*/
			c+=3;
		}
		selTag->Message(MSG_UPDATE);


	//Select all Edges that have different SubMeshes as neighbor
	//and set a Phong break for it
		Neighbor neighbor;
		AutoAlloc<BaseSelect> phongBreaks;
		neighbor.Init(vertexCount, polys, faceCount, NULL);
		
		Int32 preMatEnd = triMatEndList[0]/3;
		for(Int32 mat=1; mat<meshExt->GetSubInfoCount(); mat++)
		{
			Int32 matEnd = triMatEndList[mat]/3;

			for(Int32 p=preMatEnd; p<matEnd; p++)
			{
				PolyInfo* polyInfo = neighbor.GetPolyInfo(p);

				for (Int32 side=0; side<4; side++)
				{
					if(polyInfo->face[side] == NOTOK)
						continue;

					if(polyInfo->face[side] < preMatEnd || polyInfo->face[side] > matEnd)
						phongBreaks->Select(polyInfo->edge[side]);
				}
			}

			preMatEnd = matEnd;
		}
		obj->SetSelectedEdges(&neighbor, phongBreaks, EDGESELECTIONTYPE_PHONG);

		obj->Message(MSG_UPDATE);
		PolygonObject::Free(m_obj);
		m_obj = obj;
	}
}

void AnimaMeshObject::buildGeom(Anima::MeshExporter* meshExporter, Anima::MeshExtGeomExt* meshExtGeom, BaseTime t, BaseDocument* doc)
{
	if(meshExtGeom)
	{
		this->meshExtModel = NULL;
		this->meshExtGeom = meshExtGeom;

		Anima::MeshExt *meshExt = meshExtGeom;
		meshExt->UpdateTime(t.Get());

		Int32 vertexCount = meshExt->GetVertexCount(); 
		Int32 vertexTxtCount = meshExt->GetTxtVertexCount(); 
		Int32 faceCount = meshExt->GetTriCount()/3;
		SReal *vertices = meshExt->GetVertices();
		SReal *normals = meshExt->GetNormals();
		SReal *uvList = meshExt->GetUVList();
		size_t *uvTriList = meshExt->GetUVTriList();

		PolygonObject* obj = PolygonObject::Alloc(vertexCount, faceCount);
		if(!obj) return;
//		VariableTag* normalsTag = obj->MakeVariableTag(Tnormal, faceCount, NULL);
//		if(!normalsTag) return;
//		WORD* normalsVal = normalsTag->GetDataAddressW();
		BaseTag* phong = obj->MakeTag(Tphong);
		phong->SetParameter(DescID(PHONGTAG_PHONG_ANGLELIMIT), GeData(TRUE), DESCFLAGS_SET_0);
		phong->SetParameter(DescID(PHONGTAG_PHONG_ANGLE), GeData(1.396263), DESCFLAGS_SET_0);
		phong->SetParameter(DescID(PHONGTAG_PHONG_USEEDGES), GeData(TRUE), DESCFLAGS_SET_0);
		UVWTag* uvwTag = UVWTag::Alloc(faceCount);
		if(!uvwTag) return;
		obj->InsertTag(uvwTag);
		UVWHandle uvwHandle=uvwTag->GetDataAddressW();

//		GePrint("    geom: " + LongToString(faceCount) + " faces, " + LongToString(vertexCount) + " vertices, " + LongToString(vertexTxtCount) + " uvs");
		CPolygon* polys = obj->GetPolygonW();
		Vector* points = obj->GetPointW();

		Int32 i_vertex=0;
		Int32 i_txt_vertex=0;
		Int32 i_face=0;

		int c=0;
		int tc=0;
		float x,y,z;
		for(int i=0;i<vertexCount;i++)
		{
			x = vertices[c++]*IMPORTS_SCALE_FACTOR* m_unitScaleFactor;
			y = vertices[c++]*IMPORTS_SCALE_FACTOR* m_unitScaleFactor;	// y = z
			z = -vertices[c++]*IMPORTS_SCALE_FACTOR* m_unitScaleFactor;	// z = -y
			//mesh.setVert(i_vertex + i,pos[0],pos[1],pos[2]);
			points[i] = Vector(x, y, z);

//			pos[0] = normals[c];
//			pos[2] = normals[c+1];	// y = z
//			pos[1] = -normals[c+2];	// z = -y
//			mesh.setNormal(i_vertex + i ,Point3(pos[0],pos[1],pos[2]));
		}

		SReal ax, ay, bx, by, cx, cy;
		for(int i=0;i<faceCount;i++)
		{
			ax = uvList[2*uvTriList[3*i+0]+0];
			ay = uvList[2*uvTriList[3*i+0]+1];
			bx = uvList[2*uvTriList[3*i+1]+0];
			by = uvList[2*uvTriList[3*i+1]+1];
			cx = uvList[2*uvTriList[3*i+2]+0];
			cy = uvList[2*uvTriList[3*i+2]+1];
#if API_VERSION < 11500
			uvwTag->Set(i, UVWStruct(Vector(ax, ay, 0), Vector(cx, cy, 0), Vector(bx, by, 0))); //flip c and b cause normals are inverted
#else
			UVWTag::Set(uvwHandle, i, UVWStruct(Vector(ax, ay, 0), Vector(cx, cy, 0), Vector(bx, by, 0))); //flip c and b cause normals are inverted
#endif
		}

		uvwTag->Message(MSG_UPDATE);

		int triCount = meshExt->GetTriCount()/3;
		size_t *triList  = meshExt->GetTriList();
		size_t *triMatEndList  = meshExt->GetTriEndList();
		i_vertex = 0;
		size_t v[3];
		c=0;	
		Int32 matIdx = 0;
		Int32 baseMat = meshExporter->GetModelExportColl().GetInstanceMaterialNumber(meshExtModel);
		SelectionTag* selTag = (SelectionTag*)obj->MakeTag(Tpolygonselection);
		if(!selTag) return;
		selTag->SetName(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
		BaseSelect* polySelection = selTag->GetBaseSelect();
		int endTriMat = triMatEndList[matIdx]/3;

	//Texture Tag stuff
		TextureTag* texTag = (TextureTag*)obj->MakeTag(Ttexture);
		if(!texTag) return;
		GeData gdata;
		gdata.SetInt32(TEXTURETAG_PROJECTION_UVW);
		texTag->SetParameter(DescLevel(TEXTURETAG_PROJECTION), gdata, DESCFLAGS_SET_0);
		gdata.SetString(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
		texTag->SetParameter(DescLevel(TEXTURETAG_RESTRICTION), gdata, DESCFLAGS_SET_0);
		texTag->Message(MSG_CHANGE);
		texTag->SetMaterial(getMaterialById(baseMat+matIdx, doc));
		
		for(int t=0;t<triCount;t++)
		{
			if(t == 0 || t == endTriMat)
			{
				if (t != 0)
				{
					endTriMat = triMatEndList[++matIdx]/3;
					selTag = (SelectionTag*)obj->MakeTag(Tpolygonselection);
					if(!selTag) return;
					selTag->SetName(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
					polySelection = selTag->GetBaseSelect();

				//Texture Tag stuff
					TextureTag* texTag = (TextureTag*)obj->MakeTag(Ttexture);
					if(!texTag) return;
					gdata.SetInt32(TEXTURETAG_PROJECTION_UVW);
					texTag->SetParameter(DescLevel(TEXTURETAG_PROJECTION), gdata, DESCFLAGS_SET_0);
					gdata.SetString(SELECTIONTAG_PRE + LongToString(baseMat+matIdx));
					texTag->SetParameter(DescLevel(TEXTURETAG_RESTRICTION), gdata, DESCFLAGS_SET_0);
					texTag->Message(MSG_CHANGE);
					texTag->SetMaterial(getMaterialById(baseMat+matIdx, doc));
				}
			}

			v[0] = i_vertex + triList[c+0];
			v[1] = i_vertex + triList[c+1];
			v[2] = i_vertex + triList[c+2];
			polys[t] = CPolygon(v[0], v[2], v[1]); //flip c and b cause normals are inverted
			polySelection->Select(t);

/*
//different normal tag in r11 and r12, and the index is not right, anima saves per vertex and c4d saves per polygon
			WORD* n = normalsVal + t*12;
			n[0] = normals[c].x;
			n[1] = normals[c].y;
			n[2] = normals[c].z;
			n[3] = normals[c+1].x;
			n[4] = normals[c+1].y;
			n[5] = normals[c+1].z;
			n[6] = normals[c+2].x;
			n[7] = normals[c+2].y;
			n[8] = normals[c+2].z;
			n[9] = normals[c+2].x;
			n[10] = normals[c+2].y;
			n[11] = normals[c+2].z;
*/
			c+=3;
		}
		selTag->Message(MSG_UPDATE);

	//Select all Edges that have different SubMeshes as neighbor
	//and set a Phong break for it
		Neighbor neighbor;
		AutoAlloc<BaseSelect> phongBreaks;
		neighbor.Init(vertexCount, polys, faceCount, NULL);
		
		Int32 preMatEnd = triMatEndList[0]/3;
		for(Int32 mat=1; mat<meshExt->GetMaterialNameCount(); mat++)
		{
			Int32 matEnd = triMatEndList[mat]/3;

			for(Int32 p=preMatEnd; p<matEnd; p++)
			{
				PolyInfo* polyInfo = neighbor.GetPolyInfo(p);

				for (Int32 side=0; side<4; side++)
				{
					if(polyInfo->face[side] == NOTOK)
						continue;

					if(polyInfo->face[side] < preMatEnd || polyInfo->face[side] > matEnd)
						phongBreaks->Select(polyInfo->edge[side]);
				}
			}

			preMatEnd = matEnd;
		}
		obj->SetSelectedEdges(&neighbor, phongBreaks, EDGESELECTIONTYPE_PHONG);


		obj->Message(MSG_UPDATE);
		PolygonObject::Free(m_obj);
		m_obj = obj;
	}
}

Bool AnimaMeshObject::RegisterAnimaMeshObject(void)
{
	return RegisterObjectPlugin(ID_ANIMAMESHOBJECT,GeLoadString(IDS_ANIMAMESHOBJECT),PLUGINFLAG_HIDE|OBJECT_GENERATOR,AnimaMeshObject::Alloc,"",AutoBitmap("anima.tif"),0);
}
