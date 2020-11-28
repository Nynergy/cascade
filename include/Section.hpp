#pragma once

#include <vector>

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
