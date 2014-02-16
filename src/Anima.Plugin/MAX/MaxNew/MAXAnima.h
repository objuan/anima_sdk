#pragma once

namespace Anima
{
	class GfxModel;
	class MeshExt;
	class MeshExporter;
}

//class MeshObjectClassDesc;

//#define MeshObject_CLASS_ID	Class_ID(0x37240d5, 0x12fb4809)
//
////extern HINSTANCE hInstance;
//
//class MeshObjectClassDesc :public ClassDesc2 {
//	public:
//	int 			IsPublic() { return 1; }
//	void *			Create(BOOL loading = FALSE);
//
//	const TCHAR *	ClassName() { return "AnimaObject"; }
//	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
//	Class_ID		ClassID() { return MeshObject_CLASS_ID; }
//	const TCHAR* 	Category() { return "AnimaCategory"; }
//	//void			ResetClassParams(BOOL fileReset);
//	virtual BOOL			NeedsToSave() { return FALSE; }
//
//	const TCHAR*	InternalName()				{ return _T("AnimaObject"); }
//	HINSTANCE		HInstance()	;
//};

//class MeshObjectClassDesc:public ClassDesc2 {
//	public:
//	int 			IsPublic() { return TRUE; }
//	void *			Create(BOOL loading = FALSE);
//	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
//	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
//	Class_ID		ClassID() { return BLOBMESH_CLASS_ID; }
//	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }
//
//	const TCHAR*	InternalName() { return _T("BlobMesh"); }	// returns fixed parsable name (scripter-visible name)
//	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle
//
//};
//



class QTTask;

class MeshObject;
class MeshObjectClassDesc;

class MAXAnima
{
private:
	bool initialized;
	ImpInterface *importInt;
	Interface *globalInt;

private:

	int EnsureInit(const char *programPath);

public:

	static MAXAnima *Singleton();

	MAXAnima();
	~MAXAnima();

	int Load(const char*programPath,QString &name,ImpInterface *i,Interface *gi, bool suppressPrompts);
	int Load_old(QString &name,ImpInterface *i,Interface *gi, bool suppressPrompts);
	
//	BlobMesh *CreateBlobObject(BlobMeshClassDesc *objDesc,bool loading);
	SimpleObject *CreateObject(const char*programPath,MeshObjectClassDesc *classDesc,bool loading);
};

//INode *FindAnimaNode();

static INode* FindNodeRef(ReferenceTarget *rt);

static INode* GetNodeRef(ReferenceMaker *rm) {
	if (rm->SuperClassID()==BASENODE_CLASS_ID) return (INode *)rm;
	else return rm->IsRefTarget()?FindNodeRef((ReferenceTarget *)rm):NULL;
	}

static INode* FindNodeRef(ReferenceTarget *rt) {
	DependentIterator di(rt);
	ReferenceMaker *rm;
	INode *nd = NULL;
	while (rm=di.Next()) {	
		nd = GetNodeRef(rm);
		if (nd) return nd;
		}
	return NULL;
	}
