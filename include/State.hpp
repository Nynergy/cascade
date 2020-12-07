#pragma once

#include "SectionPanel.hpp"

enum Mode {
    NORMAL = 0,
    MOVE,
};

class State {

private:
	std::vector<SectionPanel *> panels;
	int currentPanel;
	bool exitFlag;
    Mode mode;

	int wrapIndex(int index);
    void resetIndices();

public:
	State();
	~State();
	void addPanel(SectionPanel * panel);
	std::vector<SectionPanel *> getPanels();
    void replacePanels(std::vector<SectionPanel *> newPanels);
	void setCurrentPanel(int panelIndex);
	SectionPanel * getCurrentPanel();
    void removeCurrentPanel();
	int getCurrentPanelIndex();
	bool panelIsFocused(SectionPanel * panel);
    std::vector<Section> getSections();
	void setExitFlag(bool flag);
	bool userHasQuit();
	bool userHasNotQuit();
    Mode getMode();
    void setMode(Mode newMode);

};
