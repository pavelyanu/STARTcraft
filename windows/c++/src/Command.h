#pragma once
#include <BWAPI.h>
#include <map>
#include <vector>

class Command
{
public:
	Command();
	~Command();
	CommandType type;
	virtual int GetPriority() = 0;
	virtual void Execute() = 0;
	virtual bool Possible() = 0;
};

class Attack : public Command
{
public:
	Attack(BWAPI::Unit _executor, BWAPI::Unit _target);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
};

class Hold : public Command
{
public:
	Hold(BWAPI::Unit _executor, BWAPI::Position _position);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
};

class Patrol : public Command
{
public:
	Patrol(BWAPI::Unit _executor, std::vector<BWAPI::Position> _positions);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
private:
	std::vector<BWAPI::Position> positions;
};

class Scout : public Command
{
public:
	Scout(BWAPI::Unit _executor, std::vector<BWAPI::Position> _positions);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
private:
	std::vector<BWAPI::Position> positions;
};

class Build : public Command
{
public:
	Build(BWAPI::Unit _executor, BWAPI::UnitType);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
};

class Gather : public Command
{
public:
	Gather(BWAPI::Unit _executor, BWAPI::Position _position);
	int GetPriority() override;
	void Execute() override;
	bool Possible() override;
};

enum CommandType
{
	attack_move = 10,
	hold_position = 20,
	patrol = 30,
	scout = 40,
	build = 50,
	mine = 60,
};
