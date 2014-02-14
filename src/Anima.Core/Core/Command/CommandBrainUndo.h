#pragma once

#include "Command.h"

class CommandManager;

class ANIMA_CORE_API CommandBrainUndo
{
private:
	CommandManager *commandManager;

public:
	CommandBrainUndo(CommandManager *commandManager);
	virtual ~CommandBrainUndo(void);

	// torna true se ho processato l'undo
	bool Evaluate(Command &cmd);
	
};
