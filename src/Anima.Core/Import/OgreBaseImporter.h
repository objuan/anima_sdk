#pragma once

#include "ImportModel.h"

#include "Anima_nsbegin.h"
class ImportProfile;
class BaseModel;

class ANIMA_CORE_API OgreBaseImporter: public BObject
{
private:

	QString filePath;
	ImportProfile *importProfile;

	Ogre::Vector3 CopyTranslate(const Ogre::Vector3 &src);

public:
	OgreBaseImporter(const QString &filePath,ImportProfile *importProfile);
	virtual ~OgreBaseImporter(void);

	void CopyAnim(Ogre::Animation *fromAnim,Ogre::Skeleton::BoneHandleMap &boneHandleMap,double startTime,double endTime,Ogre::SkeletonPtr toSkeleton, Ogre::Animation *toAnim,bool isLoop);

	virtual void Save(BaseModel *model);
};
#include "Anima_nsend.h"