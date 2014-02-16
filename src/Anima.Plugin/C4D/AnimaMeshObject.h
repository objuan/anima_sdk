#pragma once
#include "c4d.h"
#include "oldversions.h"

#define ID_ANIMAMESHOBJECT 1026257 //aXYZ_anima_mesh

class AnimaMeshObject : public ObjectData
{
		Bool m_bCached;
		BaseTime m_lastTime;
		Bool m_lastShowBBox;
		BaseMaterial* m_lastOverrideMat;

	protected:
		PolygonObject* m_obj;
		Anima::MeshExtModel *meshExtModel;
		Anima::MeshExtGeomExt *meshExtGeom;
		Real m_unitScaleFactor;
	public:
		AnimaMeshObject();
		~AnimaMeshObject();
		virtual Bool Init(GeListNode *node);


		virtual Bool CopyTo(NodeData* dest, GeListNode* snode, GeListNode* dnode, COPYFLAGS flags, AliasTrans* trn);
		virtual BaseObject* GetVirtualObjects(BaseObject *op, HierarchyHelp *hh);
		virtual Bool Message(GeListNode *node, Int32 type, void *t_data);

		static NodeData *Alloc(void) { return gNew AnimaMeshObject; }

		static Bool RegisterAnimaMeshObject(void);

		void setScaleFactor(Real r);
		void UpdateMesh(BaseTime t);
		void buildModel(Anima::MeshExporter* meshExporter, Anima::MeshExtModel* meshExtModel, BaseTime t, BaseDocument* doc);
		void buildGeom(Anima::MeshExporter* meshExporter, Anima::MeshExtGeomExt* meshExtGeom, BaseTime t, BaseDocument* doc);
};