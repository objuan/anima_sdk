#pragma once

class CollisionModel3D;

#include "Mesh.h"
#include "Anima_nsbegin.h"

class ANIMA_CORE_API MeshLayer
{
public:

private:

	float glColor[3];

	QColor color;
	QString name;
	KFbxMesh *nativeMesh;
	IMesh::CollisionType collisionType;
	CollisionModel3D *coll;
	IMesh::MaterialModes materialMode;

public:
	MeshLayer(const QString &name,KFbxMesh *nativeMesh,IMesh::CollisionType collisionType);
	~MeshLayer(void);

	QString GetName(){return name;}
	QColor GetColor(){return color;}
	void SetColor(QColor &color);
	void SetMaterialMode(IMesh::MaterialModes materialMode);
	
	// render

	virtual void Draw(bool selectMode,KFbxXMatrix &trx,const KFbxNode::EShadingMode &shadingMode);

	// collision

	void GetTriangle(int triIdx,KFbxVector4 &v1,KFbxVector4 &v2,KFbxVector4 &v3);
	void GetTrianglePlane(int triIdx,KFbxVector2 &base1,KFbxVector2 &base2,KFbxVector4 &dir);

	bool Pick(float origin[],float dir[],float collPoint[],float *collDistance,int *collPolygonIdx,float collNormal[]);

};
#include "Anima_nsend.h"