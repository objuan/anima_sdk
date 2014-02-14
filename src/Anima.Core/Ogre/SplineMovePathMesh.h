#pragma once

#include "Core/SceneObjectGfx.h"

#include "Anima_nsbegin.h"
class SplineMovePath;
class ModelInstance;

class SplineMovePathMesh : public SceneObjectGfxMesh  
{
private:

	Ogre::String name;
//	OgreOpcode::PtrCollisionShape *collisionShape;
	Ogre::ManualObject *mo;
	Ogre::ManualObject *mo_effect;
	Ogre::ManualObject *mo_coll;
	Ogre::Entity *entity;
	//Ogre::SceneNode *node;

	SplineMovePath *splineMovePath;
	ModelInstance *modelInstance;

	bool haveEffect;
	Ogre::String effectMaterial;
	
private:
	void AddSegment(Ogre::Vector3 vertex,Ogre::Vector3 lastPt,int &segmentCount,float *v ,int &vCount, float startLength,Ogre::Real roadWidth);
	void BuildIdx(int segmentCount,size_t *idx,int &idxCount,int startDotIdx,int endDotIdx);
   
	void GetSegmentInfo(int subPartNumber,Ogre::String &materialName,float &segmentSize);

	//void OnPreRenderFull(bool &meshChanged);
	void OnPreRenderSmall(bool &meshChanged);

public:

	SplineMovePathMesh(SplineMovePath *splineMovePath,ModelInstance *mi);
	~SplineMovePathMesh();

	void Create(Ogre::String &name);

	virtual int GetRenderQueueGroup(){return RENDER_QUEUE_ANIMA_PATH;}

	virtual void OnPreRender(bool &meshChanged);
};

#include "Anima_nsend.h"
