#pragma once

#include "Context.h"
#include "SceneObject.h"
#include "BuildDispatcher.h"

#include "Anima_nsbegin.h"

class QRandom: public BObject
{
	void *randomClass;
public:
	QRandom(int seed);
	~QRandom();
	int NextInt(int max);
	double Next();
};

class Modder;
enum ModderChildType
{
	ClonerModderType = 0,
	PhaserModderType = 1
};

class ModderCollection : public BObject
{
private:
	Animation *anim;
	QList<Modder *> list;
public:
	ModderCollection(Animation *anim);
	virtual ~ModderCollection(void);

	Modder *Get(int idx){return list[idx];}
	int Count(){return list.count();}
	int Find(ModderChildType type);
	Modder *Get(ModderChildType type);
	void Add(Modder *modder);
	void Remove(ModderChildType type);
	void Copy(ModderCollection *modderColl);

	void InvalidateBuild();
};

class ANIMA_CORE_API Modder  : public SceneObject ,public IMovePath_BuildElement
{
private:
	Animation *anim;
public:
	Modder(Animation *anim);
	virtual ~Modder(void);

	virtual AObject::Type GetType(){return AObject::ModderType;}
	virtual ModderChildType GetModderType() = 0;

	Animation *GetAnimation(){return anim;}

	virtual void OnInit(){}
	virtual void OnDelete(){}

	virtual DAG GetDAG();
	virtual void Copy(SceneObject *fromObj){}
	virtual SceneObject *Clone(SceneObject *newParent){return NULL;}

	virtual QString GetName(){return "";}
    virtual QString ClassDesc(){return "Modder";}
	virtual QString ClassName(){return "Modder";}

	virtual void Load(QDomElement e)=0;
	virtual void Save(QDomElement e)=0;

	virtual void Build();
	//virtual void InvalidateBuild(int type=0);
};
#include "Anima_nsend.h"

