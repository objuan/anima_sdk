#pragma once

#include "Item.h"

#include "Anima_nsbegin.h"

class ModelInstance;
class ModelInstanceEdit;
class QRandom;
class ModelIO;

class ANIMA_CORE_API ModelEditColor: public BObject
{
friend class ModelEdit;
private:
	QString name;
	QList<ModelEditColor *> linkList;
	//QString slinks;
public:
	int ogre_parameter;
	float alfa;
	int min_h;
	int max_h;
	int min_s;
	int max_s;
	int min_l;
	int max_l;
public:
	ModelEditColor();
	int GetOrder(){return ogre_parameter;}
	const QString &GetName(){return name; }
	QList<ModelEditColor *> &GetLinks(){return linkList;}
};

typedef QList<ModelEditColor *> ModelEditColorList;

class ANIMA_CORE_API ModelEdit : public BObject
{
private:

	QMap<QString,ModelEditColorList *> colorPartMap; // part -> color list
	QMap<QString,ModelEditColor *> colorMap; //color->name 
	ModelEditColorList editableColors; // editable colors list

public:

	ModelEdit(){}
	~ModelEdit();

	QMap<QString,ModelEditColorList *> &GetColorPartMap(){return colorPartMap;}

	QList<ModelEditColor *> *GetColors(const QString &partName);
	ModelEditColorList &GetEditableColors(){return editableColors;}
	int GetEditableColorCount(const QString &partName);

	void SetPartNames(const QStringList &nameList);
	ModelEditColor *AddColor(const QString &partName);
	void DelColor(ModelEditColor *color);
	void MoveColor(ModelEditColor *color,int step);
	void SetName(ModelEditColor *color,const QString &name);

	ModelEditColor *GetEditableColor(ModelEditColor *color);
	void SetEditableColor(ModelEditColor *color,ModelEditColor *editColor);
	void GetLinkableColors(ModelEditColor *color,QList< ModelEditColor*> &list);

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle,const QString &partName);
};

class ANIMA_CORE_API ModelInstanceEditColor: public BObject
{
friend class ModelInstanceEdit;
private:
	ModelEditColor *base_color;
	QColor color;
	ModelInstanceEdit *modelInstanceEdit;

public:
	
	ModelInstanceEditColor(ModelInstanceEdit *modelInstanceEdit);

	ModelEditColor *GetBaseColor(){return base_color;}
	ModelInstanceEdit *getModelInstanceEdit(){return modelInstanceEdit;}

	int GetParamNumber();
	QString GetName();
	QString GetNameWithLinks();
	Ogre::Vector4 GetVColor();
	QColor GetColor();
	void Compute();
	void SetColor(QColor &color);

	void OnBaseColorChanged();

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &modelEle );
};

typedef QList<ModelInstanceEditColor *> ModelInstanceEditColorList;

class ANIMA_CORE_API ModelInstanceEdit : public BObject
{
private:
	ModelInstance *modelInstance;
	QMap<QString,ModelInstanceEditColorList *> colorPartMap;
	bool changed;
	bool userDefined;

public:
	static QRandom *Random;

public:
	ModelInstanceEdit(ModelInstance *modelInstance);
	virtual ~ModelInstanceEdit(void);
	
	ModelEdit *GetModelEdit();
	ModelInstanceEditColor *GetEditColor(const QString &name);

	void Compute(bool fromGUI);
	bool IsChanged(){return changed;}
	void Apply(Ogre::Entity *entity);
	QColor GetColor(const QString &subMeshName,const QString &colorName);
	bool IsUserDefined(){return userDefined;}
	void SetUserDefined(){userDefined=true;changed=true;}

	void OnAddBaseColor(const QString &partName,ModelEditColor *color);
	void OnDelBaseColor(const QString &partName,ModelEditColor *color);

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &modelEle );
};

#include "Anima_nsend.h"
