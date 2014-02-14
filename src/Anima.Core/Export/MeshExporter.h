#pragma once

#include "Anima_nsbegin.h"

class MeshExtModel;
class MeshExtGeomExt;
class ModelInstance;
class SceneObjectGfxMesh;

class ANIMA_CORE_API MeshExtCollection
{
public:

	QMap<ModelInstance *,MeshExtModel *> *meshMap;
	QList<MeshExtModel *> *meshList;
	QList<MeshExtModel *> *firstMeshList;
	QList<MeshExtModel *> *orderedMeshList;

public:

	MeshExtCollection();
	~MeshExtCollection();

	int InstanceCount(){return meshList->count();}
	MeshExtModel *GetInstance(int idx){return (*meshList)[idx];}

	int ModelCount(){return firstMeshList->count();}
	MeshExtModel *GetModel(int idx){return (*firstMeshList)[idx];}

	//void UpdateCurrentTime();

	MeshExtModel *GetModelByVertexIdx(uint globalVertexIdx,uint &subVertexIdx);
	int GetMaterialNumber(Anima::MeshExtModel *meshExt);
	int GetInstanceMaterialNumber(Anima::MeshExtModel *meshExt);

	MeshExtModel *Get(ModelInstance *m);
};

class ANIMA_CORE_API GeomExtCollection
{
public:

	QMap<SceneObjectGfxMesh *,MeshExtGeomExt *> *meshMap;
	QList<MeshExtGeomExt *> *meshList;
	
public:

	GeomExtCollection();
	~GeomExtCollection();

	int Count(){return meshList->count();}
	MeshExtGeomExt *GetGeom(int idx){return (*meshList)[idx];}

	void Add(MeshExtGeomExt *geom);

	//void UpdateCurrentTime();

	MeshExtGeomExt *Get(SceneObject *path);
};


class ANIMA_CORE_API MeshExporter
{
public:
	enum MeshExporterBuildMode
	{
		GHOST = 0,
		VERTEX,
		FULL,
	};

private:
	QMutex mutex;
	double lastTimeBuilded;
	double lastTimeUpdated;
	MeshExtCollection modelExportColl;
	GeomExtCollection geomExportColl;
public:

	MeshExporter();

	void BuildCurrentTime();
//	void UpdateCurrentTime();
	void BeginUpdate(double time);
	void EndUpdate();

	MeshExtCollection &GetModelExportColl();
	GeomExtCollection &GetGeomExportColl();
};

#include "Anima_nsend.h"
