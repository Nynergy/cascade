#include "ListEngine.hpp"

ListEngine::ListEngine(std::string listPathIn) : listPath(listPathIn) {
    state = new State();
    commandFactory = new CommandFactory(state);
}

ListEngine::~ListEngine() {
    delete state;
}

void ListEngine::init() {
    try {
        createPanels();
        state->setCurrentPanel(0);
    } catch(InvalidFileException& e) {
        throw InvalidFileException(e.what());
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }
}

void ListEngine::createPanels() {
    try {
        std::vector<Section> sections = getSectionsFromList();
        std::vector<SectionPanel *> panels = PanelConstructor::constructPanelsFromSections(sections);
        passPanelsToState(panels);
    } catch(InvalidFileException& e) {
        throw InvalidFileException(e.what());
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }
}

std::vector<Section> ListEngine::getSectionsFromList() {
    std::vector<Section> sections;

    try {
        listPath = convertToAbsolutePath(listPath);
        ListParser parser = ListParser(listPath);
        if(parser.missingList) {
            sections = defaultList();
        } else {
            sections = parser.parseList();
        }
    } catch(InvalidFileException& e) {
        throw InvalidFileException(e.what());
    }

    return sections;
}

std::string ListEngine::convertToAbsolutePath(std::string path) {
    if(isRelativePath(path)) {
        std::string userHome = getenv("HOME");
        std::string remaining = path.substr(1);
        std::string absPath = userHome + remaining;

        return absPath;
    } else {
        return path;
    }
}

bool ListEngine::isRelativePath(std::string path) {
    std::string firstChar = path.substr(0, 1);

    return firstChar == "~";
}

std::vector<Section> ListEngine::defaultList() {
    Section defaultSection("TODO", 7);
    std::vector<Section> sections;
    sections.push_back(defaultSection);

    return sections;
}

void ListEngine::passPanelsToState(std::vector<SectionPanel *> panels) {
    for(SectionPanel * panel : panels) {
        state->addPanel(panel);
    }
}

void ListEngine::run() {
    int key;
    while(state->userHasNotQuit()) {
        // Handle input first, then render panels
        key = getch();
        handleInput(key);
        renderPanels();
    }
}

void ListEngine::renderPanels() {
    for(SectionPanel * panel : state->getPanels()) {
        if(state->panelIsFocused(panel)) {
            panel->drawPanelFocused();
        } else {
            panel->drawPanel();
        }
    }
}

void ListEngine::handleInput(int key) {
    Command * command = commandFactory->getCommandFromKey(key);
    command->execute();
    delete command;
}
