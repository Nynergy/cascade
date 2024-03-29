#include "CommandFactory.hpp"

CommandFactory::CommandFactory(State * state) : state(state) {}

Command * CommandFactory::getCommandFromKey(int key) {
	Command * command;
    if(state->getMode() == Mode::NORMAL) {
        switch(key) {
            case KEY_RESIZE:
                command = new ResizeWindowCommand(state);
                break;
            case 'q':
                command = new QuitApplicationCommand(state);
                break;
            case 's':
                command = new SaveFileCommand(state);
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
            case 'g':
                command = new JumpToBeginningCommand(state);
                break;
            case 'G':
                command = new JumpToEndCommand(state);
                break;
            case 'c':
                command = new CycleColorCommand(state);
                break;
            case 'e':
                command = new EditItemCommand(state);
                break;
            case 'E':
                command = new EditSectionCommand(state);
                break;
            case 'n':
                command = new NewItemCommand(state);
                break;
            case 'N':
                command = new NewSectionCommand(state);
                break;
            case 'd':
                command = new DeleteItemCommand(state);
                break;
            case 'D':
                command = new DeleteSectionCommand(state);
                break;
            case 'm':
                command = new ToggleMoveModeCommand(state);
                break;
            default:
                command = new NOPCommand(state);
                break;
        }
    } else if(state->getMode() == Mode::MOVE) {
        switch(key) {
            case KEY_RESIZE:
                command = new ResizeWindowCommand(state);
                break;
            case 'q':
                command = new QuitApplicationCommand(state);
                break;
            case 's':
                command = new SaveFileCommand(state);
                break;
            case 'J':
                command = new MoveSectionDownCommand(state);
                break;
            case 'K':
                command = new MoveSectionUpCommand(state);
                break;
            case 'j':
                command = new MoveItemDownCommand(state);
                break;
            case 'k':
                command = new MoveItemUpCommand(state);
                break;
            case '<':
                command = new ChangeItemSectionUpCommand(state);
                break;
            case '>':
                command = new ChangeItemSectionDownCommand(state);
                break;
            case 'm':
                command = new ToggleMoveModeCommand(state);
                break;
            default:
                command = new NOPCommand(state);
                break;
        }
    }

	return command;
}
