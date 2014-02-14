#pragma once
#include "Anima_nsbegin.h"
class TaskQueue;

class ANIMA_CORE_API Task : public BObject//: public QThread
{
public:
	enum TaskState
	{
		Waiting =0,
		Working,
		Ready,
		Aborted,
		Error
	};
private:

	TaskQueue *taskQueue;
	TaskState state;

public:
	Task();
	virtual ~Task();

	TaskState GetState(){return state;}
	//virtual bool CanDelete(){return false;}

	void TaskStart(TaskQueue *taskQueue);
	void TaskEnd();
	void TaskAbort();

	virtual void TaskError();

	// azione
	virtual QString TaskName()=0;
	virtual void TaskRun()=0;

	// QThread
   // void run();

};
#include "Anima_nsend.h"
