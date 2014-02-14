#pragma once

#include "Core/SceneObjectGfx.h"
#include "Core/AnimParts/EscalatorMovePath.h"

#include "Anima_nsbegin.h"
class EscalatorMovePath;
class ModelInstance;

class EscalatorMovePathMesh : public SceneObjectGfxMesh  
{
private:

	Ogre::String name;
	Ogre::Entity *baseEntity_enter;
	Ogre::Entity *baseEntity_exit;
	//Ogre::Entity *step;
	Ogre::ManualObject *mo_effect;

	Ogre::SceneNode *baseNode_enter;
	Ogre::SceneNode *baseNode_exit;
	Ogre::SceneNode *stepRootNode;
	Ogre::SceneNode *effectRootNode;

	EscalatorMovePath *movePath;
	ModelInstance *modelInstance;

	Ogre::Vector3 old_stairDir;
	Ogre::Vector3 stairDir;
	EscalatorMoveLine oldMoveLine;
	
	Ogre::Vector3 startLinePoint,endLinePoint;
	Ogre::Vector3 start_min,start_max;
	Ogre::Vector3 end_min,end_max;
	Ogre::Vector3 step_min,step_max;
	Ogre::Vector3 center_min,center_max;
	bool *visibleMask;

private:
	void Process();
	void AddQuad(int &baseFaceIdx,Ogre::Vector3 &p0,Ogre::Vector3 &p1,Ogre::Vector3 &p2,Ogre::Vector3 &p3);

protected:
	virtual void GetEffectMovableObject(Ogre::SceneNode *node,QList<Ogre::MovableObject *> &list);
public:

	EscalatorMovePathMesh(EscalatorMovePath *movePath,ModelInstance *mi);
	~EscalatorMovePathMesh();

	//void Update(bool stepChanged);

	virtual QString GetName();

	//virtual int GetMeshCount(){return 4;};
	void Create(Ogre::String &name);
	//virtual int GetCollisionCount(){return 0;}

	//bool GetInfo(int partNumber,double distance,double time,Ogre::Vector3 &point,double &height,Ogre::Vector3 &dir);
	//void GetStandInfo(double &distance,double &height,double &time);

	virtual MeshExtGeomExt *ExportGeometry(MeshExporter *);

	virtual void OnPreRender(bool &meshChanged);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_PATH;}
};

#include "Anima_nsend.h"
