#include "SectionPanel.hpp"

SectionPanel::SectionPanel(Box globalDimensionsIn, Section sectionIn) :
    Panel(globalDimensionsIn, sectionIn.title), section(sectionIn) {
    sectionColor = convertColorCodeToAttribute(section.colorCode);
}

int SectionPanel::convertColorCodeToAttribute(int code) {
    return COLOR_PAIR(code);
}

void SectionPanel::drawPanel() {
    clearScreen();
    drawTitleBar();
    drawItems();
    refreshWindow();
}

void SectionPanel::drawTitleBar() {
    int attr = combineAttributes(3, sectionColor,
                                    getAttribute("reverse"),
                                    getAttribute("bold"));
    setAttributes(attr, win);

    drawBar();
    drawTitle();

    unsetAttributes(attr, win);
}

void SectionPanel::drawBar() {
    Point a(0, 0); Point b(columns, 0);
    drawCustomHLineBetweenPoints(' ', a, b, win);
}

void SectionPanel::drawTitle() {
    int midX, midY;
    CENTER(win, midX, midY);
    Point titlePoint(midX, 0);
    drawCenteredStringAtPoint(title, titlePoint, win);
}

void SectionPanel::drawItems() {
    int offset = 0;
    for(std::string item : section.items) {
        offset++;
        drawItemWithOffset(item, offset);
    }
}

void SectionPanel::drawItemWithOffset(std::string item, int offset) {
    Point itemPoint(0, offset);
    drawStringAtPoint(item, itemPoint, win);
}
