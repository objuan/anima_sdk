#pragma once

#include "Anima_nsbegin.h"
class DAG;
class SceneObject;

class ANIMA_CORE_API DAG : public BObject
{
private:

	QStringList pathList;

public:

	DAG(void);
	DAG(const DAG &dag);
	DAG(const QString &localPath);
	DAG(DAG &parentDag,const QString &localPath);
	virtual ~DAG();

	bool IsRoot();


	SceneObject *GetSceneObject();
	QList<SceneObject *> GetSceneObjectPath();

	QString ToString();
	Ogre::String ToOgreID();
};
#include "Anima_nsend.h"
