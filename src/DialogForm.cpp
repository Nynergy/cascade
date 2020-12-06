#include "DialogForm.hpp"

DialogForm::DialogForm(std::string prompt, State * state) :
    Form(Point(0, LINES - 1), prompt), state(state) {}

std::string DialogForm::edit() {
    // Make cursor visible while typing
    curs_set(1);

    int ch;
    bool exit = false;
    while(!exit) {
        drawForm();
        ch = wgetch(win);
        switch(ch) {
            case KEY_RESIZE: // Our custom form handles resizing
                resizeForm();
                resizePanels();
                break;
            case 10: // Enter Key (submit)
            case KEY_F(1): // Cancel form input
                exit = true;
                break;
            default: // Delegate to form driver
                handleInput(ch);
                break;
        }
    }

    // Make cursor invisible after typing
    curs_set(0);

    return trimWhitespace(buffer);
}

void DialogForm::resizeForm() {
    delwin(win);
    Point newOrigin(0, LINES - 1);
    origin = newOrigin;
    columns = COLS - (promptLength + 2);
    win = newwin(lines, COLS - 1, origin.y, origin.x);
}

void DialogForm::resizePanels() {
    std::vector<Section> sections = state->getSections();
    std::vector<SectionPanel *> newPanels = PanelConstructor::constructPanelsFromSections(sections);
    state->replacePanels(newPanels);

    for(SectionPanel * panel : newPanels) {
        if(state->panelIsFocused(panel)) {
            panel->drawPanelFocused();
        } else {
            panel->drawPanel();
        }
    }
}

bool DialogForm::dialog() {
    int ch;
    bool exit = false;
    bool choice = false;
    while(!exit) {
        drawDialog();
        ch = wgetch(win);
        switch(ch) {
            case KEY_RESIZE:
                resizeForm();
                resizePanels();
                break;
            case 'y':
            case 'Y':
                choice = true;
                exit = true;
                break;
            case 'n':
            case 'N':
                choice = false;
                exit = true;
                break;
            default:
                // Wait until user answers yes or no
                break;
        }
    }

    return choice;
}

void DialogForm::drawDialog() {
    drawStringAtPoint(prompt, origin, win);
}
