#pragma once

#include "./SceneObject.h"

namespace Opcode
{
	class MeshInterface;
	class Model;
}

#include "Anima_nsbegin.h"

class CollisionObject;
class MeshStatic;

// ----------------------------------------------

class ANIMA_CORE_API IMesh
{
public:
		
	enum DrawType
	{
		DrawType_Disabled=0,
		DrawType_Normal,
		DrawType_Focus,
		DrawType_Selected,
	};

	enum CollisionType
	{
		Disabled =0,
		Real ,
	};

public:

};


class ANIMA_CORE_API MeshStaticClipPlane :  public Anima::BObject
{
private:
	Ogre::Plane plane;
	float height;

public:
	MeshStaticClipPlane();

	void OnPreRender();
};

class ANIMA_CORE_API MeshStaticClipPlaneManager:  public Anima::BObject
{
private:
	QList<MeshStaticClipPlane *> clipPlaneList;
public:

	MeshStaticClipPlaneManager();
//	virtual ~MeshStaticClipPlaneManager();

	inline QList<MeshStaticClipPlane *> &GetClipPlaneList(){return clipPlaneList;}
	void OnPreRender();
};

class ANIMA_CORE_API MeshStaticLayer :  public Anima::BObject
{
private:
	MeshStatic *meshStatic;
	int layerIdx;
	bool haveColor;
	QColor baseColor;
	QColor color;
	Ogre::Material *material;
	Ogre::SubMesh* subMesh;
	Ogre::SubEntity *subEntity;
	bool enabled;
	
public:
	int NumVertices;
	int NumFaces;
	QString Name;
	double cutYLevel;

	MeshStaticLayer():material(NULL),subMesh(NULL){}
	MeshStaticLayer(MeshStatic *meshStatic,int layerIdx,Ogre::SubMesh* subMesh,Ogre::SubEntity *subEntity,Ogre::Material *material,const QString &name);
	~MeshStaticLayer();

	void OnPreRender();

	inline Ogre::SubMesh* GetSubMesh(){return subMesh;}
	inline bool IsEnabled(){return enabled;}
	void SetEnabled(bool enabled);
	QColor GetColor();
	void SetColor(const QColor &col) ;
	//void UpdateMaterial();
};

class ANIMA_CORE_API MeshStatic : public IMesh , public SceneObject
{
protected:

	QString filePath;
	Anima::UnitMeter unit;
	Ogre::Vector3 rot;
	QList<MeshStaticLayer *> layerList;
	QMap<QString , MeshStaticLayer *> loadLayerList;
	MeshStaticClipPlaneManager *clipPlaneManager;
	
public:
	Ogre::Entity *entity;
	Ogre::SceneNode* node;

	CollisionObject *collObj;

	Ogre::Vector3 baricentric;
	float xzsize;
	int NumVertices;
	int NumFaces;
	QString Name;

public:
	MeshStatic();
	virtual ~MeshStatic();

	virtual AObject::Type GetType(){return Anima::AObject::MeshStaticType;}
	virtual QString ClassName(){return "MeshStatic";}
	virtual QImage Image(){return Anima::ResourceManager::Singleton.item_color;}
	virtual QString GetName(){return "MeshStatic" ;}
	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	void Initialize(const QString &meshName, QStringList &layersName);

	Anima::UnitMeter GetUnit(){return unit;}
	virtual void SetUnit(Anima::UnitMeter unit);
	Ogre::Vector3 GetRot(){return rot;}
	void SetRot(Ogre::Vector3 &rot){this->rot=rot;}

	QString GetFilePath(){return filePath;}
	void SetInfo(const QString &filePath){this->filePath=filePath;}

	CollisionObject *GetCollObj(){return collObj;}

	inline QList<MeshStaticLayer *> &GetLayerList(){return layerList;}
	inline MeshStaticClipPlaneManager *GetClipPlaneManager(){return clipPlaneManager;}

	void OnPreRender();
	void Load(QDomElement &animEle );
	void Save(QDomDocument &doc,QDomElement &ele,const QString &saveFilePath,bool isDefault);
		
	QString Dump();
};

#include "Anima_nsend.h"

