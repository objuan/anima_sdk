#pragma once
#include "Anima_nsbegin.h"
//class GfxModel;
class SubSceneObjectGfx;
class SceneObjectGfxMesh;

class SceneObjectGfxMesh_Entity: public BObject
{
public:
	SceneObjectGfxMesh *mesh;
	double baseScale;

	SceneObjectGfxMesh_Entity(){}
	SceneObjectGfxMesh_Entity(SceneObjectGfxMesh *mesh,double baseScale ) : mesh(mesh),baseScale(baseScale)
	{
	}
};

class ANIMA_CORE_API GfxZoomManager: public BObject
{
private:
	bool init;
public:

	QMap<SceneObjectGfxMesh *,SceneObjectGfxMesh_Entity> meshMap;

	GfxZoomManager();
	virtual ~GfxZoomManager();

	void Init();

	void Add(SceneObjectGfxMesh *mesh,double baseScale);
	void Remove(SceneObjectGfxMesh *mesh);

	void Update();

};


#include "Anima_nsend.h"