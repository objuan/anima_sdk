#pragma once

#include "Anima_nsbegin.h"

enum ModelExporterType
{
	MAX = 0,
	LW = 1,
	C4D = 2,
	MAYA = 3
};

typedef QString ModelExporterRenderType; //All = tutti

enum ModelExportertextureType
{
	DIFFUSE = 0,
	NORMAL = 1
};

class ModelExporterRenderMaterial;
class ModelExporterRender;
class ModelExporter;
class ModelInstance;
class ModelExporterAPI;
class Model;

// ------------------------

class ANIMA_CORE_API ModelExporterRenderMaterialTexture: public BObject
{
private:
	ModelExporterRenderMaterial *mat;
public:
	ModelExportertextureType type;
	QString localPath;

	ModelExporterRenderMaterialTexture(ModelExporterRenderMaterial *mat):mat(mat){}
	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle);

	QString GetFullPath();
};

class ANIMA_CORE_API ModelExporterRenderMaterial: public BObject
{
	QList<ModelExporterRenderMaterialTexture *> txtList;
public:
	ModelExporterRender *render;
	QString meshSource; // All = tutti i pezzi
	QString materialFolderPath;
	QString rootRenderPath;
	QString seditablech;

	QString meshExternalName;
	QString meshExternalFile;
	QList<int> meshEditableChannels;
	QMap<int,QString> colorNameMap;

	ModelExporterRenderMaterial(ModelExporterRender *render,const QString &rootRenderPath);
	~ModelExporterRenderMaterial();

	QColor GetColor(ModelInstance *mi,int sub_position);

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle);

	ModelExporterRenderMaterialTexture *GetTexture(ModelExportertextureType type);
	QString GetFullMeshExternalFilePath();
};


class ANIMA_CORE_API ModelExporterRender: public BObject
{
public:
	ModelExporter *exporter;
	ModelExporterRenderType type; // All = tutti i render
	QString stype;
	QList<ModelExporterRenderMaterial *> matList;

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle);

	ModelExporterRender(ModelExporter *exporter):exporter(exporter){}
	~ModelExporterRender();

	ModelExporterRenderMaterial *FindMatBySource(const QString &meshSource);
	ModelExporterRenderMaterial *AddMaterial(const QString &meshSource);
};

// --------------------

class ANIMA_CORE_API ModelExporter: public BObject
{
	QMap<ModelExporterRenderType,ModelExporterRender *> renderMap;

public :
	ModelExporterType type;
	bool allVersion;
	QList<int> versionList;
	QString sversion;

	ModelExporterAPI *params;

	ModelExporter(ModelExporterAPI *params):params(params){}
	~ModelExporter() ;

	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle);
	ModelExporterRender *GetRender(ModelExporterRenderType type,int version );
	ModelExporterRender *AddRender(ModelExporterRenderType type,int version);

	QString GetRenderPath();
	static QString GetRenderTypeDesc(ModelExporterRenderType type);
	//static void GetModelExporterRenderTypeList(ModelExporterType type,int version,QList<ModelExporterRenderType> &list);
};

class ANIMA_CORE_API ModelExporterAPI : public BObject
{
public:
	virtual QString GetMaterialRenderPath()=0;
};

class ANIMA_CORE_API ModelExporterParams :  public ModelExporterAPI
{
private:
	//QMap<ModelExporterType,ModelExporter *> exporterMap;
	QList<ModelExporter *> exporterList;

public:
	QString materialRenderPath;

public:
	ModelExporterParams(const QString &materialRenderPath);
	ModelExporterParams(Model *model);
	~ModelExporterParams() ;

	virtual QString GetMaterialRenderPath(){return materialRenderPath;}
	void Load(QDomElement &parentEle);
	void Save(QDomDocument &doc,QDomElement &parentEle);
	ModelExporter *GetExporter(ModelExporterType type,int version );
	ModelExporter *AddExporter(ModelExporterType type,int version);
};

#include "Anima_nsend.h"
