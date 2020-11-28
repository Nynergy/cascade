#include "ListEngine.hpp"

ListEngine::~ListEngine() {
    for(SectionPanel * panel : panels) {
        delete panel;
    }
}

void ListEngine::init() {
    createPanels();
}

void ListEngine::createPanels() {
    std::vector<Section> sections = getSectionsFromList();
    std::vector<Box> layout = generateLayoutFromSections(sections);
    populatePanels(sections, layout);
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

    ratioString = removeTrailingColon(ratioString);
    Box layoutBounds = generateLayoutBounds();
    std::vector<Box> layout;
    try {
        layout = Layouts::customVLayout(ratioString, &layoutBounds);
    } catch(InvalidRatioException& e) {
        // FIXME This is horrible
        layout = Layouts::VSplit(&layoutBounds);
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
        renderPanels();
    }
}

void ListEngine::renderPanels() {
    for(SectionPanel * panel : panels) {
        panel->drawPanel();
    }
}
