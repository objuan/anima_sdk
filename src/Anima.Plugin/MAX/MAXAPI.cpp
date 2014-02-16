#include "stdafx.h"

#include "MAXAnima.h"
#include "MAXApi.h"

#include "MeshObj.h"

//MAXAnima *maxAnima = NULL;

int __stdcall
MAX_API_DoImport(const char *programPath,const char *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts)
{
	QString fileName;
	fileName =  QString::fromLocal8Bit(name); // OKKIO, arriva non UNICODE, bo

	//if (maxAnima == NULL)
	//	maxAnima = new MAXAnima();

	return MAXAnima::Singleton()->Load(programPath,fileName,i,gi,suppressPrompts == 1);
}
