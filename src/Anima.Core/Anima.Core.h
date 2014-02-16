#pragma once

#include <QString>
#include <QStringList>
#include <Ogre.h>

#ifdef WIN32
	#ifdef ANIMA_CORE_EXPORTS
		#define ANIMA_CORE_API __declspec(dllexport)
	#else
		#define ANIMA_CORE_API __declspec(dllimport)
	#endif
#else
	#define ANIMA_CORE_API
#endif
#define ANIMA_NAMESPACE_USE
// generici

// define LIST

// abilita monitor memoria di anima

#define USE_ANIMA_MEMORY 1 // senza vebe meglio le variabili non inizializzate

// abilita check memoria di anima
#ifdef DEBUG
//#define DEBUG_MEMORY 1
#endif

namespace Anima
{
ANIMA_CORE_API void _LogInfo(const QString &log,const QString &fun,long line);
ANIMA_CORE_API void _LogDebug(const QString &log,const QString &fun,long line);
ANIMA_CORE_API void _LogWarning(const QString &log,const QString &fun,long line);
ANIMA_CORE_API void _LogError(const QString &log,const QString &fun,long line);

#define LogDebug(log) _LogDebug(log,__FUNCTION__,__LINE__)
#define LogInfo(log) _LogInfo(log,__FUNCTION__,__LINE__)
#define LogWarning(log) _LogWarning(log,__FUNCTION__,__LINE__)
#define LogError(log) _LogError(log,__FUNCTION__,__LINE__)

class IMesh;
class SceneObject;

void __OnNew(void* ptr,size_t sz); 
void __OnDelete( void* ptr);

#ifdef USE_ANIMA_MEMORY

template <class Alloc>
class ANIMA_CORE_API AllocatedObject
{
public:
	explicit AllocatedObject()
	{ }

	virtual ~AllocatedObject()
	{ }

	/// operator new, with debug line info
	void* operator new(size_t sz, const char* file, int line, const char* func)
	{
		
		void* p= Alloc::allocateBytes(sz, file, line, func);
#ifdef DEBUG_MEMORY
		__OnNew(p,sz);
#endif
		return p;
	}

	void* operator new(size_t sz)
	{
		if (sz>10000 || sz <= 0)
		{
			int y=0;
		}
		void* p=  Alloc::allocateBytes(sz);
#ifdef DEBUG_MEMORY
		__OnNew(p,sz);
#endif
		return p;
	}

	/// placement operator new
	void* operator new(size_t sz, void* ptr)
	{
		return ptr;
	}

	/// array operator new, with debug line info
	void* operator new[] ( size_t sz, const char* file, int line, const char* func )
	{
		void* p=  Alloc::allocateBytes(sz, file, line, func);
#ifdef DEBUG_MEMORY
		__OnNew(p,sz);
#endif
		return p;
	}

	void* operator new[] ( size_t sz )
	{
		void *p =  Alloc::allocateBytes(sz);
#ifdef DEBUG_MEMORY
		__OnNew(p,sz);
#endif
		return p;
	}

	void operator delete( void* ptr )
	{
#ifdef DEBUG_MEMORY
		__OnDelete(ptr);
#endif
		Alloc::deallocateBytes(ptr);
	}

	// Corresponding operator for placement ANIMA_DEL (second param same as the first)
	void operator delete( void* ptr, void* )
	{
#ifdef DEBUG_MEMORY
		__OnDelete(ptr);
#endif
		Alloc::deallocateBytes(ptr);
	}

	// only called if there is an exception in corresponding 'new'
	void operator delete( void* ptr, const char* , int , const char*  )
	{
#ifdef DEBUG_MEMORY
		__OnDelete(ptr);
#endif
		Alloc::deallocateBytes(ptr);
	}

	void operator delete[] ( void* ptr )
	{
#ifdef DEBUG_MEMORY
		__OnDelete(ptr);
#endif
		Alloc::deallocateBytes(ptr);
	}

	void operator delete[] ( void* ptr, const char* , int , const char*  )
	{
#ifdef DEBUG_MEMORY
		__OnDelete(ptr);
#endif
		Alloc::deallocateBytes(ptr);
	}
};

typedef AllocatedObject<Ogre::GeneralAllocPolicy> DefaultAllocatedObject;

class ANIMA_CORE_API BObject: public DefaultAllocatedObject
{
public:
	virtual bool IsSceneObject(){return false;}
};

#else

class ANIMA_CORE_API BObject//: public DefaultAllocatedObject
{
public:
	virtual bool IsSceneObject(){return false;}
};

#endif

class ANIMA_CORE_API AObject: public BObject
{
public:

	enum Type
	{
		NullType=0,
		FilmLayerType = 1,
		AnimationType,
		AnimationPartType,
		AnimationPartClipSplineType,
		AnimationPartStairsType,
		AnimationPartClipPointType,
		AnimationPartEscalatorType,
		AnimClipType,
		MovePathType,
		SplineMovePathType,
		LinkSplineMovePathType,
		StairMovePathType,
		LinkStairMovePathType,
		PointMovePathType,
		LinkPointMovePathType,
		EscalatorMovePathType,
		ModelInstanceType,
		ModelInstanceMoveGfxType,
		MeshType,
		SceneObjectType,
		SceneObjectGfxType,
		//IAnimationPartClipDynamicType,
		AnimClipCollectionType,
		PathOffsetPointType,
		SubSceneObjectGfxType,
		MovePathPointType,
		AnimClipBoundType,
		MovePathStartOffsetType,
		MovePathMidOffsetType,
		MovePathRotateType,
		MovePathTranslateType,
		PointMovePathGeomType,
		MeshStaticType,
		SelectionManagerGfxMoveUIAxisType,
		ModderType,
		SelectionManagerGfxMoveUIBoundType,
		SelectionManagerGfxClonerFocusType
	};

	AObject(){}
	virtual ~AObject()
	{ }

	virtual AObject::Type GetType() = 0;
	virtual QString ClassName()= 0;
	virtual QString ClassDesc();

	bool IsA(AObject::Type type);
	static bool IsA(AObject::Type objType,AObject::Type type);
};

typedef struct CollisionPoint
{
	IMesh *mesh;
	float collDist;
	//int collTriIdx;
	Ogre::Vector3 tri_verts[3];
	Ogre::Vector3 collPoint;
	Ogre::Vector3 collNormal;
}CollisionPoint;


class MouseCollisionEvent
{
public:
	enum MouseButtonType
	{
		Left = 0,
		Right = 1,
	};

private:
	bool haveMesh;
	bool handle;
	bool pointHaveMesh;
	bool pointHaveOpengl;
	int mx;
	int my;
	int dragMx;
	int dragMy;
	MouseButtonType buttonType;
	CollisionPoint point;
	CollisionPoint lastDragPoint;
	CollisionPoint dragPoint;
	//MouseCollisionType collType;
	SceneObject *object;
	int subObjectIdx;
	QStringList args;
	Qt::KeyboardModifiers modifiers;
	Ogre::Vector2 mouseOffset;

public:

	MouseCollisionEvent()
	{
		object=NULL;
		subObjectIdx=0;
		mouseOffset = Ogre::Vector2::ZERO;
	}

	MouseCollisionEvent(MouseButtonType buttonType,int mx,int my,CollisionPoint &point,const Qt::KeyboardModifiers &m) : point(point),lastDragPoint(point)
	{
		handle=false;
		this->buttonType=buttonType;
		this->mx=mx;
		this->my=my;
		pointHaveOpengl=false;
		pointHaveMesh=haveMesh=true;
		dragPoint = point;
		modifiers=m;
		object=NULL;
		subObjectIdx=0;
		mouseOffset = Ogre::Vector2::ZERO;
	}
	MouseCollisionEvent(MouseButtonType buttonType,int mx,int my,CollisionPoint &point,SceneObject *object,int subObjectIdx,const Qt::KeyboardModifiers &m) :point(point),lastDragPoint(point),object(object),subObjectIdx(subObjectIdx)
	{
		this->buttonType=buttonType;
		this->mx=mx;
		this->my=my;
		pointHaveOpengl=true;
		pointHaveMesh=haveMesh=true;
		handle=false;
		modifiers=m;
		dragPoint = point;
		subObjectIdx=0;
		mouseOffset = Ogre::Vector2::ZERO;
	}
	MouseCollisionEvent(MouseButtonType buttonType,int mx,int my,SceneObject *object,int subObjectIdx,const Qt::KeyboardModifiers &m) :point(point),lastDragPoint(point),object(object),subObjectIdx(subObjectIdx)
	{
		this->buttonType=buttonType;
		this->mx=mx;
		this->my=my;
		pointHaveOpengl=true;
		pointHaveMesh=haveMesh=false;
		handle=false;
		modifiers=m;
		dragPoint = point;
		subObjectIdx=0;
		mouseOffset = Ogre::Vector2::ZERO;
	}
	const Qt::KeyboardModifiers GetModifiers(){ return modifiers;}
	int GetMouseX(){return mx;}
	int GetMouseY(){return my;}
	int GetDragMouseX(){return dragMx;}
	int GetDragMouseY(){return dragMy;}
	Ogre::Vector2 GetMouseOffset(){return mouseOffset;}
	void SetMouseOffset(Ogre::Vector2 &off){ mouseOffset=off;}
	MouseButtonType GetButtonType(){return buttonType;}
	void SetDrag(int mx,int my,CollisionPoint &collPoint); // mesh
	void SetDrag(int mx,int my); // solo opengl
	void SetUp();
	bool PickMesh(){return haveMesh;}
	bool IsHandle(){return handle;}
	void SetHandle(bool handle){this->handle=handle;}
	CollisionPoint &GetPoint(){return point;}
	CollisionPoint &GetLastDragPoint(){return lastDragPoint;}
	CollisionPoint &GetDragPoint(){return dragPoint;}
	bool IsA(AObject::Type type);
	bool IsType(AObject::Type type);
//	MouseCollisionType GetCollisionType(){return collType;}
	SceneObject *GetObject(){return object;}
    void SetObject(SceneObject * obj){object = obj;}
	int GetSubObjectIdx(){return subObjectIdx;}
	QStringList &GetArgs(){return args;}
};

enum ANIMA_CORE_API UnitMeter
{
		Inches =0,
		Feet,
		Miles,
		Millimeters,
		Centimeters,
		Meters,
		Kilometers
};

enum ANIMA_CORE_API FILE_ANI_PATH
{
	FILE_ANI_PATH_Absolute = 0,
	FILE_ANI_PATH_Relative = 1
};

#define DoCoreException(msg) throw CoreException(msg,__FUNCTION__,__LINE__)
#define AnimaDoCoreException(msg) throw Anima::CoreException(msg,__FUNCTION__,__LINE__)

#ifdef Q_OS_MAC
#define ANIMA_OSX_API_VISIBILITY __attribute__ ((visibility ("default")))
#else
#define ANIMA_OSX_API_VISIBILITY
#endif
	
class ANIMA_CORE_API ANIMA_OSX_API_VISIBILITY CoreException
{
private:
	
	QString message;

public:

	CoreException(const QString &_message,const QString &fun,long line) 
	{
		message = QString("%1 (%2:%3)").arg(_message).arg(fun).arg(line);
	}
	~CoreException(void){}

	QString Message(){return message;}
};
//
//template <typename T>
//class AList  : public BObject
//{
//public:
//	QList<T> List;
//public:
//
//	inline int count()
//	{
//		return List.count();
//	}
//
//	inline T &operator[](int i)
//	{
//		if (i<0 || i>= count()) 
//		{
//			DoCoreException("index out of bound");
//		}
//		return List[i];
//	}
//
//	inline const T &operator[](int i) const
//	{
//		if (i<0 || i>= count())
//		{
//			DoCoreException("index out of bound");
//		}
//		return List[i];
//	}
//
//	inline void removeAt(int i)
//	{
//		if (i<0 || i>= count()) 
//		{
//			DoCoreException("index out of bound");
//		}
//		List.removeAt(i);
//	}
//	inline void append(const T &t)
//	{
//		List.append(t);
//	}
//
//    inline void insert(int i, const T &t)
//	{
//		if (i<0 || i> count()) 
//		{
//			DoCoreException("index out of bound");
//		}
//		List.insert(i,t);
//	}
//
//	inline void clear()
//	{
//		List.clear();
//	}
//
//	int indexOf(const T &t, int from = 0) const
//	{
//		return List.indexOf(t,from);
//	}
//    int lastIndexOf(const T &t, int from = -1) const
//	{
//		return List.lastIndexOf(t,from);
//	}
//    QBool contains(const T &t) const
//	{
//		return List.contains(t);
//	}
//};


void __NotifyDelete(BObject *pointer);

}
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// PER OGRE 1.6

//#define OGRE_16
//#define _getUserAny(e) e->getUserAny()
//#define _setUserAny(e,v) e->setUserAny(Ogre::Any(v))

// per ogre 1.7
#define OGRE_17
#define OGRE_VERSION_17
#define _getUserAny(e) e->getUserObjectBindings().getUserAny()
#define _setUserAny(e,v) e->getUserObjectBindings().setUserAny(Ogre::Any(v))


