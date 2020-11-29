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

        char * trimWhitespace(char * str);
    public:
        InputForm(const char * prompt);
        ~InputForm();
        void handleInput(int ch);
        char * getInputFromBuffer();
        void returnFocus();
};
