#pragma once

#include "Anima_nsbegin.h"
typedef void *VoidP;
typedef char *CharP;

class CommandManager;
class Animation;
class AnimationPart;
class SceneObject;
class Command;

class ParseOutput;
class CmdLine;
class ProgressBar;

class CommandEvent
{
protected:
	Command *command;
public:

	CommandEvent(Command *command)
	{
		this->command=command;
	}
	// edit
	virtual void OnMouseDown(MouseCollisionEvent &coll){}
	virtual void OnMouseUp(MouseCollisionEvent &coll){}
	virtual void OnMouseDrag(MouseCollisionEvent &coll){}
	virtual void OnMouseMove(MouseCollisionEvent &coll){}
	
	virtual void OnAnimationPathChanged(AnimationPart *part){}
	virtual void OnAnimationClipChanged(AnimationPart *part){}
	virtual void OnChangedAnimation(Animation *anim){}
	virtual void OnEditModeChanged(int lev){}
	virtual void OnPropertyWidgetClosed(int id,bool ok,SceneObject *obj){}
};

enum CommandLogType
{
	CommandLogType_InputLog=0,
	CommandLogType_OutputLog,
	CommandLogType_InternalLog, // log interno al comando
	CommandLogType_ErrorLog,
	CommandLogType_WarningLog,
	CommandLogType_HelpLog
};

class CommandLogRow
{
public:
	CommandLogType type;
	QString msg;
	CommandLogRow(const QString &msg,CommandLogType type):msg(msg),type(type){}
};

class ANIMA_CORE_API CommandLogRowList
{
private:
	QList<CommandLogRow *> rows;
public:

	CommandLogRowList()
	{
	}
	CommandLogRowList(const CommandLogRowList &o);
	~CommandLogRowList()
	{
		qDeleteAll(rows);
	}

	CommandLogRow &get(int idx){return *rows[idx]; }
	void append(CommandLogRow *row){rows.append(row);}
	int count(){return rows.count();}
	void clear()
	{
		for(int i=0;i<rows.count();i++) delete rows[i];
		rows.clear();
	}
	void clearPart()
	{
		while(rows.count()>1000)
		{
			CommandLogRow *r = rows[0];
			rows.removeAt(0);
			delete r;
		}
	}
};

class ANIMA_CORE_API Command //: public BObject
{
	friend class CommandEvent;
	friend class ParseOutput;

public:

	enum CommandExitState
	{
		Running=0,
		Done,
		Aborted,
		Error,
		Help
	};

private:

	CommandLogRowList fullLog;
	CommandLogRowList log;
	CmdLine *cmdLine;
	//ParseOutput *parseOutput;
	CommandEvent *commandEvent;
	
protected:
	CommandManager *commandManager;
	
public:

	CommandExitState ExitState;
	QString OriginalCmd;

	VoidP ReturnObj; 
	QString Name;
	QList<QString> Args;

	QList<Command *> SubCommands;
	ProgressBar *progressBar;

private:
		
	static Command *Command_New(const QString &name);

protected:

	void SetCommandEvent(CommandEvent *commandEvent);

	CmdLine *GetCmdLine(const QString &usageMessage);
	void Parse();
	QString NormalizePath( QString &path);

public:
	Command();
	
	static Command *Get(CommandManager *commandManager,const QString &cmd);

	virtual ~Command(void);
	void Close();

	CommandEvent *GetCommandEvent(){return commandEvent;}
	CommandManager *GetCommandManager(){return commandManager;}

	bool HaveError();
	bool IsFinited();
	void ReturnError(const QString &errorDesc);
	void ReturnHelp(const QString &errorDesc);
	void ReturnAbort(const QString &abortDesc);
	void Return(const QString &desc="");

	void AddLog(const QString &log);
	CommandLogRowList  &ReadLog(){return log;}
	void EndReadLog();
	CommandLogRowList  &GetFullLog(){return fullLog;}

	void Execute();
	void Notify();

	virtual bool HaveLog(){return true;}
	virtual bool HaveUndo(){return true;}

	virtual bool IsPathCommand(){return false;}

	virtual VoidP Exec();
	virtual void Abort();
	virtual void Undo();
	virtual void Exit();
	virtual void Dispose(){}

	virtual QString ToString();
};
#include "Anima_nsend.h"
