#include "Command.hpp"

Command::Command(State * state) : state(state) {}

void Command::clearBehindDialogForm() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box box(ul, lr);
    clearBox(box);
}

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
    form = new DialogForm("Edit Item Name:", state);
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
    clearBehindDialogForm();
    delete form;
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
    form = new DialogForm("Edit Section Title:", state);
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
    clearBehindDialogForm();
    delete form;
}

NewItemCommand::NewItemCommand(State * state) : Command(state) {}

void NewItemCommand::execute() {
    setupEditBuffer();
    std::string input = getUserInput();
    addItemToSection(input);
    teardownEditBuffer();
}

void NewItemCommand::setupEditBuffer() {
    form = new DialogForm("New Item Name:", state);
}

std::string NewItemCommand::getUserInput() {
    std::string userInput = form->edit();
    return userInput;
}

void NewItemCommand::addItemToSection(std::string newItem) {
    SectionPanel * panel = state->getCurrentPanel();
    panel->addItem(newItem);
}

void NewItemCommand::teardownEditBuffer() {
    clearBehindDialogForm();
    delete form;
}

NewSectionCommand::NewSectionCommand(State * state) : Command(state) {}

void NewSectionCommand::execute() {
    setupEditBuffer();
    std::string input = getUserInput();
    createNewSectionWithName(input);
    teardownEditBuffer();
}

void NewSectionCommand::setupEditBuffer() {
    form = new DialogForm("New Section Title:", state);
}

std::string NewSectionCommand::getUserInput() {
    std::string userInput = form->edit();
    return userInput;
}

void NewSectionCommand::createNewSectionWithName(std::string name) {
    Section newSection(name, 7); // TODO: Default color code will be config-specified
    std::vector<Section> sections = state->getSections();
    sections.push_back(newSection);
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);
}

void NewSectionCommand::teardownEditBuffer() {
    clearBehindDialogForm();
    delete form;
}

DeleteItemCommand::DeleteItemCommand(State * state) : Command(state) {}

void DeleteItemCommand::execute() {
    SectionPanel * panel = state->getCurrentPanel();
    int numItems = panel->getNumItems();
    if(numItems <= 0) { return; }

    setupDialog();
    bool agree = getUserChoice();
    if(agree) {
        deleteCurrentItem();
    }
    teardownDialog();
}

void DeleteItemCommand::setupDialog() {
    dialog = new DialogForm("Delete currently selected item? (y/n)", state);
}

bool DeleteItemCommand::getUserChoice() {
    bool userChoice = dialog->dialog();
    return userChoice;
}

void DeleteItemCommand::deleteCurrentItem() {
    SectionPanel * panel = state->getCurrentPanel();
    panel->deleteCurrentItem();
}

void DeleteItemCommand::teardownDialog() {
    clearBehindDialogForm();
    delete dialog;
}

DeleteSectionCommand::DeleteSectionCommand(State * state) : Command(state) {}

void DeleteSectionCommand::execute() {
    setupDialogs();
    bool agree = getUserChoice();
    if(agree) {
        deleteCurrentSection();
    }
    teardownDialogs();
}

void DeleteSectionCommand::setupDialogs() {
    dialog1 = new DialogForm("Delete currently focused section? (y/n)", state);
    dialog2 = new DialogForm("Really delete section? (y/n)", state);
}

bool DeleteSectionCommand::getUserChoice() {
    bool firstChoice = dialog1->dialog();
    if(firstChoice) {
        bool secondChoice = dialog2->dialog();
        return secondChoice;
    }

    return firstChoice;
}

void DeleteSectionCommand::deleteCurrentSection() {
    state->removeCurrentPanel();

    std::vector<SectionPanel *> panels = state->getPanels();
    int numPanels = (int)panels.size();
    if(numPanels <= 0) {
        addDefaultSection();
    }

    Command * resize = new ResizeWindowCommand(state);
    resize->execute();
    delete resize;
}

void DeleteSectionCommand::addDefaultSection() {
    Section newSection("TODO", 7); // TODO: Default color code will be config-specified
    std::vector<Section> sections = state->getSections();
    sections.push_back(newSection);
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);
}

void DeleteSectionCommand::teardownDialogs() {
    clearBehindDialogForm();
    delete dialog1;
    delete dialog2;
}
