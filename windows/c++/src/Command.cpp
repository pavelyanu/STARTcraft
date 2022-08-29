#include "Command.h"

#include <BWAPI.h>
#include "Utils.h"

using namespace Commands;

/*
--------------------Command--------------------
*/

Command::Command() {}
Command::~Command() {}

/*
--------------------Attack--------------------
*/

Attack::Attack(BWAPI::Unit _executor, BWAPI::Unit _target)
{
	executor = _executor;
	target = _target;
}

Attack::~Attack()
{
}

bool Attack::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::attack(executor, target);
	return Utils::IssueCommand(executor, command);
}

bool Attack::Possible()
{
	if (!executor->exists()) { return false; }
	if (!target->exists()) { return false; }
	if (!target->isVisible()) { return false; }
	if (!executor->canAttackUnit()) { return false; }
	return true;
}

bool Attack::IsDone()
{
	if (!Possible()) { return true; }
	if (!target->exists()) { return true; }
	return false;
}

/*
--------------------Hold--------------------
*/

Hold::Hold(BWAPI::Unit _executor)
{
	executor = _executor;
}

Hold::~Hold()
{
}

bool Hold::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::holdPosition(executor);
	return Utils::IssueCommand(executor, command);
}

bool Hold::Possible()
{
	if (!executor->canHoldPosition()) { return false; }
	if (!executor->exists()) { return false; }
	return true;
}

bool Hold::IsDone()
{
	return false;
}

/*
--------------------Patrol--------------------
*/

Patrol::Patrol(BWAPI::Unit _executor, BWAPI::Position _position)
{
	executor = _executor;
	position = _position.makeValid();
}

Patrol::~Patrol()
{
}

bool Patrol::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::patrol(executor, position);
	return Utils::IssueCommand(executor, command);
}

bool Patrol::Possible()
{
	if (!executor->exists()) { return false; }
	if (!executor->canMove()) { return false; }
	return true;
}

bool Patrol::IsDone()
{
	return false;
}

/*
--------------------Scout--------------------
*/

Scout::Scout(BWAPI::Unit _executor, std::vector<BWAPI::Position> _positions)
{
	executor = _executor;
	positions = _positions;
}

Scout::~Scout()
{
}

bool Scout::Execute()
{
	return false;
}

bool Scout::Possible()
{
	return false;
}

bool Scout::IsDone()
{
	return false;
}

/*
--------------------Build--------------------
*/

Build::Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType)
{
	executor = _executor;
	targetType = _targetType;
	position = BWAPI::Broodwar->getBuildLocation(targetType, BWAPI::Broodwar->self()->getStartLocation());
}

Build::Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType, BWAPI::Position _position)
{
	executor = _executor;
	targetType = _targetType;
	position = BWAPI::TilePosition(_position);
}

Build::Build(BWAPI::Unit _executor, BWAPI::UnitType _targetType, BWAPI::TilePosition _position)
{
	executor = _executor;
	targetType = _targetType;
	position = _position;
}

Build::~Build()
{
}

bool Build::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::build(executor, position, targetType);
	return Utils::IssueCommand(executor, command);
}

bool Build::Possible()
{
	if (!executor->canBuild(targetType)) { return false; }
	if (!BWAPI::Broodwar->canBuildHere(position, targetType, executor)) { return false; }
	return true;
}

bool Build::IsDone()
{
	for (auto& u : BWAPI::Broodwar->getUnitsOnTile(position))
	{
		if (u->getType() == targetType) { return true; }
		if (u->getType().isBuilding()) {
			position = BWAPI::Broodwar->getBuildLocation(targetType, position);
		}
	}
	return false;
}

/*
--------------------Gather--------------------
*/

Gather::Gather(BWAPI::Unit _executor, BWAPI::Unit _target)
{
	executor = _executor;
	target = _target;
}

Gather::Gather(BWAPI::Unit _executor)
{
	executor = _executor;
	target = Utils::GetClosestUnitTo(executor, BWAPI::Broodwar->getMinerals());
}

Gather::~Gather()
{
}

bool Gather::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::gather(executor, target);
	return Utils::IssueCommand(executor, command);
}

bool Gather::Possible()
{
	if (!executor->exists()) { return false; }
	if (!executor->canGather()) { return false; }
	else if (!target->exists()) { return false; }
	return true;
}

bool Gather::IsDone()
{
	return false;
}

/*
--------------------Move--------------------
*/

Move::Move(BWAPI::Unit _executor, BWAPI::Position _position)
{
	executor = _executor;
	position = _position.makeValid();
}

Move::~Move()
{
}

bool Move::Execute()
{
	const BWAPI::UnitCommand command = BWAPI::UnitCommand::move(executor, position);
	return Utils::IssueCommand(executor, command);
}

bool Move::Possible()
{
	if (!executor->exists()) { return false; }
	if (!executor->canMove()) { return false; }
	if (!BWAPI::Broodwar->hasPath(executor->getPosition(), position)) { return false; }
	return true;
}

bool Move::IsDone()
{
	if (position.getDistance(executor->getPosition()) < 10) { return true; }
	return false;
}
