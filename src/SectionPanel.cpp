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
    std::string wideTitle = " " + title + " ";

    return wideTitle;
}

void SectionPanel::drawItems() {
    int offset = 0;
    for(std::string item : section.items) {
        offset++;
        drawItemWithOffset(item, offset);
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
        highlighted = false;
        if(offset == highlightIndex) {
            setAttributes(getAttribute("reverse"), win);
            highlighted = true;
        }
        offset++;
        drawItemWithOffset(item, offset);
        if(highlighted) {
            unsetAttributes(getAttribute("reverse"), win);
        }
    }
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
