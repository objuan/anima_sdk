#include "stdafx.h"

#include "Command.h"

#include <tclap/CmdLine.h>
using namespace TCLAP;
using namespace std;

// ========================

#include "Anima_nsbegin.h"
class ParseOutput : public StdOutput
{
private:
	
	Command *command;

public:
	ParseOutput(Command *command);
	virtual void failure(CmdLineInterface& c, ArgException& e);

	virtual void usage(CmdLineInterface& c);

	virtual void version(CmdLineInterface& c);
};
#include "Anima_nsend.h"
