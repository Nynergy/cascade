#include "State.hpp"

State::State() : exitFlag(false) {}

State::~State() {
	for(SectionPanel * panel : panels) {
		delete panel;
	}
}

void State::addPanel(SectionPanel * panel) {
	panels.push_back(panel);
}

std::vector<SectionPanel *> State::getPanels() {
	return panels;
}

void State::setCurrentPanel(int panelIndex) {
	panelIndex = wrapIndex(panelIndex);
	currentPanel = panelIndex;
}

int State::wrapIndex(int index) {
	if(index > (int)panels.size() - 1) {
		return 0;
	}

	if(index < 0) {
		return panels.size() - 1;
	}

	return index;
}

SectionPanel * State::getCurrentPanel() {
	return panels[currentPanel];
}

int State::getCurrentPanelIndex() {
	return currentPanel;
}

bool State::panelIsFocused(SectionPanel * panel) {
	return panel == panels[currentPanel];
}

std::vector<Section> State::getSections() {
    std::vector<Section> sections;
    for(SectionPanel * panel : panels) {
        sections.push_back(panel->getSection());
    }

    return sections;
}

void State::setExitFlag(bool flag) {
	exitFlag = flag;
}

bool State::userHasQuit() {
	return exitFlag;
}

bool State::userHasNotQuit() {
	return !userHasQuit();
}