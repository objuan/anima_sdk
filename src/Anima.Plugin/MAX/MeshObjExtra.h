#pragma once

namespace Anima
{
	class GfxModel;
	class MeshExt;
	class MeshExporter;
	class MeshExtSubmeshInfo;
	class Task;
}

#define MeshObjectDummyMesh_CLASS_ID	Class_ID(0x46140528, 0x1bbd7d3e)
#define MeshObjectGeomMesh_CLASS_ID		Class_ID(0x1b99070d, 0x5ba0b46)

class MeshObjectDummyClassDesc :public ClassDesc2 {
private:
	//Interface *globalInterface;

	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE);

	const TCHAR *	ClassName() { return "MeshObjectDummyClassDesc"; }
	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
	Class_ID		ClassID() { return MeshObjectDummyMesh_CLASS_ID; }
	const TCHAR* 	Category() { return "AnimaCategory"; }
	virtual BOOL	NeedsToSave() { return FALSE; }

	const TCHAR*	InternalName()	{ return _T("MeshObjectDummyClassDesc"); }
	//HINSTANCE		HInstance()	;
};

class MeshObjectGeomClassDesc :public ClassDesc2 {
private:
	//Interface *globalInterface;

	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE);

	const TCHAR *	ClassName() { return "MeshObjectGeomClassDesc"; }
	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
	Class_ID		ClassID() { return MeshObjectGeomMesh_CLASS_ID; }
	const TCHAR* 	Category() { return "AnimaCategory"; }
	virtual BOOL	NeedsToSave() { return FALSE; }

	const TCHAR*	InternalName()	{ return _T("MeshObjectGeomClassDesc"); }
	//HINSTANCE		HInstance()	;
};


class  MeshObjectGeomMesh : public SimpleObject2 {

		void BuildMesh(TimeValue t); // interface

		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev);
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);

		// Animatable methods		
		void DeleteThis(){}
		SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
		Class_ID ClassID() { return MeshObjectGeomMesh_CLASS_ID; } 
		void GetClassName(TSTR& s) {s = "MeshObjectGeomMesh";}
		TCHAR *GetObjectName() { return ("an(i)maMesh");}

		CreateMouseCallBack* GetCreateMouseCallBack();
};

class  MeshObjectDummyMesh : public DummyObject {

	
		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev);
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);

		// Animatable methods		
		void DeleteThis(){}
		SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
		Class_ID ClassID() { return MeshObjectDummyMesh_CLASS_ID; } 
		void GetClassName(TSTR& s) {s = "MeshObjectDummyMesh";}
		TCHAR *GetObjectName() { return ("an(i)maMesh");}

		virtual void InitNodeName (TSTR &s) ;
		virtual RefResult NotifyRefChanged  (  Interval  iv,  RefTargetHandle  hTarg,  PartID &  partID,   RefMessage  msg ) ; 

		CreateMouseCallBack* GetCreateMouseCallBack();
};

