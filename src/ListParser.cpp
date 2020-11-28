#include "ListParser.hpp"

ListParser::ListParser(std::string listPath) {
    try {
        fproc = new FileProcessor(listPath);
    } catch(InvalidFileException& e) {
        missingList = true;
    }
}

ListParser::~ListParser() {
    delete fproc;
}

std::vector<Section> ListParser::parseList() {
    std::vector<Section> sections;

    bool fileIsEmpty = true;
    bool firstLine = true;
    std::string line;
    while(!(fproc->isEOF)) {
        fileIsEmpty = false;
        line = fproc->poll();
        if(isSectionTitle(line) || firstLine) {
            firstLine = false;
            try {
                std::string sectionTitle = extractSectionTitle(line);
                int sectionColorCode = extractColorCode(line);
                Section section = parseSection(sectionTitle, sectionColorCode);
                sections.push_back(section);
            } catch(InvalidFileException& e) {
                throw InvalidFileException(e.what());
            }
        }
    }

    if(fileIsEmpty) {
        const char * message = "List file cannot be empty.";
        throw InvalidFileException(message);
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
    if(closingBrace == std::string::npos) {
        const char * message = "Section titles must be enclosed in braces (e.g. [Section Title]).";
        throw InvalidFileException(message);
    }

    std::string title = line.substr(1, closingBrace - 1);

    return title;
}

int ListParser::extractColorCode(std::string line) {
    // Remove title portion first, then check for colon and integer
    size_t closingBrace = line.find(']');
    line = line.substr(closingBrace);

    size_t colon = line.find_last_of(':');
    if(colon == std::string::npos) {
        const char * message = "Section lines must have a colon (:) between the title and color code.";
        throw InvalidFileException(message);
    }

    std::string codeStr = line.substr(colon + 1);
    std::string trimStr = trimWhitespace(codeStr);
    int code;
    try {
        code = stoi(trimStr);
    } catch(std::invalid_argument& e) {
        const char * message = "Color code must be an integer.";
        throw InvalidFileException(message);
    } catch(std::out_of_range& e) {
        const char * message = "Color code cannot be an exceedingly large number.";
        throw InvalidFileException(message);
    }

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
