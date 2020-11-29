#include "Command.hpp"

Command::Command(State * state) : state(state) {}

NOPCommand::NOPCommand(State * state) : Command(state) {}

void NOPCommand::execute() {
	// Absolutely nothing
}

ResizeWindowCommand::ResizeWindowCommand(State * state) : Command(state) {}

void ResizeWindowCommand::execute() {
    std::vector<Section> sections = state->getSections();
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);
}

QuitApplicationCommand::QuitApplicationCommand(State * state) : Command(state) {}

void QuitApplicationCommand::execute() {
	state->setExitFlag(true);
}

FocusPanelDownCommand::FocusPanelDownCommand(State * state) : Command(state) {}

void FocusPanelDownCommand::execute() {
	incrementCurrentPanel();
}

void FocusPanelDownCommand::incrementCurrentPanel() {
	int currentPanelIndex = state->getCurrentPanelIndex();
	state->setCurrentPanel(++currentPanelIndex);
}

FocusPanelUpCommand::FocusPanelUpCommand(State * state) : Command(state) {}

void FocusPanelUpCommand::execute() {
	decrementCurrentPanel();
}

void FocusPanelUpCommand::decrementCurrentPanel() {
	int currentPanelIndex = state->getCurrentPanelIndex();
	state->setCurrentPanel(--currentPanelIndex);
}

ScrollDownCommand::ScrollDownCommand(State * state) : Command(state) {}

void ScrollDownCommand::execute() {
    SectionPanel * panel = state->getCurrentPanel();
	panel->scrollDown();
}

ScrollUpCommand::ScrollUpCommand(State * state) : Command(state) {}

void ScrollUpCommand::execute() {
    SectionPanel * panel = state->getCurrentPanel();
	panel->scrollUp();
}

CycleColorCommand::CycleColorCommand(State * state) : Command(state) {}

void CycleColorCommand::execute() {
    SectionPanel * panel = state->getCurrentPanel();
    panel->incrementColorCode();
}

EditBufferCommand::EditBufferCommand(State * state) : Command(state) {}

void EditBufferCommand::execute() {
    curs_set(1); // Make cursor visible while typing

    // FIXME This is just a test of the InputForm
    InputForm * inputForm = new InputForm("Enter a name:");
    int ch;
    bool exit = false;
    while(!exit) {
        inputForm->drawForm();
        ch = wgetch(inputForm->getWin());
        switch(ch) {
            case 10: // Enter Key
                {
                    char * str = inputForm->getInputFromBuffer();
                    std::string userInput = str;
                    drawStringAtPoint(str, Point(0, LINES - 1));
                    inputForm->returnFocus();
                    exit = true;
                }
                break;
            case KEY_F(1):
                {
                    exit = true;
                }
                break;
            default: // Delegate to form
                {
                    inputForm->handleInput(ch);
                }
                break;
        }
    }

    curs_set(0); // Make cursor invisible again
    delete inputForm;
}
