#pragma once

#include <form.h>
#include <cstring>
#include <cctype>

#include "vexes.hpp"

class InputForm {

private:
    FIELD * fields[2];
    FORM * form;
    const char * prompt;
    WINDOW * win;
    int lines, columns;
    std::string buffer;

    void clearForm();
    void clearBehindForm();
    void drawPrompt();
    void drawBuffer();
    void addCharToBuffer(int ch);
    void removeCharFromBuffer();
    char * trimWhitespace(char * str);

public:
    InputForm(const char * prompt);
    ~InputForm();
    void drawForm();
    WINDOW * getWin();
    void handleInput(int ch);
    char * getInputFromBuffer();
    void returnFocus();

};
