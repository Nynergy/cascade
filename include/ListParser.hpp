#pragma once

#include <vector>

#include "FileProcessor.hpp"
#include "Section.hpp"

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
