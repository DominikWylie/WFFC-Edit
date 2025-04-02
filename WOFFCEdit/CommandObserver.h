#pragma once

#include "d3d11.h"
#include <SimpleMath.h>
#include <vector>

enum Command {
	undo,
	redo,
	copy,
	paste
};

class CommandObserver
{

public:

	virtual void commandCall(Command command) = 0;

	virtual std::vector<Command> getCommandsToObserve() = 0;

};

