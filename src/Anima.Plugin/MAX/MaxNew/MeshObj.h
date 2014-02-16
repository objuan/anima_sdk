#pragma once

namespace Anima
{
	class GfxModel;
	class MeshExtModel;
	class MeshExtGeomExt;
	class MeshExporter;
	class MeshExtSubmeshInfo;
	class Task;
}

enum { anima_params, }; 
enum { anima_spec_params,anima_rgb_params,anima_update_params,anima_display_params,anima_render_params };
enum 
{ 
	anima_specular_head,anima_specular_body,anima_rgb_value,anima_update_mode,anima_info,anima_scene_info,anima_filepath,anima_max_model_value,anima_renderable_value,anima_smoothing_value,anima_blur_mode,anima_embedded_mode
};

#define MeshObject_CLASS_ID	Class_ID(0x37240d5, 0x12fb4809)

//class PSCM_Accessor;

#define MeshObjectSubMesh_CLASS_ID	Class_ID(0x66a04f19, 0x70f5433d)

class MeshObjectSubMesh;

//#define _QM(qstr)  qstr.toStdWString ().c_str ()
//#define _MQ(x) QString::fromWCharArray(x)

MSTR _QM(const QString &qstr) ;
QString _MQ(const MSTR &x);

//extern HINSTANCE hInstance;

class MyMessageBox : public QMessageBox{
	int timeout;
	bool autoClose;
	int currentTime;
public:
	void setAutoClose(bool autoClose){this->autoClose=autoClose;}
	void setTimeout(int timeout){this->timeout=timeout;}
	void showEvent ( QShowEvent * event ) {
		currentTime = 0;
		if (autoClose) {
		this->startTimer(1000);
		}
	}

	void timerEvent(QTimerEvent *event)
	{
		currentTime++;
		if (currentTime>=timeout) {
		this->done(0);
		}
	}
};


class MeshObjectClassDesc :public ClassDesc2 {
private:
	Interface *globalInterface;

	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE);

	const MCHAR *	ClassName() { return _M("AnimaObject"); }
	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
	Class_ID		ClassID() { return MeshObject_CLASS_ID; }
	const MCHAR* 	Category() { return _M("AnimaCategory"); }
	//void			ResetClassParams(BOOL fileReset);
	virtual BOOL	NeedsToSave() { return FALSE; }

	const MCHAR*	InternalName()				{ return _M("AnimaObject"); }
	HINSTANCE		HInstance()	;
};

class MeshObjectSubMeshClassDesc :public ClassDesc2 {
private:
	Interface *globalInterface;

	public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE);

	const MCHAR *	ClassName() { return _M("AnimaObjectSubMesh"); }
	SClass_ID		SuperClassID() { return GEOMOBJECT_CLASS_ID; }
	Class_ID		ClassID() { return MeshObjectSubMesh_CLASS_ID; }
	const MCHAR* 	Category() { return _M("AnimaCategory"); }
	virtual BOOL	NeedsToSave() { return FALSE; }

	const MCHAR*	InternalName()				{ return _M("AnimaObjectSubMesh"); }
	HINSTANCE		HInstance()	;
};

enum MeshObjectViewMode
{
	MeshObjectViewMode_FULL = 0,
	MeshObjectViewMode_BOX,
	MeshObjectViewMode_LOAD,
	MeshObjectViewMode_MAX
};

class MeshObjectModelMaterial
{
public:
	int materialNumber;
	int materialSubIdx;		
	Mtl *objMaterial;
};
class MeshObject : public SimpleObject2 {
private:
	int i_vertex;
	int i_txt_vertex;
	int i_face;
	int totalVertexCount;
	int totalFaceCount;
	int totalTxtVertexCount;
	//Anima::MeshExporter *meshExporter;
	
	bool invalidate;
	double oldTime;
	//TimeValue lastUpdateTime;

	//bool isInPlayTime;

	MeshObjectViewMode viewMode; // 0 = normale, 1 == bbox sempre
//	int maxModelDetail;

	bool disposed;
	bool loaded;
	bool loading;
	bool initializing;
	bool clearing;
	bool connected;
	bool matBuilded;
	QList<MeshObjectModelMaterial> modelMaterialList;
	MultiMtl *objMaterialExtra;
	INode *node;
	int visibilityCount;
	//int updateMode; // 0 = rendering , 1 = manually

	int taskDone;
	bool toBuildVisibility;
	QList<INode *> selectedNodes;

	//PSCM_Accessor *accessor;
	ParamBlockDesc2 *obj_param_blk;
//	QString loadedAnimaPath;

public:
	QString embeddedFile;
	double endPlayTime;
	QString current_render_type; // Anima::ModelExporterRenderType

public:			
		// Class vars
	
		//static IParamMap *pmapParam;
		//static IObjParam *ip;
		static MeshObject *meshObject;

		QString animaPath;
		QList<MeshObjectSubMesh *> main_geomList;
		QList<MeshObjectSubMesh *> main_modelList;
		Anima::MeshExporter *meshExporter;
		INode *animaNode;

private:
	/*	void AddModel(Anima::MeshExtModel *meshExt);

		void UpdateModelFull(Anima::MeshExtModel *meshExt);
		void UpdateModelGroup(Anima::MeshExtModel *meshExt);*/

		void BuildMaterial();
		void BuildNormals();
	//	StdMat *New3DSDefaultMtl(const char *name,const char *txtPath,const QString &normal_txtPath,int matIdx,Anima::MeshExtSubmeshInfo *subInfo);
		
	//	Mtl *LoadMaterial(Anima::MeshExt *meshExt,Anima::ModelExporter *modelExporter,Anima::ModelExporterRender *modelExporterRender,const QString &materialName);

		void BuildMeshBox(TimeValue t);
		void BuildMeshFull(TimeValue t);

		void SetVisibility(TimeValue time );
		void ClearAnimaMaterial();

		// SOLO MESH 2
	//	int LoadFromEmbedded(TimeValue t);
	//	void _Clear();
	/*	INode *FindAnimaNode(INode *node);
		INode *FindAnimaNode();*/

public:			
		MeshObject(MeshObjectClassDesc *classDesc,BOOL loading);		
		~MeshObject();

		void BuildMaterialMask();

		//void Restart();
		//INode *GetNode();
		//void CheckSceneNode();

		QList<MeshObjectModelMaterial> &GetModelMaterialList(){return modelMaterialList;}
		Mtl *GetMaterial(int modelIdx){ return modelMaterialList[modelIdx].objMaterial;}
		int GetMaterialSubIdx(int modelIdx){ return modelMaterialList[modelIdx].materialSubIdx;}
		MultiMtl *GetMaterialExtra(){return objMaterialExtra;}

		void UpdateMaterialValues();
		void CheckMaterial();

		void RebuildMaterial();
		void UpdateMaxModel();

		void OnPreRender(); // EVENT
		void OnPostRender(); // EVENT
		void OnSelectChanged(); // EVENT

		CreateMouseCallBack* GetCreateMouseCallBack();
		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev);
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);
		RefTargetHandle Clone(RemapDir& remap = DefaultRemapDir());
		const MCHAR *GetObjectName() { return _M("an(i)maMeshRoot");}
		BOOL HasUVW();
		void SetGenUVW(BOOL sw);
				
		// Animatable methods		
		void DeleteThis();
		SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
		Class_ID ClassID() { return MeshObject_CLASS_ID; } 
		void GetClassName(TSTR& s) {s = _M("AnimaObject");}
		
		// From ReferenceTarget
		IOResult Load(ILoad *iload);
		IOResult Save(ISave* isave) ;

		void Clear();
		//int LoadFromEmbedded(TimeValue t);
		//int LoadFromGUI(const QString &animaPath,INode* animaNode);
		int Load(const QString &animaPath,INode* animaNode,bool fromFile,bool buildMaterial);

						
		// From SimpleObject
		void BuildMesh(TimeValue t); // interface
		BOOL OKtoDisplay(TimeValue t);
		void InvalidateUI();
		
		void SetVisibility(MeshObjectSubMesh *mesh ,INode*node,TimeValue time  );
	
		//void EnsureTime(TimeValue t);
	
		bool AskBBOXMode(MeshObjectSubMesh *mesh);
		void OnTaskManagerChanged(Anima::Task *task);

	/*	virtual int NumberOfRenderMeshes  (    )  ;
		virtual Mesh* GetMultipleRenderMesh  (  TimeValue  t,  INode *  inode,  View &  view,  BOOL &  needDelete,  int  meshNumber  )  ;*/

	/*	ParamDimension *GetParameterDim(int pbIndex);
		TSTR GetParameterName(int pbIndex);*/


		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock2; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock2->ID() == id) ? pblock2 : NULL; } // return id'd ParamBlock
	
		QString GetSceneDesc();
		void  OnRefresh ();
	};

class  MeshObjectSubMesh : public SimpleObject2 {
private:
		bool build;
		bool initialized;
		bool smoothValid;
		static QList<MSTR *> nameList;
		Anima::MeshExporter *meshExporter;
		//INode *node;

public:
		Anima::MeshExtModel *meshExtModel;
		Anima::MeshExtGeomExt *meshExtGeom;
		int visibilityFlag;
	//	bool bboxMode;
public:			
		// Class vars
	//	void Init(INode *node){this->node=node;initialized=true;}
		void BuildGeom(Anima::MeshExporter *meshExporter,Anima::MeshExtGeomExt *meshExt,TimeValue t);
		void BuildModel(Anima::MeshExporter *meshExporter,Anima::MeshExtModel *meshExt,TimeValue t);
		void Update(TimeValue t);

public:			
		 MeshObjectSubMesh(BOOL loading);		
		~MeshObjectSubMesh();

		
	//	INode *GetNode(){return node;}

		static void InitializeAll(TimeValue t);
		static void Clear();
		static void BuildAll(TimeValue t,INode *parentNode);
		static void PostBuildAll(TimeValue t,INode *parentNode);


		// From SimpleObject
		void BuildMesh(TimeValue t); // interface

	
		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev);
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);

		// Animatable methods		
		void DeleteThis();
		SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
		Class_ID ClassID() { return MeshObjectSubMesh_CLASS_ID; } 
		void GetClassName(MSTR& s) {s = _M("MeshObjectSubMesh");}
		const MCHAR *GetObjectName() { return _M("an(i)maMesh");}

		void Invalidate();
		void InvalidateGeom();

		void EnsureSmooth();

		void GetInfo(bool &boxMode,int &vertices,int &tri);

		CreateMouseCallBack* GetCreateMouseCallBack();
	};

//class  MyDummyObject : public DummyObject 
//{
//
//};