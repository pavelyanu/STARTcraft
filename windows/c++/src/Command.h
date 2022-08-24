#pragma once
#include <BWAPI.h>
#include <map>
#include <vector>

class Command
{
public:
	Command();
	virtual ~Command();
	virtual int GetPriority() = 0;
	virtual bool Execute() = 0;
	virtual bool Possible() = 0;
	virtual bool IsDone() = 0;
	BWAPI::Unit executor;
};

class Move : public Command
{
public:
	Move(BWAPI::Unit _executor, BWAPI::Position _position);
	virtual ~Move();
	inline int GetPriority() noexcept override { return 0; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	BWAPI::Position position;
};

class Attack : public Command
{
public:
	Attack(BWAPI::Unit _executor, BWAPI::Unit _target);
	virtual ~Attack();
	inline int GetPriority() noexcept override { return 10; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	BWAPI::Unit target;
};

class Hold : public Command
{
public:
	Hold(BWAPI::Unit _executor);
	virtual ~Hold();
	inline int GetPriority() noexcept override { return 20; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
};

class Patrol : public Command
{
public:
	Patrol(BWAPI::Unit _executor, BWAPI::Position _position);
	virtual ~Patrol();
	inline int GetPriority() noexcept override { return 30; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	BWAPI::Position position;
};

class Scout : public Command
{
public:
	Scout(BWAPI::Unit _executor, std::vector<BWAPI::Position> _positions);
	virtual ~Scout();
	inline int GetPriority() noexcept override { return 40; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	std::vector<BWAPI::Position> positions;
	BWAPI::Position currentGoal;
};

class Build : public Command
{
public:
	Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType);
	Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType, BWAPI::Position _position);
	Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType, BWAPI::TilePosition _position);
	virtual ~Build();
	inline int GetPriority() noexcept override { return 50; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	BWAPI::UnitType targetType;
	BWAPI::TilePosition position;
};

class Gather : public Command
{
public:
	Gather(BWAPI::Unit _executor, BWAPI::Unit _target);
	Gather(BWAPI::Unit _executor);
	virtual ~Gather();
	inline int GetPriority() noexcept override { return 60; }
	bool Execute() override;
	bool Possible() override;
	bool IsDone() override;
private:
	BWAPI::Unit target;
};
