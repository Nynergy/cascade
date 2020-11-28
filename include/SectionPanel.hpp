#pragma once

#include "vexes.hpp"

#include "Section.hpp"

class SectionPanel : public Panel {

private:
    Section section;
    int sectionColor;

    int convertColorCodeToAttribute(int code);
    void drawTitleBar();
    void drawBar();
    void drawTitle();
    void drawItems();
    void drawItemWithOffset(std::string item, int offset);

public:
    SectionPanel(Box globalDimensionsIn, Section sectionIn);

    void drawPanel() override;

};
