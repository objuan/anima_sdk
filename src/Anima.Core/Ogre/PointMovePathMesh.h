#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"
class PointMovePath;
class ModelInstance;

class PointMovePathMesh : public SceneObjectGfxMesh  
{
private:

	Ogre::String name;
//	OgreOpcode::PtrCollisionShape *collisionShape;
	Ogre::ManualObject *mo;
	Ogre::ManualObject *mo_effect;
	//Ogre::ManualObject *mo_link;
	PointMovePath *pointMovePath;
	ModelInstance *modelInstance;

	bool haveEffect;
	//bool haveLink;
	//Ogre::Vector3 linkOffset;
	Ogre::String effectMaterial;
	bool needBuild;
	//double linkAngle;

private:
	void AddSegment(Ogre::Vector3 vertex,Ogre::Vector3 lastPt,int &segmentCount,float *v ,int &vCount);
	void BuildIdx(int segmentCount,size_t *idx,int &idxCount);
public:

	PointMovePathMesh(PointMovePath *pointMovePath,ModelInstance *mi);
	~PointMovePathMesh();

	void SetPosRot(const Ogre::Vector3 &pos,const Ogre::Quaternion &rot);

	void InvalidateBuild(){needBuild=true;}

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_PATH;}

	void Create(Ogre::String &name);
	virtual void OnPreRender(bool &meshChanged);
};

#include "Anima_nsend.h"
