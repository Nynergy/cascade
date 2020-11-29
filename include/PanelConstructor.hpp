#pragma once

#include "SectionPanel.hpp"

class PanelConstructor {

private:
    static std::vector<Box> generateLayoutFromSections(std::vector<Section> sections) {
        int numSections = (int)sections.size();
        std::string ratioString = "";
        for(int i = 0; i < numSections; i++) {
            ratioString = ratioString + "1:";
        }

        ratioString = removeTrailingColon(ratioString);
        Box layoutBounds = generateLayoutBounds();
        std::vector<Box> layout;

        if(ratioString == "1") {
            layout.push_back(layoutBounds);
            
            return layout;
        }

        try {
            layout = Layouts::customVLayout(ratioString, &layoutBounds);
        } catch(InvalidRatioException& e) {
            throw InvalidRatioException(e.what());
        }

        return layout;
    }

    static std::string removeTrailingColon(std::string ratioString) {
        std::string trimmed = ratioString.substr(0, ratioString.size() - 1);

        return trimmed;
    }

    static Box generateLayoutBounds() {
        Point ul(0, 0); Point lr(COLS - 1, LINES - 2);
        Box bounds(ul, lr);

        return bounds;
    }

    static std::vector<SectionPanel *> populatePanels(std::vector<Section> sections, std::vector<Box> layout) {
        std::vector<SectionPanel *> panels;
        int numPanels = (int)sections.size();
        for(int i = 0; i < numPanels; i++) {
            SectionPanel * panel = new SectionPanel(layout[i], sections[i]);
            panels.push_back(panel);
        }

        return panels;
    }

public:
    static std::vector<SectionPanel *> constructPanelsFromSections(std::vector<Section> sections) {
        std::vector<SectionPanel *> panels;

        try {
            std::vector<Box> layout = generateLayoutFromSections(sections);
            panels = populatePanels(sections, layout);
        } catch(InvalidRatioException& e) {
            throw InvalidRatioException(e.what());
        }

        return panels;
    }

};
