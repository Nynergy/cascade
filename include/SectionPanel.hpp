#pragma once

#include "vexes.hpp"

#include "Section.hpp"

class SectionPanel : public Panel {

private:
    Section section;
    int sectionColor;
    int highlightIndex;

    int convertColorCodeToAttribute(int code);
    void drawTitleBar();
    void drawBar();
    void drawTitle();
    std::string widenTitle();
    void drawItems();
    void drawItemWithOffset(std::string item, int offset);
    void drawItemsWithHighlight();

public:
    SectionPanel(Box globalDimensionsIn, Section sectionIn);

    void drawPanel() override;
    void drawPanelFocused();
    void incrementHighlightIndex();
    void decrementHighlightIndex();

};
