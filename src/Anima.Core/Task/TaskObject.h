#pragma once

//#include "Context.h"

#include "Anima_nsbegin.h"

class ANIMA_CORE_API TaskObject : public BObject
{
private:

	static long _number;
	static long activeNumber;
	//static QHash<long,TaskObject *> testMap;

	long number;
	QString name;
	bool disposed;

	int referenceCount;

public:
	TaskObject(const QString &name);
	virtual ~TaskObject(void);

	long Number(){return number;}

	void AddReference();
	void Dispose();
	bool IsDisposed(){return disposed;}

	static bool Check(TaskObject *obj);
	static QString Dump();

};
#include "Anima_nsend.h"

