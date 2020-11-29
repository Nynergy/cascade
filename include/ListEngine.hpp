#pragma once

#include "CommandFactory.hpp"
#include "Config.hpp"
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
    std::vector<Section> defaultList();
    void passPanelsToState(std::vector<SectionPanel *> panels);
    void renderPanels();
    void handleInput(int key);

public:
    ListEngine(std::string listPathIn);
    ~ListEngine();

    void init() override;
    void run() override;

};
