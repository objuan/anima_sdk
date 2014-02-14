#pragma once

namespace Anima
{
	class Model;
}

class ANIMA_CORE_API ColladaModelReader
{
private:
	
	QString filePath;
	QDomDocument *doc;

	Anima::Model *model;

public:
	Anima::ModelInstance *currentModel;
	QString controllerName;

public:
	ColladaModelReader(Anima::Model *model);
	virtual ~ColladaModelReader(void);

	QString ModelPath();
	Anima::Model *GetModel(){return model;}

	void Open();
	void Close();

	QString GetRootNodeName();
	QString GetRootSkeletonName();
	QString GetControllerName(){return controllerName;}

	QDomElement Find(const QString &name);
	QList<QDomElement> FindAll(const QString &name);
	QDomElement FindByAttrib(QDomElement root,const QString attrib);
};
