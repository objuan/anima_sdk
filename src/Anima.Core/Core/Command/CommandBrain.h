#pragma once

#include "Command.h"

class CommandManager;

class ANIMA_CORE_API CommandBrain
{
private:
	CommandManager *commandManager;

public:
	CommandBrain(CommandManager *commandManager);
	virtual ~CommandBrain(void);

	// torna true se posso continuare
	void *Evaluate(Command &cmd);
	
};
