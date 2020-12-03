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

EditItemCommand::EditItemCommand(State * state) : Command(state) {}

void EditItemCommand::execute() {
    curs_set(1); // Make cursor visible while typing
    setupEditBuffer();
    std::string input = getUserInput();
    changeItemName(input);
    teardownEditBuffer();
    curs_set(0); // Make cursor invisible again
}

void EditItemCommand::setupEditBuffer() {
    std::string item = getItemName();
    inputForm = new InputForm("Edit Item Name:");
    inputForm->injectString(item);
}

std::string EditItemCommand::getItemName() {
    SectionPanel * panel = state->getCurrentPanel();
    return panel->getCurrentItem();
}

std::string EditItemCommand::getUserInput() {
    std::string userInput;
    int ch;
    bool exit = false;
    while(!exit) {
        inputForm->drawForm();
        ch = wgetch(inputForm->getWin());
        switch(ch) {
            case 10: // Enter Key
                {
                    userInput = inputForm->getInputFromBuffer();
                    exit = true;
                }
                break;
            case KEY_F(1): // Cancel form input
                {
                    exit = true;
                }
                break;
            default: // Delegate to form driver
                {
                    inputForm->handleInput(ch);
                }
                break;
        }
    }

    return userInput;
}

void EditItemCommand::changeItemName(std::string input) {
    SectionPanel * panel = state->getCurrentPanel();
    panel->setCurrentItem(input);
}

void EditItemCommand::teardownEditBuffer() {
    delete inputForm;
}

EditSectionCommand::EditSectionCommand(State * state) : Command(state) {}

void EditSectionCommand::execute() {
    curs_set(1); // Make cursor visible while typing
    setupEditBuffer();
    std::string input = getUserInput();
    changeSectionName(input);
    teardownEditBuffer();
    curs_set(0); // Make cursor invisible again
}

void EditSectionCommand::setupEditBuffer() {
    std::string title = getSectionTitle();
    inputForm = new InputForm("Edit Section Title:");
    inputForm->injectString(title);
}

std::string EditSectionCommand::getSectionTitle() {
    SectionPanel * panel = state->getCurrentPanel();
    return panel->getSectionTitle();
}

std::string EditSectionCommand::getUserInput() {
    std::string userInput;
    int ch;
    bool exit = false;
    while(!exit) {
        inputForm->drawForm();
        ch = wgetch(inputForm->getWin());
        switch(ch) {
            case 10: // Enter Key
                {
                    userInput = inputForm->getInputFromBuffer();
                    exit = true;
                }
                break;
            case KEY_F(1): // Cancel form input
                {
                    exit = true;
                }
                break;
            default: // Delegate to form driver
                {
                    inputForm->handleInput(ch);
                }
                break;
        }
    }

    return userInput;
}

void EditSectionCommand::changeSectionName(std::string input) {
    SectionPanel * panel = state->getCurrentPanel();
    panel->setSectionTitle(input);
}

void EditSectionCommand::teardownEditBuffer() {
    delete inputForm;
}
