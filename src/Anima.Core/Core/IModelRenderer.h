#pragma once

#include "../Anima.Core.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class Model;

class ANIMA_CORE_API IModelRenderer : public BObject
{
public:

	virtual void AddModel(ModelInstance *)=0;
	virtual void DelModel(ModelInstance *)=0;

	virtual void BeginRender()=0;
	virtual void EndRender()=0;
	virtual void Render(ModelInstance *)=0;
};

#include "Anima_nsend.h"