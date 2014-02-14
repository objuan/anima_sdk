#pragma once


#include "Anima_nsbegin.h"

class MeshSceneObject;
class Animation;
class AnimationPart;
class SceneBlock;
class SceneLibrary;

enum SceneLibraryElementAccess{
	SceneLibraryElementAccess_MOVE_PATH=1,
	SceneLibraryElementAccess_ALTER_PATH=2,
	SceneLibraryElementAccess_CHANGE_ACTOR=4,
	SceneLibraryElementAccess_ALTER_ACTOR=8,
	SceneLibraryElementAccess_FULL_ACCESS = 1+2+4+8
};

enum SceneBlockType{
	SceneBlockType_PATH=1,
	SceneBlockType_ANIMATION=2,
	SceneBlockType_LAYER=3,
	SceneBlockType_SCENE=4
};

class ANIMA_CORE_API SceneBlockElement
{
private:
	SceneBlock *sceneBlock;

public:
	SceneBlockElement();

	bool HasAccess(SceneLibraryElementAccess access);

	SceneBlock *GetSceneBlock(){return sceneBlock;}
	void SetSceneBlock(SceneBlock *sceneBlock){ this->sceneBlock = sceneBlock;}
	bool HasSceneBlock(){return sceneBlock!=NULL;}
};

class ANIMA_CORE_API SceneBlock
{
private:
	SceneLibrary *library;
	int access; 
	SceneBlockType type;
	QString name;
	QString description;
	QString version;

public:
	SceneBlock(const QString name,SceneLibrary *library);
	virtual ~SceneBlock(){}

	QString GetName(){return name;}
	void SetName(const QString &name){ this->name=name;}

	QString GetDescription(){return description;}
	void SetDescription(const QString &description){ this->description=description;}

	QString GetVersion(){return version;}
	void SetVersion(const QString &version){ this->version=version;}

	SceneBlockType GetType(){return type;}
	void SetType(SceneBlockType type){this->type=type;}

	// access
	void SetAccess(SceneLibraryElementAccess access,bool value);
	bool HasAccess(SceneLibraryElementAccess access);

	void ImportTo(SceneObject *root);

	// IO
	virtual void Load(QDomElement e);
	virtual void Save(QDomElement e);
};

class ANIMA_CORE_API SceneLibrary
{
private:
	QString name;
	QString description;
	QString version;
	QList<SceneBlock *> blockList;

public:
	SceneLibrary(const QString name);
	~SceneLibrary();

	QString GetName(){return name;}
	void SetName(const QString &name){ this->name=name;}

	QString GetDescription(){return description;}
	void SetDescription(const QString &description){ this->description=description;}

	QString GetVersion(){return version;}
	void SetVersion(const QString &version){ this->version=version;}

	void AddBlock(SceneBlock *block);
	void RemoveBlock(SceneBlock *block);
	QList<SceneBlock *> &GetBlockList(){return blockList;}
};

class ANIMA_CORE_API SceneLibraryManager
{
private:
	QList<SceneLibrary *> libraryList;
public:
	void Load();
	QList<SceneLibrary *> GetlibraryList(){return libraryList;}
	SceneLibrary *GetLibrary(const QString &name);
};
#include "Anima_nsend.h"

