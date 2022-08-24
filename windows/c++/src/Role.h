#pragma once
#include <BWAPI.h>
#include <bwem.h>

#include <vector>
#include <memory>

#include "Command.h"

class Role
{
public:
	Role();
	virtual ~Role();
	virtual void OnFrame() = 0;
	template<typename T>
	void PushCommand(T&& _command);
	void PopCommand();
	const Command* GetCurrentCommand();
	void ClearCommands();
	void PushNext();
	BWAPI::Unit self;
	std::vector<std::unique_ptr<Command>> commands;
	std::unique_ptr<Command> command;
};

template<typename T>
inline void Role::PushCommand(T&& _command)
{
	commands.push_back(std::move(command));
	command = std::make_unique<T>(_command);
}

class Worker : public Role
{
	Worker(BWAPI::Unit _self);
	Worker(BWAPI::Unit _self, BWAPI::Unit target);
	void OnFrame();
	virtual ~Worker();
};

class Guard : public Role
{
	Guard(BWAPI::Unit _self);
	Guard(BWAPI::Unit _self, BWAPI::Position);
	void OnFrame();
	virtual ~Guard();
};

class Spy : public Role
{
	Spy(BWAPI::Unit _self);
	void OnFrame();
	virtual ~Spy();
};

class Observer : public Role
{
	Observer(BWAPI::Unit _self);
	void OnFrame();
	virtual ~Observer();
};

class Attacker : public Role
{
	Attacker(BWAPI::Unit _self);
	void OnFrame();
	virtual ~Attacker();
};
