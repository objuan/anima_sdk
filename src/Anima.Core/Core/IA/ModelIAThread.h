#pragma once

#include "Anima_nsbegin.h"

class ModelInstance;
class GfxClipAnim;
class MovePath;
class AnimClipModel;
//class ModelInstanceIAPath;

class ANIMA_CORE_API ModelIAThread : public QThread
{
protected:
	 bool restart;
     bool abort;
	 bool end;
	 bool waiting;
	 QMutex mutex;
     QWaitCondition condition;
	
protected:
	void run();

public:

	ModelIAThread();
	~ModelIAThread();

	bool IsBuilding();
	void Restart();
	void Abort();
	void WaitEnd();

	virtual void Process(bool)=0;

};

#include "Anima_nsend.h"