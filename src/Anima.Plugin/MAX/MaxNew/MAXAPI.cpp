#include "stdafx.h"

#include "MAXAnima.h"
#include "MAXApi.h"

#include "MeshObj.h"

//MAXAnima *maxAnima = NULL;

int __stdcall
MAX_API_DoImport(const char *programPath,const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts)
{
	QString fileName;
	//char buffer[9999];
	//size_t s =  wcstombs  ( buffer,name , wcslen(name));

	//fileName =  QString("%1").arg(buffer); //QString::fromLocal8Bit(name); // OKKIO, arriva non UNICODE, bo

	//if (maxAnima == NULL)
	//	maxAnima = new MAXAnima();

	return MAXAnima::Singleton()->Load(programPath,_MQ(name),i,gi,suppressPrompts == 1);
}
