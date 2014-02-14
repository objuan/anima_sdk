#pragma once

class ColladaOptions;

class ANIMA_CORE_API ColladaExporter 
{
public:
	ColladaExporter();
	virtual ~ColladaExporter(void);

	virtual void Save(const QString &filePath,ColladaOptions &options);
};
