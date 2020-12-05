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
    std::string truncateStringByLength(std::string str, int length);

public:
    SectionPanel(Box globalDimensionsIn, Section sectionIn);

    void drawPanel() override;
    void drawPanelFocused();
    void scrollDown();
    void scrollUp();
    void incrementHighlightIndex();
    void decrementHighlightIndex();
    Section getSection();
    std::string getSectionTitle();
    void setSectionTitle(std::string newTitle);
    std::string getCurrentItem();
    void setCurrentItem(std::string item);
    int getNumItems();
    void incrementColorCode();
};
