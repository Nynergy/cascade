#include "ListEngine.hpp"

ListEngine::~ListEngine() {
    for(SectionPanel * panel : panels) {
        delete panel;
    }
}

void ListEngine::init() {
    try {
        createPanels();
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }
}

void ListEngine::createPanels() {
    try {
        std::vector<Section> sections = getSectionsFromList();
        std::vector<Box> layout = generateLayoutFromSections(sections);
        populatePanels(sections, layout);
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }
}

std::vector<Section> ListEngine::getSectionsFromList() {
    std::string listPath = Config::getInstance().getValueFromKey("MasterList");
    listPath = convertToAbsolutePath(listPath);
    ListParser parser = ListParser(listPath);
    std::vector<Section> sections = parser.parseList();

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

std::vector<Box> ListEngine::generateLayoutFromSections(std::vector<Section> sections) {
    int numSections = (int)sections.size();
    std::string ratioString = "";
    for(int i = 0; i < numSections; i++) {
        ratioString = ratioString + "1:";
    }

    layoutRatio = removeTrailingColon(ratioString);
    Box layoutBounds = generateLayoutBounds();
    std::vector<Box> layout;
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
        panels.push_back(panel);
    }
}

void ListEngine::run() {
    int key;
    while((key = getch()) != 'q') {
        // Handle input first, then render panels
        switch(key) {
            case KEY_RESIZE:
                resizePanels();
                break;
            default:
                break;
        }

        renderPanels();
    }
}

void ListEngine::renderPanels() {
    for(SectionPanel * panel : panels) {
        panel->drawPanel();
    }
}

void ListEngine::resizePanels() {
    Box layoutBounds = generateLayoutBounds();
    std::vector<Box> layout;

    try {
        layout = Layouts::customVLayout(layoutRatio, &layoutBounds);
    } catch(InvalidRatioException& e) {
        throw InvalidRatioException(e.what());
    }

    int numPanels = (int)layout.size();
    for(int i = 0; i < numPanels; i++) {
        panels[i]->resizePanel(layout[i]);
    }
}
