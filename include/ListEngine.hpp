#pragma once

#include "CommandFactory.hpp"
#include "ListParser.hpp"

class ListEngine : public Engine {

private:
    std::string listPath;
    State * state;
    CommandFactory * commandFactory;
    std::string layoutRatio;

    void createPanels();
    std::vector<Section> getSectionsFromList();
    std::string convertToAbsolutePath(std::string path);
    bool isRelativePath(std::string path);
    void passPanelsToState(std::vector<SectionPanel *> panels);
    void handleInput(int key);
    void renderPanels();
    void renderModeIndicator();
    void clearModeIndicator();

public:
    ListEngine(std::string listPathIn);
    ~ListEngine();

    void init() override;
    void run() override;

};
