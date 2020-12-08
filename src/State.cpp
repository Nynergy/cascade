#include "State.hpp"

State::State() : exitFlag(false), mode(Mode::NORMAL) {}

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

int State::getNumPanels() {
    return (int)panels.size();
}

void State::replacePanels(std::vector<SectionPanel *> newPanels) {
    for(SectionPanel * panel : panels) {
        delete panel;
    }

    panels = newPanels;
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

void State::removeCurrentPanel() {
    delete panels[currentPanel];
    panels.erase(panels.begin() + currentPanel);
    resetIndices();
}

void State::resetIndices() {
    currentPanel = 0;
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

Mode State::getMode() {
    return mode;
}

void State::setMode(Mode newMode) {
    mode = newMode;
}

void State::swapPanelDown() {
    if(currentPanel == (getNumPanels() - 1)) {
        return;
    }

    std::swap(panels[currentPanel], panels[currentPanel + 1]);
    currentPanel++;
}

void State::swapPanelUp() {
    if(currentPanel == 0) {
        return;
    }

    std::swap(panels[currentPanel - 1], panels[currentPanel]);
    currentPanel--;
}
