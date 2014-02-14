#pragma once
#include "Anima_nsbegin.h"
class Task;

#ifndef WIN32
#define _stdcall
#endif
typedef void (_stdcall *TaskQueueHandler)(Task *);

struct TaskQueueEntry
{
	TaskQueueEntry *prev;
	TaskQueueEntry *next;
	Task *task;
};
class ANIMA_CORE_API TaskQueue: public QThread  , public BObject
{
	Q_OBJECT
private:

	TaskQueueEntry *firstTask;
	TaskQueueEntry *lastTask;
	QMap<Task *,TaskQueueEntry *> taskMap;

	Task *currentTask;
	int total;
	bool cleaning;
	bool ancora;
	bool exit;
	QMutex mutex;
	QThread *task;
	bool renderMode;

	TaskQueueHandler handler;
private:

	void _Tick();
	void Process();

public:
	TaskQueue();
	virtual ~TaskQueue();

	bool Working(){return currentTask!=NULL;}
	int WaitCount(){return taskMap.count();}
	int Count(){return WaitCount() + (Working()? 1 : 0);}
	int Total(){return total;}

	// IO
	void Add(Task *task);
	void Remove(Task *task);

	void _NotifyStateChanged(Task *task);

	void End();
	void Clear();

	void WaitForComplete();

	void OnDeleting(Task *task);

	bool IsRenderMode(){return renderMode;}
	void SetRenderMode(bool renderMode);

	// thread

	void run();

	void Register_TaskQueueHandler(TaskQueueHandler handler);

signals:
	void StateChanged();
};

#include "Anima_nsend.h"
