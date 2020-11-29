#include "InputForm.hpp"

InputForm::InputForm(const char * prompt) : prompt(prompt) {
    int promptLength = strlen(prompt);
    fields[0] = new_field(1, COLS - (promptLength + 2), LINES - 1, promptLength + 1, 0, 0);
    fields[1] = NULL;
    field_opts_off(fields[0], O_STATIC);

    form = new_form(fields);
    post_form(form);
    Point promptPoint(0, LINES - 1);
    std::string promptStr = prompt;
    drawStringAtPoint(promptStr, promptPoint);
    form_driver(form, REQ_BEG_FIELD);
}

InputForm::~InputForm() {
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
}

void InputForm::handleInput(int ch) {
    switch(ch) {
        case 127: // Backspace Key
            {
                form_driver(form, REQ_DEL_PREV);
                form_driver(form, REQ_SCR_BCHAR);
                form_driver(form, REQ_END_FIELD);
            }
            break;
        default: // Normal character
            {
                form_driver(form, ch);
            }
            break;
    }
}

char * InputForm::trimWhitespace(char * str) {
    char * end;

    // Leading space
    while(isspace(*str)) {
        str++;
    }

    if(str == 0) {
        // String is all spaces
        return str;
    }

    // Trailing space
    end = str + strnlen(str, 128) - 1;
    while(end > str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char * InputForm::getInputFromBuffer() {
    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_PREV_FIELD);
    char * input = trimWhitespace(field_buffer(fields[0], 0));
    form_driver(form, REQ_CLR_FIELD);

    return input;
}

void InputForm::returnFocus() {
    form_driver(form, REQ_BEG_FIELD);
}
