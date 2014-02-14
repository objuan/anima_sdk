#pragma once

namespace Opcode
{
	class MeshInterface;
	class Model;
}

#define USE_HW_COLLISION
#if defined(WIN64) || defined(Q_OS_MAC)
#undef  USE_HW_COLLISION
#else if
#endif

#include "Anima_nsbegin.h"

class SceneObject;
class MeshStatic;

class ANIMA_CORE_API CollisionObject : public BObject
{
friend class CollisionObjectStorage;
protected:
		
	float x_min[3];
	float x_max[3];
	MeshStatic *meshStatic;
	SceneObject *obj;
	Ogre::Mesh *mesh;
	Ogre::String meshName;
	bool disposeMesh;
	bool disposed;
	
protected:
	
public:

	void countIndicesAndVertices(Ogre::Mesh *mesh,size_t & index_count, size_t & vertex_count);
	CollisionObject(MeshStatic *meshStatic);
	CollisionObject(SceneObject *obj,Ogre::Mesh *mesh,bool disposeMesh);
	virtual ~CollisionObject();

	virtual size_t GetNumVertices()=0;
	virtual size_t GetNumFaces()=0;

	SceneObject *GetSceneObject(){return obj;}
	Ogre::String GetMeshName(){return meshName;}

	Ogre::Vector3 GetMinBound();
	Ogre::Vector3 GetMaxBound();
	Ogre::Vector3 GetMiddleBound();

	virtual void BuildCollision()=0;

	virtual bool Collision(const Ogre::Vector3 &min,const Ogre::Vector3 &max,Ogre::Matrix4 &trx,Ogre::Vector3 &point,float &collDistance,Ogre::Vector3 &collNormal)=0;
	// collDistance==NULL ,solo test hit
	virtual bool Pick(Ogre::SceneNode *node,Ogre::Ray &ray,Ogre::Vector3 &point,float *collDistance,Ogre::Vector3 &collNormal)=0; 
	virtual void getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2)=0;
};

class ANIMA_CORE_API FullTriCollisionObject : public CollisionObject
{
friend class CollisionObjectStorage;
friend class ClonerModder_Area_Mask;
private:
	size_t numVertices; 
	size_t numFaces; 
	Ogre::Vector3* mVertexBuf; 
	size_t*   mFaceBuf; 
	int *layerIdxSuccFaces;

private:
	void convertMeshData(Ogre::Mesh *mesh,Ogre::Vector3* vertexBuf, size_t vertex_count,size_t * faceBuf, size_t index_count);
public:

	FullTriCollisionObject(MeshStatic *meshStatic);
	FullTriCollisionObject(SceneObject *obj,Ogre::Mesh *mesh,bool disposeMesh);
	virtual ~FullTriCollisionObject();

	virtual size_t GetNumVertices(){return numVertices;}
	virtual size_t GetNumFaces(){return numFaces;}

	virtual void BuildCollision();

	virtual void getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2);
	virtual bool Collision(const Ogre::Vector3 &min,const Ogre::Vector3 &max,Ogre::Matrix4 &trx,Ogre::Vector3 &point,float &collDistance,Ogre::Vector3 &collNormal);
	// collDistance==NULL ,solo test hit
	virtual bool Pick(Ogre::SceneNode *node,Ogre::Ray &ray,Ogre::Vector3 &point,float *collDistance,Ogre::Vector3 &collNormal); 
};

#ifdef USE_HW_COLLISION
class ANIMA_CORE_API TreeCollisionObject : public CollisionObject
{
friend class CollisionObjectStorage;
private:
	size_t numVertices; 
	size_t numFaces; 
	float *mVertexBuf; 
	size_t *mFaceBuf; 
	size_t *layerFaces;

	void *_opcModel;
	void *_opcMeshAccess;
	
	static bool init;
private:
	void convertMeshData(Ogre::Mesh *mesh,float * vertexBuf, size_t vertex_count,size_t * faceBuf, size_t index_count);
	//void getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2);

public:

	TreeCollisionObject(MeshStatic *meshStatic);
	TreeCollisionObject(SceneObject *obj,Ogre::Mesh *mesh,bool disposeMesh);
	virtual ~TreeCollisionObject();

	virtual size_t GetNumVertices(){return numVertices;}
	virtual size_t GetNumFaces(){return numFaces;}

	virtual void BuildCollision();

	virtual bool Collision(const Ogre::Vector3 &min,const Ogre::Vector3 &max,Ogre::Matrix4 &trx,Ogre::Vector3 &point,float &collDistance,Ogre::Vector3 &collNormal);
	// collDistance==NULL ,solo test hit
	virtual bool Pick(Ogre::SceneNode *node,Ogre::Ray &ray,Ogre::Vector3 &point,float *collDistance,Ogre::Vector3 &collNormal); 
	virtual void getTriCoords(size_t index, Ogre::Vector3& v0, Ogre::Vector3& v1, Ogre::Vector3& v2);
};
#endif

class ANIMA_CORE_API CollisionObjectStorage : public BObject
{
private:
	QList<CollisionObject *> list;
	QList<CollisionObject *> so_list;

	QMutex mutex;
public:

	CollisionObjectStorage();
	virtual ~CollisionObjectStorage();

	QList<CollisionObject *> &BeginSceneObjects();
	void EndSceneObjects();

	CollisionObject *Add(SceneObject *obj,Ogre::Mesh *mesh,bool disposeMesh);
	CollisionObject *Add(MeshStatic *meshStatic);

	void Remove(SceneObject *obj);
	void Remove(const Ogre::String &meshName);

	void NotifyDelete(SceneObject *obj);
	void Clear();

};

#include "Anima_nsend.h"

