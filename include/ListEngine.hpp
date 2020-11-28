#pragma once

#include "Config.hpp"
#include "ListParser.hpp"
#include "SectionPanel.hpp"

class ListEngine : public Engine {

private:
    std::vector<SectionPanel *> panels;
    std::string layoutRatio;

    void createPanels();
    std::vector<Section> getSectionsFromList();
    std::string convertToAbsolutePath(std::string path);
    bool isRelativePath(std::string path);
    std::vector<Box> generateLayoutFromSections(std::vector<Section> sections);
    std::string removeTrailingColon(std::string ratioString);
    Box generateLayoutBounds();
    void populatePanels(std::vector<Section> sections, std::vector<Box> layout);
    void renderPanels();
    void resizePanels();

public:
    ~ListEngine();

    void init() override;
    void run() override;

};
