#include "InputForm.hpp"

InputForm::InputForm(const char * prompt) : prompt(prompt) {
    int promptLength = strlen(prompt);
    fields[0] = new_field(1, COLS - (promptLength + 1), LINES - 1, promptLength + 1, 0, 0);
    fields[1] = NULL;
    field_opts_off(fields[0], O_STATIC);

    // Setup form with it's own window
    form = new_form(fields);
    scale_form(form, &lines, &columns);

    win = newwin(lines, columns + (promptLength + 1), LINES - 1, 0);
    keypad(win, TRUE);
    set_form_win(form, win);
    set_form_sub(form, derwin(win, lines, columns, 0, 0));

    post_form(form);
    form_driver(form, REQ_BEG_FIELD);

    buffer = "";
}

InputForm::~InputForm() {
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    delwin(win);
}

void InputForm::drawForm() {
    clearForm();
    drawPrompt();
    drawBuffer();
}

void InputForm::clearForm() {
    int promptLength = strlen(prompt);
    Point ul(promptLength + 1, 0); Point lr(COLS - 1, 0);
    Box formBox(ul, lr);
    clearBox(formBox, win);
}

void InputForm::clearBehindForm() {
    Point ul(0, LINES - 1); Point lr(COLS - 1, LINES - 1);
    Box behindFormBox(ul, lr);
    clearBox(behindFormBox);
}

void InputForm::drawPrompt() {
    Point promptPoint(0, 0);
    std::string promptStr = prompt;
    drawStringAtPoint(promptStr, promptPoint, win);
}

void InputForm::drawBuffer() {
    int promptLength = strlen(prompt);
    int bufferSize = (int)buffer.size();
    std::string tempBuffer;
    int formLength = columns - (promptLength + 2);
    if(bufferSize >= formLength) {
        tempBuffer = buffer.substr(bufferSize - formLength, bufferSize);
    } else {
        tempBuffer = buffer;
    }

    Point bufferPoint(promptLength + 1, 0);
    drawStringAtPoint(tempBuffer, bufferPoint, win);
}

WINDOW * InputForm::getWin() {
    return win;
}

void InputForm::handleInput(int ch) {
    switch(ch) {
        case 127: // Backspace Key
            {
                form_driver(form, REQ_DEL_PREV);
                form_driver(form, REQ_SCR_BCHAR);
                form_driver(form, REQ_NEXT_WORD);
                removeCharFromBuffer();
            }
            break;
        default: // Normal character
            {
                form_driver(form, ch);
                addCharToBuffer(ch);
            }
            break;
    }
}

void InputForm::removeCharFromBuffer() {
    buffer = buffer.substr(0, buffer.size() - 1);
}

void InputForm::addCharToBuffer(int ch) {
    char converted[2];
    sprintf(converted, "%c", ch);
    std::string str = converted;
    buffer = buffer + str;
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

    clearBehindForm();

    return input;
}

void InputForm::returnFocus() {
    form_driver(form, REQ_BEG_FIELD);
}
