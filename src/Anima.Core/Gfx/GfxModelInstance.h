#pragma once

#include "Anima_nsbegin.h"
class GfxModel;

class ANIMA_CORE_API GfxModelInstance
{
private:
	
	GfxModel *model;
	QString name;

public:

	GfxModelInstance(GfxModel *model,const QString &name);
	~GfxModelInstance(void);

	QString GetName(){return name;}

};
#include "Anima_nsend.h"