#pragma once

#include "Anima_nsbegin.h"

class Model;
class EscalatorMovePathMesh;
class ModelInstance;
class ModelExporterParams;
class MeshExporter;

struct MeshExtStaticVertexInfo
{
	bool cutted;
	int vertexIdx;
};

class ANIMA_CORE_API MeshExtStatic : public BObject
{
private:
	Anima::Model *model;
	MeshExtStaticVertexInfo *v_map;
	int v_map_count;
	bool loaded;
	int size;
	
//	Ogre::Vector3 *vertices;
	float* vertices;

public: 
	
	MeshExtStatic(Anima::Model *model);
	~MeshExtStatic();
	void Initialize();

	int ValidCount(){return v_map_count;}
	bool NeedLoad();
	void Start(int size);
	void AddVertex(int idx,Ogre::Vector3 &v);
	void End();
	bool IsCut(int vertexIdx);
	int GetMapVertexIdx(int vertexIdx);
};

#define MeshExt_BODY_IDX 0
#define MeshExt_HEAD_IDX 1

class ANIMA_CORE_API MeshExtSubmeshInfo : public BObject
{
public:

	int subMeshIdx;
	QString name;
	QString materialName;
	float smoothThresold;

	int vertexCount;
	int startTri;
	int endTri;

	bool IsBody();
	bool IsHead();

	MeshExtSubmeshInfo() : subMeshIdx(0),vertexCount(0),startTri(0),endTri(0)
	{
	}
};


class ANIMA_CORE_API MeshExt : public BObject
{
protected:

	MeshExporter *meshExporter;
	int currentVCount;
	//int *subVCount;

	Ogre::Vector3 position;
	float *vertices;
	float *normals;
	//size_t *v_map;
	size_t *tri_list;
	size_t *tri_uv_list;
	float *uvList;

	size_t vertex_count;
	size_t tri_count;
	size_t txt_vertex_count;
	size_t tri_mat_count;
	size_t *tri_mat_end_list;
	MeshExtSubmeshInfo *subPartInfoList;

	QList<QString> matNameList;

	//MeshExtStatic *staticInfo;
	//Anima::ModelInstance *model;
	//Ogre::Entity *entity;
	bool loaded;

protected:
	virtual void EnsureLoadVertex()=0;
	virtual void EnsureLoadAll()=0;

	void Initialize(Ogre::Mesh * mesh ,size_t & index_count, size_t & vertex_count,bool isExtraGeom);
	
	void BeginUpdate(double time);
	void EndUpdate();

public:
	int MaterialBaseNumber;

public:

	MeshExt(MeshExporter *meshExporter);
	~MeshExt();

	size_t GetVertexCount(){EnsureLoadVertex(); return vertex_count; }
	size_t GetTxtVertexCount(){EnsureLoadVertex(); return txt_vertex_count; }
	size_t GetTriCount(){EnsureLoadAll(); return tri_count; }
	int GetMaterialCount(){EnsureLoadAll(); return tri_mat_count; }

	virtual int GetSubInfoCount()=0;
	MeshExtSubmeshInfo &GetSubInfo(int idx);

	float *GetVertices();
	float *GetNormals();
	size_t *GetTriList();
	size_t *GetUVTriList();
	size_t *GetTriEndList(){EnsureLoadAll(); return tri_mat_end_list; }
	float *GetUVList(){EnsureLoadAll(); return uvList; }

	virtual void UpdateTime(double time)=0;
	virtual Ogre::Vector3 GetPosition(){return position;}
	virtual QString GetName()=0;

	virtual int GetMaterialNameCount()=0;
	virtual QString GetMaterialName(int idx)=0;
	// BBOX
};

class ANIMA_CORE_API MeshExtModel : public MeshExt
{
private:

	Ogre::Entity *entity;
	MeshExtStatic *staticInfo;
	Anima::ModelInstance *model;

protected:

	virtual void EnsureLoadVertex();
	virtual void EnsureLoadAll();

	void convertMeshData(float * vertexBuf, size_t vertex_count,size_t * faceBuf, size_t index_count,float *txtBuf);
	int FindVertex(float * vertexBuf,Ogre::Vector3 &v);

public:

	MeshExtModel(MeshExporter *meshExporter,Anima::ModelInstance *model,Ogre::Entity *entity);
	~MeshExtModel();

	virtual int GetSubInfoCount();
	int getNumSubMeshes();
	const QString getMaterialName(int subIdx);

	Ogre::MeshPtr GetOgreMesh(){return entity->getMesh();}
	Anima::ModelInstance *GetModel(){return model;}
	
	virtual int GetMaterialNameCount(){return 1;}
	virtual QString GetMaterialName(int idx){return "";}
	virtual QString GetName();
	virtual void UpdateTime(double time);
};

class ANIMA_CORE_API MeshExtGeomMaterial
{
public:

	QString name;
	int idx;

	MeshExtGeomMaterial(){}
	MeshExtGeomMaterial(const QString &m,int idx) : name(m),idx(idx)
	{}
};

class ANIMA_CORE_API MeshExtGeomExt : public MeshExt
{
private:

	size_t current_offset ;
	size_t current_txt_offset ;
	size_t shared_offset ;
	size_t next_offset ;
	size_t next_txt_offset ;
	size_t index_offset ;
	size_t txt_offset ;

	Anima::SceneObjectGfxMesh *mesh;
	ModelExporterParams *expParams;

	QList<Ogre::SceneNode *> nodeList;
	QList<Ogre::Entity *> entityList;

	QList<MeshExtGeomMaterial > matList;
	QMap<QString,MeshExtGeomMaterial> matMap;
	unsigned short *tri_map_list;

protected:

	virtual void EnsureLoadVertex();
	virtual void EnsureLoadAll();

	void Load(Ogre::Entity *,float * vertexBuf,size_t * faceBuf,float * txtBuf);

public:

	MeshExtGeomExt(MeshExporter *meshExporter,Anima::SceneObjectGfxMesh *mesh,ModelExporterParams *expParams);
	~MeshExtGeomExt();

	virtual int GetSubInfoCount();
	void Add(Ogre::SceneNode *);
	void Add(Ogre::Entity *);

	virtual QString GetName();
	ModelExporterParams *GetExportParams(){return expParams;}

	virtual int GetMaterialNameCount();
	virtual QString GetMaterialName(int idx);
	Anima::SceneObjectGfxMesh *GetMesh(){return mesh;}

	unsigned short *GetTriMapList(){return tri_map_list;}

	virtual void UpdateTime(double time);
};


#include "Anima_nsend.h"
