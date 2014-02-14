#pragma once

#include "Command.h"

class CommandManager;
class AnimationPart;
class CommandManager;
class FilmLayer;
class Animation;
class AnimationPartClipSpline;
class AnimationPartStairs;

class AddAnimation_CommandSeq : public Command
{
	QStringList descList;

	enum AddAnimation_State
	{
		DesignPath=0,
		AddActor=1,
		AddClip=2,
		Done
	};

private:

	bool addFilmLayer;
	FilmLayer *filmLayer;
	Animation *animation;
	AddAnimation_State state;

	void SetState();
public:

	AddAnimation_CommandSeq(FilmLayer *filmLayer);

	virtual void *Exec(CommandManager &cmdManager);
	virtual bool Event(Command &cmd,CommandManager &cmdManager);
	virtual void Abort(CommandManager &cmdManager,const QString &desc);

	QString GetDesc();
};

class AddClipSpline_CommandSeq : public Command
{
private:
	
	Animation *anim;
	AnimationPartClipSpline *animPart;
	QString state;

public:

	AddClipSpline_CommandSeq(Animation *anim);
	virtual void *Exec(CommandManager &cmdManager);
	virtual bool Event(Command &cmd,CommandManager &cmdManager);
	virtual void Abort(CommandManager &cmdManager,const QString &desc);
	virtual bool Undo();

	QString GetDesc();
};

class AddStairs_CommandSeq : public Command
{
private:
	
	Animation *anim;
	AnimationPartStairs *animPart;
	QString state;
	int widgetID;

public:

	AddStairs_CommandSeq(Animation *anim);
	virtual void *Exec(CommandManager &cmdManager);
	virtual bool Event(Command &cmd,CommandManager &cmdManager);
	virtual void Abort(CommandManager &cmdManager,const QString &desc);
	virtual bool Undo();

	QString GetDesc();
};



