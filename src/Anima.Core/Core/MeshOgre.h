#pragma once

#include "./Mesh.h"
#include "./SceneObject.h"

namespace Opcode
{
	class MeshInterface;
	class Model;
}

#include "Anima_nsbegin.h"

class CollisionObject;

// ----------------------------------------------

class ANIMA_CORE_API MeshOgre : public MeshStatic, public SceneObject
{
private:


public:
	
	Ogre::Entity *entity;
	Ogre::SceneNode* node;

	CollisionObject *collObj;

private:
	
public:

	MeshOgre();
	virtual ~MeshOgre();

	virtual AObject::Type GetType(){return Anima::AObject::MeshStaticType;}
	virtual QString ClassName(){return "MeshOgre";}
	virtual QImage Image(){return Anima::ResourceManager::Singleton.item_color;}
	virtual QString GetName(){return "MeshOgre" ;}
	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	virtual void SetUnit(Anima::UnitMeter unit);

	//virtual void SetColor(const QColor &color);
	//virtual void ResumeColor();

	void Initialize(const QString &meshName,const QStringList &layersName);

	CollisionObject *GetCollObj(){return collObj;}

	QString Dump();
};

#include "Anima_nsend.h"
