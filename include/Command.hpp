#pragma once

#include "PanelConstructor.hpp"
#include "State.hpp"

class Command {
protected:
	State * state;

	Command(State * state);
public:
	virtual ~Command() {}
	virtual void execute() = 0;
};

class NOPCommand : public Command {
public:
	NOPCommand(State * state);
	void execute() override;
};

class ResizeWindowCommand : public Command {
public:
    ResizeWindowCommand(State * state);
    void execute() override;
};

class QuitApplicationCommand : public Command {
public:
	QuitApplicationCommand(State * state);
	void execute() override;
};

class FocusPanelDownCommand : public Command {
private:
	void incrementCurrentPanel();

public:
	FocusPanelDownCommand(State * state);
	void execute() override;
};

class FocusPanelUpCommand : public Command {
private:
	void decrementCurrentPanel();

public:
	FocusPanelUpCommand(State * state);
	void execute() override;
};

class ScrollDownCommand : public Command {
public:
	ScrollDownCommand(State * state);
	void execute() override;
};

class ScrollUpCommand : public Command {
public:
	ScrollUpCommand(State * state);
	void execute() override;
};

class CycleColorCommand : public Command {
public:
    CycleColorCommand(State * state);
    void execute() override;
};

class EditItemCommand : public Command {
private:
    Form * form;

    void setupEditBuffer();
    std::string getItemName();
    std::string getUserInput();
    void changeItemName(std::string input);
    void teardownEditBuffer();
    void clearBehindForm();
public:
    EditItemCommand(State * state);
    void execute() override;
};

class EditSectionCommand : public Command {
private:
    Form * form;

    void setupEditBuffer();
    std::string getSectionTitle();
    std::string getUserInput();
    void changeSectionName(std::string input);
    void teardownEditBuffer();
    void clearBehindForm();
public:
    EditSectionCommand(State * state);
    void execute() override;
};

class NewItemCommand : public Command {
private:
    Form * form;

    void setupEditBuffer();
    std::string getUserInput();
    void addItemToSection(std::string newItem);
    void teardownEditBuffer();
    void clearBehindForm();
public:
    NewItemCommand(State * state);
    void execute() override;
};

class NewSectionCommand : public Command {
private:
    Form * form;

    void setupEditBuffer();
    std::string getUserInput();
    void createNewSectionWithName(std::string name);
    void teardownEditBuffer();
    void clearBehindForm();
public:
    NewSectionCommand(State * state);
    void execute() override;
};
