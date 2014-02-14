#pragma once

#include "../Anima.Core.h"
#include "Core/IModelRenderer.h"
#include "Anima_nsbegin.h"

class ModelInstance;
class Model;

class ModelInstanceMesh: public BObject
{
	ModelInstance *modelInstance;

	int indexV;
	int indexT;
	ModelInstanceMesh *next;
	ModelInstanceMesh *prev;

	ModelInstanceMesh();
};

class ANIMA_CORE_API OgreModelRenderer : public IModelRenderer 
{
private:

	ModelInstanceMesh *first;
	QMap<ModelInstance *,ModelInstanceMesh *> mapMesh;

private:

public:

	OgreModelRenderer();
	~OgreModelRenderer();

	void AddModel(ModelInstance *);
	void DelModel(ModelInstance *);

	void BeginRender();
	void EndRender();
	void Render(ModelInstance *);
};

#include "Anima_nsend.h"