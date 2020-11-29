#pragma once

#include "Command.hpp"

class CommandFactory {

private:
	State * state;

public:
	CommandFactory(State * state);
	Command * getCommandFromKey(int key);

};
