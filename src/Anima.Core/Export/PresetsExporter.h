#pragma once

#include "./ParamsExporter.h"

#include "Anima_nsbegin.h"

enum PresetsExporterType
{
	PresetsExporterType_User,
	PresetsExporterType_Fixed,
};

class ANIMA_CORE_API PresetsExporter : public ModelExporterAPI
{
private:
	QList<ModelExporter *> exporterList;
	QString materialRenderPath;

public:

	QString Name;
	PresetsExporterType Type;

	bool ModelAll;
	QList<Model *> ModelList;

public:
	PresetsExporter(const QString &materialRenderPath);
	~PresetsExporter() ;

	virtual QString GetMaterialRenderPath(){return materialRenderPath;}

	bool HaveSingleMaterial(ModelExporterType type,int version,const QString &renderType  );
	void Load(QDomElement &parentEle);
	ModelExporter *GetExporter(ModelExporterType type,int version );
};

enum PresetsExporter_ViewportMode
{
	PresetsExporter_ViewportMode_SOLID= 0,
	PresetsExporter_ViewportMode_TEXTURED
	//PresetsExporter_ViewportMode_WIREFRAME
};

class ANIMA_CORE_API PresetsExporterManager : public BObject
{
private:
	void Search(const QString &path);
	QList<PresetsExporter *> presetList;
	
	PresetsExporter *currentPreset;
	bool haveSingleMaterial;
	PresetsExporter_ViewportMode viewportMode;

public:

	PresetsExporterManager();
	~PresetsExporterManager();

	void SetSingleMaterial(ModelExporterType type,int version,const QString &renderType  );
	bool HaveSingleMaterial( ){return haveSingleMaterial;}
	//bool HaveSingleMaterial(ModelExporterType type,int version,const QString &renderType  );
	PresetsExporter *GetCurrentPreset();
	void SetCurrentPreset(int idx);
	int GetCurrentPresetIdx();
	void SetCurrentPreset(const QString &preset);
	QList<PresetsExporter *> &GetPresetList(){return presetList;}

	PresetsExporter_ViewportMode GetViewportMode(){return viewportMode;}
	void SetViewportMode(PresetsExporter_ViewportMode mode){this->viewportMode=mode;}

	void Load();
};

#include "Anima_nsend.h"
