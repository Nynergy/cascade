#include "CommandFactory.hpp"

CommandFactory::CommandFactory(State * state) : state(state) {}

Command * CommandFactory::getCommandFromKey(int key) {
	Command * command;
	switch(key) {
		case 'q':
			command = new QuitApplicationCommand(state);
			break;
		case 'J':
        case '\t':
			command = new FocusPanelDownCommand(state);
			break;
		case 'K':
        case KEY_BTAB:
			command = new FocusPanelUpCommand(state);
			break;
		case 'j':
			command = new ScrollDownCommand(state);
			break;
		case 'k':
			command = new ScrollUpCommand(state);
			break;
		default:
			command = new NOPCommand(state);
			break;
	}

	return command;
}
