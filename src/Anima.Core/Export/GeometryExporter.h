#ifndef _GEOMETRY_EXPORTER_H_
#define _GEOMETRY_EXPORTER_H_

#ifndef _BASE_EXPORTER_H_
#include "BaseExporter.h"
#endif // _BASE_EXPORTER_H_

class FCDEntity;
class FCDEntityInstance;
class FCDGeometry;
class FCDGeometryMesh;
class FCDGeometryPolygons;
class FCDGeometryPolygonsInput;
class FCDGeometrySource;
class FCDMaterial;
class FCDTexture;
class FCDEffectStandard;
struct ExportedMaterial;
class FCDMaterialInstance;

typedef QMap<QString*, FCDGeometry*> ExportedGeometryMap;
//typedef QList<Mtl> MaterialList;

// since there's no base class defining both MeshNormalSpec & MNNormalSpec
// we have to create this dispatcher class
//
class NormalSpec
{
private:
	//MeshNormalSpec *triangleSpec;
	bool isTriangles;
	//MNNormalSpec *polySpec;
	bool isEditablePoly;

public:
	//NormalSpec():triangleSpec(NULL),polySpec(NULL),isEditablePoly(false),isTriangles(false){}
	//~NormalSpec(){ SetNull(); }

	//// two main modifiers
	////void SetTriangles(MeshNormalSpec *t);
	////void SetPolygons(MNNormalSpec *p);

	//void SetNull();
	//bool IsNull();

	//// dispatchers
	//int GetNumNormals();
	////Point3 GetNormal(int idx);
	//int GetNormalIndex(int faceIdx, int cornerIdx);

}; // end NormalSpec class


class GeometryExporter : public BaseExporter
{
public:
	GeometryExporter(DocumentExporter* document);
	virtual ~GeometryExporter();

	FCDEntity* ExportMesh(Ogre::Mesh* mesh, bool affectsControllers = false);
	//FCDEntity* ExportSpline(INode* node, Object* object);
	//FCDEntity* ExportNURBS(INode* node, Object* object);

	void ExportInstance(Ogre::Mesh* mesh, FCDEntityInstance* instance);

	void countIndicesAndVertices(size_t & index_count, size_t & vertex_count);
	void convertMeshData(float * vertexBuf, size_t vertex_count,size_t * faceBuf, size_t index_count);

	FCDTexture* ExportTexture(unsigned int idx, FCDEffectStandard* stdProfile,const QString &path);
	FCDMaterial* ExportMaterial(Ogre::MaterialPtr mat);
	
private:
	void ExportVertexPositions(FCDGeometryMesh* colladaMesh);
	void ExportNormals(FCDGeometryMesh* colladaMesh);
	void ExportEditablePolyNormals(Ogre::Mesh& mesh, FCDGeometrySource* source);
	void ExportTextureChannel(FCDGeometryMesh* colladaMesh, int channelIndex);
	void ExportPolygonsSet(FCDGeometryPolygons* colladaPolygons, int materialIndex, int numMaterials);
//	void FlattenMaterials(Ogre::Material* material, MaterialList& mtlMap, int materialIndex = -1);
	//bool IsEditablePoly() { return triObject == NULL; }
	//bool IsNURB(Object* object);
	//void ClassifyObject(Object* mesh, bool affectsControllers);

	NormalSpec currentNormals;

	// Buffered and possibly created geometric object.
	Ogre::Mesh *mesh;
	//GeomObject* geomObject;
	//TriObject* triObject;
	//PolyObject* polyObject;
	bool deleteObject;
public:
	size_t numVertices; ///<
	size_t numFaces; ///<
	float* mVertexBuf; ///<
	float* mNormalBuf; ///<
	float* mTxtBuf; ///<
	size_t*   mFaceBuf; ///<
	int* materialStartBuff;

	QList<ExportedMaterial> exportedMaterials;
};

// structure used to link together the COLLADA material and the 3dsMax MatID.
struct ExportedMaterial
{
	class FCDMaterial* colladaMaterial;
	int materialId;

	ExportedMaterial(FCDMaterial* _colladaMaterial, int _materialId)
		:	colladaMaterial(_colladaMaterial), materialId(_materialId) {}
};


#endif // _GEOMETRY_EXPORTER_H_
