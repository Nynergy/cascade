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
    SectionPanel * panel = state->getCurrentPanel();
    int numItems = panel->getNumItems();
    if(numItems <= 0) { return; }

    setupEditBuffer();
    std::string input = getUserInput();
    changeItemName(input);
    teardownEditBuffer();
}

void EditItemCommand::setupEditBuffer() {
    std::string item = getItemName();
    Point formPoint(0, LINES - 1);
    form = new Form(formPoint, "Edit Item Name:");
    form->injectString(item);
}

std::string EditItemCommand::getItemName() {
    SectionPanel * panel = state->getCurrentPanel();
    return panel->getCurrentItem();
}

std::string EditItemCommand::getUserInput() {
    std::string userInput = form->edit();
    return userInput;
}

void EditItemCommand::changeItemName(std::string input) {
    SectionPanel * panel = state->getCurrentPanel();
    panel->setCurrentItem(input);
}

void EditItemCommand::teardownEditBuffer() {
    clearBehindForm();
    delete form;
}

void EditItemCommand::clearBehindForm() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box formBox(ul, lr);
    clearBox(formBox);
}

EditSectionCommand::EditSectionCommand(State * state) : Command(state) {}

void EditSectionCommand::execute() {
    setupEditBuffer();
    std::string input = getUserInput();
    changeSectionName(input);
    teardownEditBuffer();
}

void EditSectionCommand::setupEditBuffer() {
    std::string title = getSectionTitle();
    Point formPoint(0, LINES - 1);
    form = new Form(formPoint, "Edit Section Title:");
    form->injectString(title);
}

std::string EditSectionCommand::getSectionTitle() {
    SectionPanel * panel = state->getCurrentPanel();
    return panel->getSectionTitle();
}

std::string EditSectionCommand::getUserInput() {
    std::string userInput = form->edit();
    return userInput;
}

void EditSectionCommand::changeSectionName(std::string input) {
    SectionPanel * panel = state->getCurrentPanel();
    panel->setSectionTitle(input);
}

void EditSectionCommand::teardownEditBuffer() {
    clearBehindForm();
    delete form;
}

void EditSectionCommand::clearBehindForm() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box formBox(ul, lr);
    clearBox(formBox);
}
