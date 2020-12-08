#include "ListEngine.hpp"

ListEngine::ListEngine(std::string listPathIn) : listPath(listPathIn) {
    state = new State(listPathIn);
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
        sections = parser.parseList();
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
        renderModeIndicator();
    }
}

void ListEngine::handleInput(int key) {
    Command * command = commandFactory->getCommandFromKey(key);
    command->execute();
    delete command;
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

void ListEngine::renderModeIndicator() {
    clearModeIndicator();

    Mode currentMode = state->getMode();
    switch(currentMode) {
        case Mode::NORMAL:
            break;
        case Mode::MOVE:
            {
                Point modePoint(COLS - 11, LINES - 1);
                setAttributes(getAttribute("reverse"));
                drawStringAtPoint(" MOVE MODE ", modePoint);
                unsetAttributes(getAttribute("reverse"));
            }
            break;
        default:
            break;
    }
}

void ListEngine::clearModeIndicator() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box modeBox(ul, lr);
    clearBox(modeBox);
}
