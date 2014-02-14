#pragma once

#include "Task.h"

#include "Anima_nsbegin.h"

class TaskObject;

class ANIMA_CORE_API DeleteTask : public Task
{
private:
	TaskObject *taskObject;

public:

	DeleteTask(TaskObject *taskObject);
	virtual ~DeleteTask();

	virtual bool CanDelete(){return true;}

	virtual QString TaskName(){return "Delete";}

	virtual void TaskRun();

};
#include "Anima_nsend.h"
