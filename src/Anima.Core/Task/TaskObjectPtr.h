#pragma once

//#include "Context.h"

#include "Anima_nsbegin.h"

class ANIMA_CORE_API TaskObjectPtr
{
private:
	bool disposed;
	TaskObject *obj;

public:
	TaskObjectPtr(TaskObject *obj);
	virtual ~TaskObjectPtr(void);

	void Dispose();
	bool IsDisposed(){return disposed;}

};
#include "Anima_nsend.h"

