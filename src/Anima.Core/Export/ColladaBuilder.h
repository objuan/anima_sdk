#pragma once

class ColladaModelReader;

namespace Anima
{
	class ModelInstance;
}

class ANIMA_CORE_API ColladaBuilder
{
private:
	
	QString filePath;
	QFile *file;
	QXmlStreamWriter *writer;

	ColladaModelReader *currentReader;

private:
	QString FormatDate(QDateTime &d);
	void WriteElement(QDomElement ele,
		QString (*writeElementCallback)(ColladaModelReader &,const QString &,const QString &) = NULL,
		bool (*nodeElementCallback)(QDomElement &ele,ColladaModelReader &reader)=NULL);
	
	void WriteSource(const QString &id,const QString &param,QList<float> &values);

public:
	ColladaBuilder(const QString &filePath);
	virtual ~ColladaBuilder(void);

	void Open();
	void Close();

	void WriteStartScene();
	void WriteEndScene();

	void WriteAsset(const QString &comment);
	void WriteMaterials(QList<ColladaModelReader *> &list);
	void WriteGeometry(QList<ColladaModelReader *> &list);
	void WriteSkeleton(Anima::ModelInstance *model,ColladaModelReader *reader);
	void WriteInstance(Anima::ModelInstance *model,ColladaModelReader *reader);

	void WriteStartAnim();
	void WriteAnimPath(Anima::ModelInstance *model,ColladaModelReader *reader);
	void WriteEndAnim();

};
