#ifndef OGRE_UTIL_H
#define OGRE_UTIL_H

#include <OgreWireBoundingBox.h>

// Includes
//#include "global.h"
//#include "OgreOptions.h"
//#include <Ogre/OgreFrameListener.h>
//#include <Ogre/OgreTexture.h>


// Disable this define if you don't want to lock the Ogre rendering loop with a mutex,
// and don't want the dependency on the jthread library.
//#define USE_MUTEX

#define FLAG_PICKABLE 1<<10

#ifdef USE_MUTEX
#include <jthread/jmutexautolock.h>
#endif // USE_MUTEX

#include "Anima_nsbegin.h"

	// Forward declarations
	class SceneNode;
	class SceneObjectGfxMeshInfo;
	//typedef std::map<string, string> KeyValueMap;

	/** This is a class containing some static functions to ease some Ogre functionality.

		@internal Removed functionality:
		- No scene created (autoCreateScene option?)
		- No parented window (because this would not be auto-created by Ogre
		- No viewport and camera can be created (because no scene yet)
		- The createViewport function is gone (but wasn't that useful anyhow)
		- ChangeEntityTexture is gone (move to Ogre::Scene?)
	*/
class OgreUtil
	{
	protected:
	
#ifdef USE_MUTEX
		static JMutex m_sMutex; //!< A mutex to lock and unlock Ogre
#endif // USE_MUTEX

	public:

		// Picking
		static Ogre::MovableObject *pick(Ogre::Camera *pCamera, int winX, int winY, bool trianglePrecision = false, bool breakOnBlockingObject = false);
		static bool isPickable(const Ogre::MovableObject *pObject);
		static void setPickable(Ogre::MovableObject *pObject, bool pickable = true);

#ifdef USE_MUTEX
		// Locking functions
		static JMutexAutoLock autoLock() { return JMutexAutoLock(m_sMutex); }
		static void lock() { m_sMutex.Lock(); }
		static void unlock() { m_sMutex.Unlock(); }
#endif // USE_MUTEX

	public:
		// Protected functions
		static bool isHit(Ogre::Entity *pEntity,const Ogre::Ray &ray, float *pDistance = 0,int *subIdx=0);
		static void getMeshInformation(const Ogre::Mesh* const mesh, size_t &vertex_count,
									 Ogre::Vector3* &vertices,
									 size_t &index_count, unsigned long* &indices,
									 const Ogre::Vector3 &position,
									 const Ogre::Quaternion &orient,
									 const Ogre::Vector3 &scale);

		static void getSubMeshInformation(const Ogre::Mesh* const mesh, int subIdx,size_t &vertex_count,
									 Ogre::Vector3* &vertices,
									 size_t &index_count, unsigned long* &indices,
									 const Ogre::Vector3 &position,
									 const Ogre::Quaternion &orient,
									 const Ogre::Vector3 &scale);
	};


//class OBBoxRenderable : public Ogre::SimpleRenderable
//{
//	Ogre::MaterialPtr mat;
//	Ogre::VertexData vertexes;
//	public:
//		OBBoxRenderable(const QString &matName);
//		//void setupBoundingBox(const Ogre::AxisAlignedBox& aab);
//		//virtual ~OBBoxRenderable();
//		Ogre::Real getSquaredViewDepth(const Ogre::Camera*)const;
//		Ogre::Real getBoundingRadius()const;
//		virtual void getWorldTransforms (Ogre::Matrix4 *xform)const;
//
//		void SetColor(const QColor &col);
//		void   attach (Ogre::SceneNode *node);
//		 void detach (void);
//};


//class OBBoxRenderable : public Ogre::WireBoundingBox
//{
//	Ogre::MaterialPtr mat;
//	Ogre::String type;
//	public:
//		OBBoxRenderable(const QString &matName);
//		virtual ~OBBoxRenderable();
//		//virtual const Ogre::String  &getMovableType(){return type;}
//		void SetColor(const QColor &col);
//		void   attach (Ogre::SceneNode *node);
//		 void detach (void);
//};

class OBBoxRenderable
{
public:
	
	static bool init;
	Ogre::MaterialPtr mat;
	Ogre::Entity *entity;
	
public:
		OBBoxRenderable(const QString &matName);
		virtual ~OBBoxRenderable();
		void SetColor(const QColor &col);
		void  attach (Ogre::SceneNode *node,Ogre::AxisAlignedBox &bbox,SceneObjectGfxMeshInfo *info);
		 void detach (Ogre::SceneNode *node);

		 Ogre::SceneNode *getParentSceneNode();
};

#include "Anima_nsend.h"

#endif // OGRE_UTIL_H
