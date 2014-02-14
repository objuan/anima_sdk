#pragma once

#include "ImportModel.h"

#include "Anima_nsbegin.h"
class ImportProfile;
class Model;
class BaseModel;

class ANIMA_CORE_API OgreModelImporter : public BObject
{
private:

	QString filePath;

public:
	OgreModelImporter(const QString &filePath);
	virtual ~OgreModelImporter(void);

	virtual void Save(Model *model);
};

Ogre::SkeletonPtr GetNormalizedSkeleton(BaseModel *model,Ogre::SkeletonPtr skeleton);

#include "Anima_nsend.h"