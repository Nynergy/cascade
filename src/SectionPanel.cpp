#include "SectionPanel.hpp"

SectionPanel::SectionPanel(Box globalDimensionsIn, Section sectionIn) :
    Panel(globalDimensionsIn, sectionIn.title), section(sectionIn), highlightIndex(0) {
    sectionColor = convertColorCodeToAttribute(section.colorCode);
}

int SectionPanel::convertColorCodeToAttribute(int code) {
    if(code < 1 || code > 7) { code = 7; }
    return COLOR_PAIR(code);
}

void SectionPanel::drawPanel() {
    clearScreen();
    drawTitleBar();
    drawItems();
    refreshWindow();
}

void SectionPanel::drawTitleBar() {
    int barAttr = combineAttributes(3, sectionColor,
                                       getAttribute("reverse"),
                                       getAttribute("bold"));
    setAttributes(barAttr, win);
    drawBar();
    unsetAttributes(barAttr, win);

    int titleAttr = combineAttributes(2, sectionColor,
                                         getAttribute("bold"));
    setAttributes(titleAttr, win);
    drawTitle();
    unsetAttributes(titleAttr, win);
}

void SectionPanel::drawBar() {
    Point a(0, 0); Point b(columns, 0);
    drawCustomHLineBetweenPoints(' ', a, b, win);
}

void SectionPanel::drawTitle() {
    int midX, midY;
    CENTER(win, midX, midY);
    Point titlePoint(midX, 0);

    std::string wideTitle = widenTitle();
    drawCenteredStringAtPoint(wideTitle, titlePoint, win);
}

std::string SectionPanel::widenTitle() {
    std::string wideTitle = " " + truncateStringByLength(title, columns - 5) + " ";

    return wideTitle;
}

void SectionPanel::drawItems() {
    int offset = 0;
    for(std::string item : section.items) {
        std::string truncItem = truncateStringByLength(item, columns - 2);
        offset++;
        drawItemWithOffset(truncItem, offset);
    }
}

void SectionPanel::drawItemWithOffset(std::string item, int offset) {
    // Draw item bar, then item name, so it spans the whole screen
    Point a(0, offset); Point b(columns, offset);
    drawCustomHLineBetweenPoints(' ', a, b, win);

    Point itemPoint(0, offset);
    drawStringAtPoint(item, itemPoint, win);
}

void SectionPanel::drawPanelFocused() {
    clearScreen();
    drawTitleBar();
    drawItemsWithHighlight();
    refreshWindow();
}

void SectionPanel::drawItemsWithHighlight() {
    int offset = 0;
    bool highlighted;
    for(std::string item : section.items) {
        std::string truncItem = truncateStringByLength(item, columns - 2);
        highlighted = false;
        if(offset == highlightIndex) {
            setAttributes(getAttribute("reverse"), win);
            highlighted = true;
        }
        offset++;
        drawItemWithOffset(truncItem, offset);
        if(highlighted) {
            unsetAttributes(getAttribute("reverse"), win);
        }
    }
}

std::string SectionPanel::truncateStringByLength(std::string str, int length) {
    int stringLength = str.size();
    if(stringLength >= length) {
        std::string truncStr = str.substr(0, length - 3);
        return truncStr + "...";
    }

    return str;
}

void SectionPanel::scrollDown() {
    incrementHighlightIndex();
}

void SectionPanel::scrollUp() {
    decrementHighlightIndex();
}

void SectionPanel::incrementHighlightIndex() {
    if(section.items.size() == 0) {
        highlightIndex = -1;
        return;
    }

    highlightIndex = (highlightIndex + 1) % section.items.size();
}

void SectionPanel::decrementHighlightIndex() {
    highlightIndex = highlightIndex == 0 ? section.items.size() - 1 : highlightIndex - 1;
}

Section SectionPanel::getSection() {
    return section;
}

std::string SectionPanel::getSectionTitle() {
    return section.title;
}

void SectionPanel::setSectionTitle(std::string newTitle) {
    // section.title is for file serialization, setTitle() is for Panel title
    section.title = newTitle;
    setTitle(newTitle);
}

std::string SectionPanel::getCurrentItem() {
    return section.items[highlightIndex];
}

void SectionPanel::setCurrentItem(std::string item) {
    int numItems = getNumItems();
    if(numItems <= 0) { return; }

    if(item == "") {
        // TODO: Move this into a deleteCurrentItem() method
        section.items.erase(section.items.begin() + highlightIndex);
    } else {
        section.items[highlightIndex] = item;
    }
}

int SectionPanel::getNumItems() {
    return (int)section.items.size();
}

void SectionPanel::incrementColorCode() {
    int code = (section.colorCode % 7) + 1;
    section.colorCode = code;
    sectionColor = convertColorCodeToAttribute(code);
}
