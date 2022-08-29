#include "Utils.h"

#include "Commander.h"

#include <iostream>

void Utils::RightClick(BWAPI::Unit unit, BWAPI::Unit target)
{
	if (!unit || !target) { return; }
	const int lastCommandTime = unit->getLastCommandFrame();
	const int frameCount = BWAPI::Broodwar->getFrameCount();
	const BWAPI::Unit lastTarget = unit->getLastCommand().getTarget();
	if (lastCommandTime >= frameCount || target == lastTarget) { return; }
	unit->rightClick(target);
}

void Utils::RightClick(BWAPI::Unit unit, BWAPI::Position target)
{
	if (!unit || !target) { return; }
	const int lastCommandTime = unit->getLastCommandFrame();
	const int frameCount = BWAPI::Broodwar->getFrameCount();
	const BWAPI::Position lastTarget = unit->getLastCommand().getTargetPosition();
	if (lastCommandTime >= frameCount || target == lastTarget) { return; }
	unit->rightClick(target);
}

bool Utils::IssueCommand(BWAPI::Unit unit, BWAPI::UnitCommand command)
{
	if (!unit) { return false; }
	const int lastCommandTime = unit->getLastCommandFrame();
	const int frameCount = BWAPI::Broodwar->getFrameCount();
	const BWAPI::UnitCommand lastCommand = unit->getLastCommand();
	if (lastCommandTime >= frameCount || command == lastCommand) { return true; }
	return unit->issueCommand(command);
}

BWAPI::Unit Utils::GetUnitOfType(BWAPI::UnitType type)
{
	for (auto& unit : BWAPI::Broodwar->self()->getUnits())
    {
        if (unit->getType() == type && unit->isCompleted())
        {
            return unit;
        }
    }
    return nullptr;
}

BWAPI::Unit Utils::GetClosestUnitTo(BWAPI::Position p, const BWAPI::Unitset& units)
{
    BWAPI::Unit closestUnit = nullptr;
    for (auto& u : units)
    {
        if (!closestUnit || u->getDistance(p) < closestUnit->getDistance(p))
        {
            closestUnit = u;
        }
    }
    return closestUnit;
}



BWAPI::Unit Utils::GetClosestUnitTo(const BWAPI::Unit unit, const BWAPI::Unitset& units)
{
    if (!unit) { return nullptr; }
    return GetClosestUnitTo(unit->getPosition(), units);
}

int Utils::GetSupplyTotoal()
{
	int supply = BWAPI::Broodwar->self()->supplyTotal();
	for (auto& unit : BWAPI::Broodwar->self()->getUnits())
	{
		const BWAPI::UnitCommand& command = unit->getLastCommand();
		if (command.getType() == BWAPI::UnitCommandTypes::Build) {
			supply += command.getUnitType().supplyProvided();
		}
		if (unit->isCompleted()) { continue; }
		supply += unit->getType().supplyProvided();
	}
	return supply / 2;
}

bool Utils::Build(BWAPI::UnitType type, BWAPI::Position p)
{
	if (!IsProducable(type)) { return false; }
	const BWAPI::UnitType builderType = type.whatBuilds().first;
	const BWAPI::TilePosition desiredPosition = BWAPI::TilePosition(p);
	BWAPI::Unit builder =
		BWAPI::Broodwar->
			getClosestUnit(p, (BWAPI::Filter::GetType == builderType) && BWAPI::Filter::IsAlly);
	if (!builder) { return false; }
	const BWAPI::TilePosition buildPosition =
		BWAPI::Broodwar->getBuildLocation(type, desiredPosition, Utils::MAXMAPSIZE, builder);
	return builder->build(type, buildPosition);
}

bool Utils::Build(BWAPI::UnitType type)
{
	if (!IsProducable(type)) { return false; }
	const BWAPI::Position p = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	return Build(type, p);
}

bool Utils::Build(const BWAPI::UnitType type, Commander* commander)
{
	const BWAPI::UnitType builderType = type.whatBuilds().first;
	const BWAPI::Position p = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	const BWAPI::TilePosition desiredPosition = BWAPI::TilePosition(p);
	BWAPI::Unit builder =
		BWAPI::Broodwar->
		getClosestUnit(p, (BWAPI::Filter::GetType == builderType) && BWAPI::Filter::IsAlly);
	if (!builder) { return false; }
	const BWAPI::TilePosition buildPosition =
		BWAPI::Broodwar->getBuildLocation(type, desiredPosition, Utils::MAXMAPSIZE, builder);
	commander->PushCommand(builder, BWAPI::UnitCommand::build(builder, buildPosition, type));
	return true;
}


bool Utils::Train(BWAPI::UnitType type)
{
	if (!IsProducable(type)) { return false; }
	const BWAPI::UnitType builderType = type.whatBuilds().first;
	BWAPI::Unitset units = BWAPI::Broodwar->self()->getUnits();
	for (auto& unit : units)
	{
		if (unit->getType() == builderType && !unit->isTraining()) {
			return unit->train(type);
		}
	}
	return false;
}

bool Utils::Upgrade(BWAPI::UpgradeType type)
{
	const BWAPI::UnitType upgraderType = type.whatUpgrades();
	BWAPI::Unitset units = BWAPI::Broodwar->self()->getUnits();
	for (auto& unit : units)
	{
		if (unit->getType() == upgraderType && !unit->isUpgrading()) {
			return unit->upgrade(type);
		}
	}
	return false;

}

bool Utils::IsProducable(BWAPI::UnitType type)
{
	const int minerals = BWAPI::Broodwar->self()->minerals();
	const int gas = BWAPI::Broodwar->self()->gas();
	const int requiredMinerals = type.mineralPrice();
	const int requiredGas = type.gasPrice();
	if (requiredGas > gas || requiredMinerals > minerals)
	{
		return false;
	}
	return true;
}

bool Utils::IsProducable(BWAPI::UpgradeType type)
{
	const int minerals = BWAPI::Broodwar->self()->minerals();
	const int gas = BWAPI::Broodwar->self()->gas();
	const int requiredMinerals = type.mineralPrice();
	const int requiredGas = type.gasPrice();
	if (requiredGas > gas || requiredMinerals > minerals)
	{
		return false;
	}
	return true;
}

int Utils::CountUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units)
{
    int sum = 0;
    for (auto& unit : units)
    {
        if (unit->getType() == type)
        {
            sum++;
        }
    }
    return sum;
}

void Utils::TrainWorkers(int max)
{
	BWAPI::Player self = BWAPI::Broodwar->self();
	BWAPI::UnitType workerType = self->getRace().getWorker();
	int currently = CountUnitsOfType(workerType, self->getUnits());
	if (currently < max)
	{
		Train(workerType);
	}
}

void Utils::DrawUnitCommands()
{
    for (auto& unit : BWAPI::Broodwar->self()->getUnits())
    {
        const BWAPI::UnitCommand & command = unit->getLastCommand();
        if (command.getTargetPosition() != BWAPI::Positions::None)
        {
            BWAPI::Broodwar->drawLineMap(unit->getPosition(), command.getTargetPosition(), BWAPI::Colors::Red);
        }
        if (command.getTargetTilePosition() != BWAPI::TilePositions::None)
        {
            BWAPI::Broodwar->drawLineMap(unit->getPosition(), BWAPI::Position(command.getTargetTilePosition()), BWAPI::Colors::Green);
        }
        if (command.getTarget() != nullptr)
        {
            BWAPI::Broodwar->drawLineMap(unit->getPosition(), command.getTarget()->getPosition(), BWAPI::Colors::White);
        }
    }
}

void Utils::DrawUnitIDs()
{
    for (auto& unit : BWAPI::Broodwar->self()->getUnits())
    {
		BWAPI::Broodwar->drawTextMap(unit->getPosition(), std::to_string(unit->getID()).c_str(), BWAPI::Colors::White);
    }
}
