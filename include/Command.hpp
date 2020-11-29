#pragma once

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
