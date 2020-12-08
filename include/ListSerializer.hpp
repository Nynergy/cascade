#pragma once

#include "State.hpp"

class ListSerializer {

private:
    static bool isRelativePath(std::string path) {
        std::string firstChar = path.substr(0, 1);

        return firstChar == "~";
    }

    static std::string convertToAbsolutePath(std::string path) {
        if(isRelativePath(path)) {
            std::string userHome = getenv("HOME");
            std::string remaining = path.substr(1);
            std::string absPath = userHome + remaining;

            return absPath;
        } else {
            return path;
        }
    }

public:
    static void serializeListToFile(State * state) {
        std::string listPath = convertToAbsolutePath(state->getListPath());
        std::ofstream outfile(listPath);

        std::vector<Section> sections = state->getSections();

        for(Section section : sections) {
            outfile << "[" << section.title << "] : " << section.colorCode << std::endl;
            for(std::string item : section.items) {
                outfile << item << std::endl;
            }
            outfile << std::endl;
        }

        outfile.close();
    }

};
