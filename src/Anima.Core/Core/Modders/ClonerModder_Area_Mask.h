#pragma once

#include "./ClonerModder.h"

#include "Anima_nsbegin.h"

class Animation;
class ClonerModder;

enum ClonerModder_Rotation_Type
{
	ClonerModder_Rotation_Type_Random = 0,
	ClonerModder_Rotation_Type_Focus = 1,
	ClonerModder_Rotation_Type_Parallel = 2,
	ClonerModder_Rotation_Type_Concentric = 3
};

class ANIMA_CORE_API ClonerModder_Rotation
{
private:
	ClonerModder *modder;
public:
	ClonerModder_Rotation_Type type;
	Ogre::Vector3 focusPosition;
	double random_seed; // <1 disabled
	double concentric_ray; // <1 disabled

	Ogre::Vector3 midPoint;

	ClonerModder_Rotation(ClonerModder *modder);
	bool  Init();
	void Process();
	void SetConcentricRay(double concentric_ray);
	virtual void Load(QDomElement e);
	virtual void Save(QDomElement e);
};

// ======================

enum ClonerModder_Area_Mask_Type
{
		ClonerModder_Area_Mask_Type_Inactive=0,
		ClonerModder_Area_Mask_Type_Geometry=1,
		ClonerModder_Area_Mask_Type_BitmapMask=2
};

enum ClonerModder_Area_Mask_Type_Geometry_Mode
{
		ClonerModder_Area_Mask_Type_Geometry_Mode_Inactive=0,
		ClonerModder_Area_Mask_Type_Geometry_Mode_Layer=1,
		ClonerModder_Area_Mask_Type_Geometry_Mode_Object=2
};

class ANIMA_CORE_API ClonerModder_Area_Mask : public ClonerModder_Area
{
private:
	ClonerModder *modder;
	ClonerModder_Area_Mask_Type type;
	ClonerModder_Area_Mask_Type_Geometry_Mode geomMode;
	Animation *anim;
	QString layerSelected;
	QString objectPath;
	bool invalidate;
	
	QList<Ogre::Vector3> positionList;

private:
	bool GetInfo(Ogre::Vector3** vertexBuf ,size_t** faceBuf ,size_t &vertex_count, size_t & index_count);
	void countIndicesAndVertices(Ogre::SubMesh *mesh,size_t & index_count, size_t & vertex_count);
	void convertMeshData(Ogre::SubMesh *mesh,Ogre::Vector3* vertexBuf, size_t vertex_count,size_t * faceBuf, size_t index_count);
public:

	ClonerModder_Area_Mask(ClonerModder *modder,Animation *anim);

	void ensureValidate();
	virtual double GetArea();
	virtual void SetArea(double area);
	
	virtual void Process(Animation *anim,QList<ModelInstance *> models ,int seed,bool useInvert);

	QString getLayerSelected(){ return layerSelected;}
	void setLayerSelected(QString layer);

	QString getObjectPath(){ return objectPath;}
	void setObjectPath(QString objectPath);

	ClonerModder_Area_Mask_Type getType(){return type;}
	void setType(ClonerModder_Area_Mask_Type type);

	ClonerModder_Area_Mask_Type_Geometry_Mode getGeomMode(){return geomMode;}
	void setGeomMode(ClonerModder_Area_Mask_Type_Geometry_Mode mode);

	void OnMeshUnitChanged();

	int Count(){return positionList.count();}
	
	virtual void Load(QDomElement e);
	virtual void Save(QDomElement e);
};

#include "Anima_nsend.h"

