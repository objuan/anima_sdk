#pragma once

#include "Anima_nsbegin.h"

class QVideo;

class ANIMA_CORE_API VideoManagerThread : public QThread
{
protected:
	 bool restart;
     bool abort;
	 bool end;
	 bool waiting;
	 bool firstTimeToken;
	 QMutex mutex;
     QWaitCondition condition;
	
protected:
	void run();

public:

	VideoManagerThread();
	~VideoManagerThread();

	bool IsBuilding();
	void Restart();
	void Abort();
	void WaitEnd();

	virtual void Process(bool)=0;

};


class ANIMA_CORE_API VideoManager : public VideoManagerThread
{
	Q_OBJECT
private:
	int currentFrame;
	QMutex mutex;
	QWaitCondition renderCondition;

	QString fileFolder;
	QString filePrefix;
	bool imageMode;
	QVideo *videoEncoder;
	int videoW;
	int videoH;

private:

	void OpenSaveStream();
	void SaveFrame();
	void CloseSaveStream();

public:
	VideoManager();
	~VideoManager() ;

	virtual void Process(bool checkAbort);

	void StartRender();
	void StopRender();

	void NotifyFrameRender();

signals:
	void OnRenderProgress(int frame,int total,bool done);
};

#include "Anima_nsend.h"
