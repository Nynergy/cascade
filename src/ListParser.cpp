#include "ListParser.hpp"

ListParser::ListParser(std::string listPath) {
    fproc = new FileProcessor(listPath);
}

ListParser::~ListParser() {
    delete fproc;
}

std::vector<Section> ListParser::parseList() {
    std::vector<Section> sections;

    std::string line;
    while(!(fproc->isEOF)) {
        line = fproc->poll();
        if(isSectionTitle(line)) {
            std::string sectionTitle = extractSectionTitle(line);
            int sectionColorCode = extractColorCode(line);
            Section section = parseSection(sectionTitle, sectionColorCode);
            sections.push_back(section);
        }
    }

    return sections;
}

bool ListParser::isSectionTitle(std::string line) {
    // Section titles are of the format [Section Title] : ColorCode
    std::string firstChar = line.substr(0, 1);
    
    return firstChar == "[";
}

std::string ListParser::extractSectionTitle(std::string line) {
    size_t closingBrace = line.find(']');
    std::string title = line.substr(1, closingBrace - 1);

    return title;
}

int ListParser::extractColorCode(std::string line) {
    size_t colon = line.find(':');
    std::string codeStr = line.substr(colon + 1);
    std::string trimStr = trimWhitespace(codeStr);
    int code = stoi(trimStr);

    return code;
}

std::string ListParser::trimWhitespace(std::string str) {
	size_t first = str.find_first_not_of(' ');
	if(std::string::npos == first) {
		return str;
	}

	size_t last = str.find_last_not_of(' ');

	return str.substr(first, (last - first + 1));
}

Section ListParser::parseSection(std::string sectionTitle, int sectionColorCode) {
    Section section(sectionTitle, sectionColorCode);
    std::string line;
    while(!(fproc->isEOF)) {
        line = fproc->poll();
        if(isEndOfSection(line)) {
            break;
        }

        section.addItem(line);
    }

    return section;
}

bool ListParser::isEndOfSection(std::string line) {
    return line == "";
}
