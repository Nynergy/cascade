#include "Command.hpp"

Command::Command(State * state) : state(state) {}

void Command::clearBehindDialogForm() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box box(ul, lr);
    clearBox(box);
}

bool Command::checkForNumItems(int minimum) {
    SectionPanel * panel = state->getCurrentPanel();
    int numItems = panel->getNumItems();
    return numItems > minimum;
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
    if(state->userHasUnsavedChanges()) {
        setupDialog();
        bool saveFile = getUserChoice();
        if(saveFile) {
            Command * save = new SaveFileCommand(state);
            save->execute();
            delete save;
        }
        teardownDialog();
    }
	state->setExitFlag(true);
}

void QuitApplicationCommand::setupDialog() {
    dialog = new DialogForm("Save changes before quitting? (y/n)", state);
}

bool QuitApplicationCommand::getUserChoice() {
    bool userChoice = dialog->dialog();
    return userChoice;
}

void QuitApplicationCommand::teardownDialog() {
    clearBehindDialogForm();
    delete dialog;
}

SaveFileCommand::SaveFileCommand(State * state) : Command(state) {}

void SaveFileCommand::execute() {
    ListSerializer::serializeListToFile(state);
    state->changesSaved();
}

FocusPanelDownCommand::FocusPanelDownCommand(State * state) : Command(state) {}

void FocusPanelDownCommand::execute() {
	incrementCurrentPanel();
}

void FocusPanelDownCommand::incrementCurrentPanel() {
	int currentPanelIndex = state->getCurrentPanelIndex();
    int numPanels = (int)(state->getPanels()).size();
    currentPanelIndex = std::min(currentPanelIndex + 1, numPanels - 1);
	state->setCurrentPanel(currentPanelIndex);
}

FocusPanelUpCommand::FocusPanelUpCommand(State * state) : Command(state) {}

void FocusPanelUpCommand::execute() {
	decrementCurrentPanel();
}

void FocusPanelUpCommand::decrementCurrentPanel() {
	int currentPanelIndex = state->getCurrentPanelIndex();
    currentPanelIndex = std::max(currentPanelIndex - 1, 0);
	state->setCurrentPanel(currentPanelIndex);
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

JumpToBeginningCommand::JumpToBeginningCommand(State * state) : Command(state) {}

void JumpToBeginningCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    panel->moveToBeginningOfItems();
}

JumpToEndCommand::JumpToEndCommand(State * state) : Command(state) {}

void JumpToEndCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    panel->moveToEndOfItems();
}

CycleColorCommand::CycleColorCommand(State * state) : Command(state) {}

void CycleColorCommand::execute() {
    SectionPanel * panel = state->getCurrentPanel();
    panel->incrementColorCode();

    state->changesMade();
}

EditItemCommand::EditItemCommand(State * state) : Command(state) {}

void EditItemCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    setupEditBuffer();
    std::string input = getUserInput();
    changeItemName(input);
    teardownEditBuffer();

    state->changesMade();
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

    state->changesMade();
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

    state->changesMade();
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

    state->changesMade();
}

void NewSectionCommand::setupEditBuffer() {
    form = new DialogForm("New Section Title:", state);
}

std::string NewSectionCommand::getUserInput() {
    std::string userInput = form->edit();
    return userInput;
}

void NewSectionCommand::createNewSectionWithName(std::string name) {
    std::string colorStr = Config::getInstance().getValueFromKey("DefaultSectionColor");
    int colorCode = getCodeFromColorStr(colorStr);

    Section newSection(name, colorCode);
    std::vector<Section> sections = state->getSections();
    sections.push_back(newSection);
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);
}

int NewSectionCommand::getCodeFromColorStr(std::string colorStr) {
    if(colorStr == "red") {
        return 1;
    } else if(colorStr == "green") {
        return 2;
    } else if(colorStr == "yellow") {
        return 3;
    } else if(colorStr == "blue") {
        return 4;
    } else if(colorStr == "magenta") {
        return 5;
    } else if(colorStr == "cyan") {
        return 6;
    } else {
        return 7; // white is the default
    }
}

void NewSectionCommand::teardownEditBuffer() {
    clearBehindDialogForm();
    delete form;
}

DeleteItemCommand::DeleteItemCommand(State * state) : Command(state) {}

void DeleteItemCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    setupDialog();
    bool agree = getUserChoice();
    if(agree) {
        deleteCurrentItem();
        state->changesMade();
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
        state->changesMade();
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
    std::string colorStr = Config::getInstance().getValueFromKey("DefaultSectionColor");
    int colorCode = getCodeFromColorStr(colorStr);

    Section newSection("TODO", colorCode);
    std::vector<Section> sections = state->getSections();
    sections.push_back(newSection);
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);
}

int DeleteSectionCommand::getCodeFromColorStr(std::string colorStr) {
    if(colorStr == "red") {
        return 1;
    } else if(colorStr == "green") {
        return 2;
    } else if(colorStr == "yellow") {
        return 3;
    } else if(colorStr == "blue") {
        return 4;
    } else if(colorStr == "magenta") {
        return 5;
    } else if(colorStr == "cyan") {
        return 6;
    } else {
        return 7; // white is the default
    }
}

void DeleteSectionCommand::teardownDialogs() {
    clearBehindDialogForm();
    delete dialog1;
    delete dialog2;
}

ToggleMoveModeCommand::ToggleMoveModeCommand(State * state) : Command(state) {}

void ToggleMoveModeCommand::execute() {
    Mode currentMode = state->getMode();
    if(currentMode == Mode::MOVE) {
        state->setMode(Mode::NORMAL);
    } else {
        state->setMode(Mode::MOVE);
    }
}

MoveItemDownCommand::MoveItemDownCommand(State * state) : Command(state) {}

void MoveItemDownCommand::execute() {
    bool enoughItems = checkForNumItems(1);
    if(!enoughItems) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    panel->swapItemDown();

    state->changesMade();
}

MoveItemUpCommand::MoveItemUpCommand(State * state) : Command(state) {}

void MoveItemUpCommand::execute() {
    bool enoughItems = checkForNumItems(1);
    if(!enoughItems) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    panel->swapItemUp();

    state->changesMade();
}

MoveSectionDownCommand::MoveSectionDownCommand(State * state) : Command(state) {}

void MoveSectionDownCommand::execute() {
    int numPanels = state->getNumPanels();
    if(numPanels <= 1) { return; }

    state->swapPanelDown();

    Command * resize = new ResizeWindowCommand(state);
    resize->execute();
    delete resize;

    state->changesMade();
}

MoveSectionUpCommand::MoveSectionUpCommand(State * state) : Command(state) {}

void MoveSectionUpCommand::execute() {
    int numPanels = state->getNumPanels();
    if(numPanels <= 1) { return; }

    state->swapPanelUp();

    Command * resize = new ResizeWindowCommand(state);
    resize->execute();
    delete resize;

    state->changesMade();
}

ChangeItemSectionUpCommand::ChangeItemSectionUpCommand(State * state) : Command(state) {}

void ChangeItemSectionUpCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    int numPanels = state->getNumPanels();
    if(numPanels <= 1) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    std::string item = panel->getCurrentItem();
    panel->deleteCurrentItem();

    Command * focusUp = new FocusPanelUpCommand(state);
    focusUp->execute();
    delete focusUp;

    panel = state->getCurrentPanel();
    panel->addItem(item);

    state->changesMade();
}

ChangeItemSectionDownCommand::ChangeItemSectionDownCommand(State * state) : Command(state) {}

void ChangeItemSectionDownCommand::execute() {
    bool enoughItems = checkForNumItems(0);
    if(!enoughItems) { return; }

    int numPanels = state->getNumPanels();
    if(numPanels <= 1) { return; }

    SectionPanel * panel = state->getCurrentPanel();
    std::string item = panel->getCurrentItem();
    panel->deleteCurrentItem();

    Command * focusDown = new FocusPanelDownCommand(state);
    focusDown->execute();
    delete focusDown;

    panel = state->getCurrentPanel();
    panel->addItem(item);

    state->changesMade();
}
