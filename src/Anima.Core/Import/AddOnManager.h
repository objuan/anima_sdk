#pragma once

#include "Anima_nsbegin.h"

class Model;

class ANIMA_CORE_API AddOn: public BObject
{

};

class ANIMA_CORE_API AddOn_Model: public AddOn
{
};

class ANIMA_CORE_API AddOnManager: public BObject
{
private:

	void _Import(const QString &path,Model *model,bool simulate);

public:

	AddOnManager();
	
	void Import(const QString &path);
	void Load(const QString &path,Model *model);

	// -1 = file non presente , 0 = no , 1 = si
	int GetModelInfo(const QString &modelFolder,Model *model);

	QString CreateModelLibrary(const QString &libraryName,const QString &modelFolder,const QString &version,int modelID,const QString &destFolder);
};

#include "Anima_nsend.h"