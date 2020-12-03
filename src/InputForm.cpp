#include "InputForm.hpp"

InputForm::InputForm(const char * prompt) : prompt(prompt), promptLength(strlen(prompt)), buffer("") {
    setupWin();
}

InputForm::~InputForm() {
    delwin(win);
}

void InputForm::setupWin() {
    lines = 1; columns = COLS - (promptLength + 1);

    win = newwin(lines, columns + (promptLength + 1), LINES - 1, 0);
    keypad(win, TRUE);
}

void InputForm::drawForm() {
    clearForm();
    drawPrompt();
    drawBuffer();
}

void InputForm::clearForm() {
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

void InputForm::injectString(std::string str) {
    for(char ch : str) {
        addCharToBuffer(ch);
    }
}

void InputForm::handleInput(int ch) {
    switch(ch) {
        case 127: // Backspace Key
            {
                removeCharFromBuffer();
            }
            break;
        default: // Normal character
            {
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

std::string InputForm::trimWhitespace(std::string str) {
    size_t first = str.find_first_not_of(' ');
	if(std::string::npos == first) {
		return str;
	}

	size_t last = str.find_last_not_of(' ');

	return str.substr(first, (last - first + 1));
}

std::string InputForm::getInputFromBuffer() {
    std::string input = trimWhitespace(buffer);

    clearBehindForm();

    return input;
}
