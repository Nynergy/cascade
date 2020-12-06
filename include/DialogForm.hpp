#pragma once

#include "PanelConstructor.hpp"
#include "State.hpp"

class DialogForm : public Form {

private:
    State * state;

    void resizeForm();
    void resizePanels();
    void drawDialog();

public:
    DialogForm(std::string prompt, State * state);

    std::string edit() override;
    bool dialog();

};
