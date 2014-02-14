#pragma once

#include "Anima_nsbegin.h"

class Model;
class ModelInstance;
class AnimClip;
class AnimationPartClipPoint;

class ANIMA_CORE_API Property: public BObject
{
public:
	QString propName;
	unsigned char type;

	int valueInt;
	QString valueStr;

	Property(){}
	Property(const QString &propName,int value):propName(propName),valueInt(value),type(0){}
	Property(const QString &propName,const QString value):propName(propName),valueStr(value),type(1){}

	int GetValueInt(){return valueInt;}
	QString GetValueStr(){return valueStr;}
	void SetValueInt(int v){valueInt=v;}
	void SetValueStr(const QString & v){valueStr=v;}

	void Load(QDomElement &ele);
	void Save(QDomDocument &doc,QDomElement &ele );
};

class ANIMA_CORE_API PropertyCollection: public BObject
{
private:
	PropertyCollection *parent;
	QMap<QString,Property *> propertyMap;

public:

	PropertyCollection(PropertyCollection *parent);
	virtual ~PropertyCollection();

	PropertyCollection *GetParent(){return parent;}

	int GetInt(const QString &propertyName,int defaultValue);
	int GetInt(const QString &propertyName,int defaultValue,bool &isParentValue);

	void Clear(const QString &propertyName);
	Property *Get(const QString &propertyName,bool *isParentValue);
	Property *Set(const QString &propertyName,int value ); // 

	void Clear();
	// IO
	void Load(QDomElement &ele,AnimationPartClipPoint *animPart);
	void Save(QDomDocument &doc,QDomElement &ele,ModelInstance *mi );
	void CopyFrom(PropertyCollection &coll);
};


#include "Anima_nsend.h"
