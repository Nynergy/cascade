#include <iostream>

#include "Config.hpp"
#include "ListParser.hpp"

std::string convertToAbsolutePath(std::string tildePath) {
    std::string tildeRemoved = tildePath.substr(1);
    std::string userHome = getenv("HOME");
    std::string absolutePath = userHome + tildeRemoved;

    return absolutePath;
}

int main() {

    std::string userHome = getenv("HOME");
    std::string cascadePath = "/.cascade/cascade.conf";
    std::string configPath = userHome + cascadePath;
    Config::getInstance().readFromConfigFile(configPath);

    std::string masterList = Config::getInstance().getValueFromKey("MasterList");
    std::cout << "Master List is at: " << masterList << std::endl << std::endl;

    std::string listPath = convertToAbsolutePath(masterList);

    ListParser * lp = new ListParser(listPath);
    std::vector<Section> sections = lp->parseList();

    for(Section sect : sections) {
        std::cout << "Section Title: " << sect.title << std::endl;
        std::cout << "Section Color Code: " << sect.colorCode << std::endl;
        for(std::string item : sect.items) {
            std::cout << "Item: " << item << std::endl;
        }

        std::cout << std::endl;
    }

    delete lp;

    return 0;

}
