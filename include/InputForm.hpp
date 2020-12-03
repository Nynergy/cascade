#pragma once

#include <cstring>

#include "vexes.hpp"

class InputForm {

private:
    const char * prompt;
    int promptLength;
    std::string buffer;
    WINDOW * win;
    int lines, columns;

    void setupWin();
    void clearForm();
    void clearBehindForm();
    void drawPrompt();
    void drawBuffer();
    void addCharToBuffer(int ch);
    void removeCharFromBuffer();
    std::string trimWhitespace(std::string str);

public:
    InputForm(const char * prompt);
    ~InputForm();
    void drawForm();
    WINDOW * getWin();
    void injectString(std::string str);
    void handleInput(int ch);
    std::string getInputFromBuffer();

};
