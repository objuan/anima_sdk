#pragma once
#include "c4d.h"
#include "oldversions.h"

#define ID_ANIMAOBJECT 1026246 //aXYZ_anima_plugin
#define OVERRIDE_MAT_TAGNAME "anima_override_mat_tag"

struct AnimaInfos
{
	Int32 actors;
	Int32 shownActors;
	Int32 polys;
	Int32 shownPolys;
	Int32 verts;
	Int32 shownVerts;
	AnimaInfos() { actors=shownActors=polys=shownPolys=verts=shownVerts=0; }
};

class AnimaObject : public ObjectData
{
		Bool m_bCached;
		BaseTime m_lastTime;
		Filename m_lastFilename;
		Real m_lastDarkness;
		Int32 m_bboxCount;
		AnimaInfos m_infos;
		Bool m_clearInfos;
		Bool m_lastDeformMode;
		BaseObject* m_obj;
	public:
		AnimaObject();
		~AnimaObject();
		virtual Bool Init(GeListNode *node);

		virtual BaseObject* GetVirtualObjects(BaseObject *op, HierarchyHelp *hh);
		virtual Bool Message(GeListNode *node, Int32 type, void *t_data);
		virtual Bool CopyTo(NodeData* dest, GeListNode* snode, GeListNode* dnode, COPYFLAGS flags, AliasTrans* trn);
		virtual Bool GetDParameter(GeListNode* node, const DescID& id, GeData& t_data, DESCFLAGS_GET& flags);

		static NodeData *Alloc(void) { return gNew AnimaObject; }

		static Bool RegisterAnimaObject(void);

		BaseObject* BuildAll(const Filename& f, BaseObject* op, Bool bEmbedded, Bool bForceFromFile, BaseDocument* doc, Bool resetScaleFactor);
		Bool hasToShowBBox();
		void AddInfos(Int32 actors, Int32 shownActors, Int32 polys, Int32 shownPolys, Int32 verts, Int32 shownVerts);
		void RebuildObject(BaseObject *op, BaseDocument* doc, Bool bForceFromFile=FALSE, Bool resetScaleFactor=FALSE);

		static Real g_endTime;
};