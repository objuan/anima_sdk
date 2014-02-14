#pragma once

//#include "Anima.Import.h"
#include "Anima_nsbegin.h"
class Model;
class BaseModel;
class OgreBaseImporter;
class OgreModelImporter;

class ANIMA_CORE_API ImportClip: public BObject
{
public:
	QString Clip;
	int start;
	int end;
	bool isLoop;
	bool isOverlay;
};

class ANIMA_CORE_API ImportProfile: public BObject
{
public:
	
	QString name;
	QList<ImportClip> importClipList;
	QStringList PreNamefilter;
	int ImportFPS;
	QString Transform;

public:
	ImportProfile();
	virtual ~ImportProfile(void);

	void Load(QDomElement &parentEle);
};

//class Importer
//{
//public:
//	virtual void Save(Model *model)=0;
//};

class ANIMA_CORE_API ImportBaseModel: public BObject
{
public:
	
	QString modelFilePath;
	QList<ImportProfile *> importProfileList;
	OgreBaseImporter *importer;

public:
	ImportBaseModel(QString &modelFilePath);
	virtual ~ImportBaseModel(void);

	void Load(const QString &filePath,ImportProfile *importProfile);
	void Save(BaseModel *model);
};

class ANIMA_CORE_API ImportModel: public BObject
{
public:
	
	QString modelFilePath;
	OgreModelImporter *importer;

public:
	ImportModel();
	virtual ~ImportModel(void);

	void Load(const QString &filePath);
	void Save(Model *model);
};
#include "Anima_nsend.h"