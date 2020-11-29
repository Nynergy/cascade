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
        std::vector<Box> layout = generateLayoutFromSections(sections);
        populatePanels(sections, layout);
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

std::vector<Box> ListEngine::generateLayoutFromSections(std::vector<Section> sections) {
    int numSections = (int)sections.size();
    std::string ratioString = "";
    for(int i = 0; i < numSections; i++) {
        ratioString = ratioString + "1:";
    }

    layoutRatio = removeTrailingColon(ratioString);
    Box layoutBounds = generateLayoutBounds();
    std::vector<Box> layout;

    if(layoutRatio == "1") {
        layout.push_back(layoutBounds);
        
        return layout;
    }

    try {
        layout = Layouts::customVLayout(layoutRatio, &layoutBounds);
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }

    return layout;
}

std::string ListEngine::removeTrailingColon(std::string ratioString) {
    std::string trimmed = ratioString.substr(0, ratioString.size() - 1);

    return trimmed;
}

Box ListEngine::generateLayoutBounds() {
    Point ul(0, 0); Point lr(COLS - 1, LINES - 2);
    Box bounds(ul, lr);

    return bounds;
}

void ListEngine::populatePanels(std::vector<Section> sections, std::vector<Box> layout) {
    int numPanels = (int)sections.size();
    for(int i = 0; i < numPanels; i++) {
        SectionPanel * panel = new SectionPanel(layout[i], sections[i]);
        state->addPanel(panel);
    }
}

void ListEngine::run() {
    int key;
    while(state->userHasNotQuit()) {
        // Handle input first, then render panels
        key = getch();
        switch(key) {
            case KEY_RESIZE:
                try {
                    resizePanels();
                } catch(InvalidRatioException& e) {
                    throw InvalidRatioException(e.what());
                }
                break;
            default:
                handleInput(key);
                break;
        }

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

void ListEngine::resizePanels() {
    std::vector<Section> sections = state->getSections();
    std::vector<Box> layout;

    try {
        layout = generateLayoutFromSections(sections);
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }

    int numPanels = (int)layout.size();
    std::vector<SectionPanel *> panels = state->getPanels();
    for(int i = 0; i < numPanels; i++) {
        panels[i]->resizePanel(layout[i]);
    }
}

void ListEngine::handleInput(int key) {
    Command * command = commandFactory->getCommandFromKey(key);
    command->execute();
    delete command;
}
