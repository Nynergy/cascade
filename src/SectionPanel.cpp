#include "SectionPanel.hpp"

SectionPanel::SectionPanel(Box globalDimensionsIn, Section sectionIn) :
    Panel(globalDimensionsIn, sectionIn.title), section(sectionIn), highlightIndex(0),
    firstItemIndex(0) {
    sectionColor = convertColorCodeToAttribute(section.colorCode);
    lastItemIndex = std::min((int)section.items.size(), lines - 1);
}

int SectionPanel::convertColorCodeToAttribute(int code) {
    if(code < 1 || code > 7) { code = 7; }
    return COLOR_PAIR(code);
}

void SectionPanel::drawPanel() {
    clearScreen();
    drawTitleBar();
    drawItems();
    drawIndicators();
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
    int bound = std::min(lastItemIndex, getNumItems());
    for(int i = firstItemIndex; i < bound; i++) {
        std::string item = section.items[i];
        std::string truncItem = truncateStringByLength(item, columns - 2);
        offset++;
        drawItemWithOffset(truncItem, offset);
    }
}

void SectionPanel::drawItemWithOffset(std::string item, int offset) {
    // Draw item bar, then item name, so it spans the whole screen
    Point a(2, offset); Point b(columns - 2, offset);
    drawCustomHLineBetweenPoints(' ', a, b, win);

    Point itemPoint(2, offset);
    drawStringAtPoint(item, itemPoint, win);
}

void SectionPanel::drawIndicators() {
    drawVerticalIndicatorLines();

    if(firstItemIndex > 0) {
        drawUpperIndicators();
    }
    if(lastItemIndex < getNumItems()) {
        drawLowerIndicators();
    }
}

void SectionPanel::drawVerticalIndicatorLines() {
    Point a(0, 1); Point b(0, lines);
    drawVLineBetweenPoints(a, b, win);

    Point c(columns, 1); Point d(columns, lines);
    drawVLineBetweenPoints(c, d, win);
}

void SectionPanel::drawUpperIndicators() {
    Point p1(0, 1); Point p2(columns, 1);
    setAttributes(getAttribute("alternate"), win);
    drawCharAtPoint(ACS_UARROW, p1, win);
    drawCharAtPoint(ACS_UARROW, p2, win);
    unsetAttributes(getAttribute("alternate"), win);
}

void SectionPanel::drawLowerIndicators() {
    Point p1(0, lines); Point p2(columns, lines);
    setAttributes(getAttribute("alternate"), win);
    drawCharAtPoint(ACS_DARROW, p1, win);
    drawCharAtPoint(ACS_DARROW, p2, win);
    unsetAttributes(getAttribute("alternate"), win);
}

void SectionPanel::drawPanelFocused() {
    clearScreen();
    drawTitleBar();
    drawItemsWithHighlight();
    drawIndicators();
    refreshWindow();
}

void SectionPanel::drawItemsWithHighlight() {
    int offset = 0;
    int bound = std::min(lastItemIndex, getNumItems());
    bool highlighted;
    for(int i = firstItemIndex; i < bound; i++) {
        std::string item = section.items[i];
        std::string truncItem = truncateStringByLength(item, columns - 2);
        highlighted = false;
        if(i == highlightIndex) {
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
        std::string truncStr = str.substr(0, length - 4);
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

    highlightIndex = std::min(highlightIndex + 1, getNumItems() - 1);
    if(highlightIndex >= lastItemIndex) {
        firstItemIndex++;
        lastItemIndex++;
    }
}

void SectionPanel::decrementHighlightIndex() {
    if(section.items.size() == 0) {
        highlightIndex = -1;
        return;
    }

    highlightIndex = std::max(highlightIndex - 1, 0);
    if(highlightIndex < firstItemIndex) {
        firstItemIndex--;
        lastItemIndex--;
    }
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
        deleteCurrentItem();
    } else {
        section.items[highlightIndex] = item;
    }
}

void SectionPanel::deleteCurrentItem() {
    section.items.erase(section.items.begin() + highlightIndex);
    resetIndices();
}

void SectionPanel::resetIndices() {
    if(getNumItems() <= 0) {
        highlightIndex = -1;
    } else {
        highlightIndex = 0;
    }

    firstItemIndex = 0;
    lastItemIndex = std::min(getNumItems(), lines - 1);
}

int SectionPanel::getNumItems() {
    return (int)section.items.size();
}

void SectionPanel::addItem(std::string newItem) {
    if(newItem == "") { return; }

    section.items.push_back(newItem);
    moveToEndOfItems();
}

void SectionPanel::moveToBeginningOfItems() {
    highlightIndex = 0;
    firstItemIndex = 0;
    lastItemIndex = std::min(getNumItems(), lines);
}

void SectionPanel::moveToEndOfItems() {
    highlightIndex = (int)section.items.size() - 1;
    lastItemIndex = highlightIndex + 1;
    firstItemIndex = std::max(lastItemIndex - lines, 0);
}

void SectionPanel::incrementColorCode() {
    int code = (section.colorCode % 7) + 1;
    section.colorCode = code;
    sectionColor = convertColorCodeToAttribute(code);
}

void SectionPanel::swapItemDown() {
    if(highlightIndex == (getNumItems() - 1)) {
        return;
    }

    std::swap(section.items[highlightIndex], section.items[highlightIndex + 1]);
    incrementHighlightIndex();
}

void SectionPanel::swapItemUp() {
    if(highlightIndex == 0) {
        return;
    }

    std::swap(section.items[highlightIndex - 1], section.items[highlightIndex]);
    decrementHighlightIndex();
}
