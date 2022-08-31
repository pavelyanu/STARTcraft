#pragma once
#include <BWAPI.h>
#include <bwem.h>

#include <vector>
#include <deque>
#include <memory>
#include <iostream>

#include "Command.h"
#include "MapTools.h"

using namespace Commands;

class Role
{
public:
	Role();
	virtual ~Role();
	virtual void OnFrame() = 0;
	virtual std::string name() = 0;
	void PushCommand(std::unique_ptr<Command> _command);
	void PopCommand();
	Command* GetCurrentCommand();
	void ClearCommands();
	void PopNext();
	inline int CommandsLength() { return commands.size(); }
	BWAPI::Unit self;
	std::vector<std::unique_ptr<Command>> commands;
	std::unique_ptr<Command> command;
};

inline void Role::PushCommand(std::unique_ptr<Command> _command)
{
	commands.push_back(std::move(command));
	command = std::move(_command);
	//std::cout << "Pushed " << command.get()->name() << " to " << name() << self->getID() << '\n';
}

class Worker : public Role
{
public:
	Worker(BWAPI::Unit _self);
	Worker(BWAPI::Unit _self, BWAPI::Unit target);
	void OnFrame();
	virtual ~Worker();
	inline std::string name() { return "Worker"; }
};

class Guard : public Role
{
public:
	Guard(BWAPI::Unit _self);
	Guard(BWAPI::Unit _self, BWAPI::Position);
	void OnFrame();
	virtual ~Guard();
	bool attacking;
	inline std::string name() { return "Guard"; }
};

class Observer : public Role
{
public:
	Observer(BWAPI::Unit _self, MapTools* maptools);
	void OnFrame();
	virtual ~Observer();
	inline std::string name() {return "Observer";}
	void MoveToNextDestination();
	std::vector<BWAPI::Position> unexplored;
	BWAPI::Position destination;
	MapTools* maptools;
	int frameCount;
};

class Spy : public Role
{
public:
	Spy(BWAPI::Unit _self, MapTools* maptools);
	void OnFrame();
	void MoveToNextDestination();
	virtual ~Spy();
	inline std::string name() { return "Spy"; }
	std::deque<BWAPI::TilePosition> startPositions;
	BWAPI::TilePosition destination;
	bool foundEnemyBase;
	BWAPI::TilePosition enemyBase;
	MapTools* maptools;
};

class Attacker : public Role
{
public:
	Attacker(BWAPI::Unit _self, MapTools* destination);
	void OnFrame();
	virtual ~Attacker();
	inline std::string name() { return "Attacker"; }
	MapTools* maptools;
	bool attacking;
}; 