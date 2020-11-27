#pragma once

#include <vector>

#include "FileProcessor.hpp"

// Each section keeps track of its own internals
struct Section {

    std::string title;
    int colorCode;
    std::vector<std::string> items;

    Section(std::string titleIn, int colorCodeIn) :
        title(titleIn), colorCode(colorCodeIn) {}

    void addItem(std::string itemIn) {
        items.push_back(itemIn);
    }

};

class ListParser {

private:
    FileProcessor * fproc;

    bool isSectionTitle(std::string line);
    std::string extractSectionTitle(std::string line);
    int extractColorCode(std::string line);
    std::string trimWhitespace(std::string str);
    Section parseSection(std::string sectionTitle, int sectionColorCode);
    bool isEndOfSection(std::string line);

public:
    ListParser(std::string listPath);
    ~ListParser();
    std::vector<Section> parseList();

};
