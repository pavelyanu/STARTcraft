#include "Planner.h"

#include <BWAPI.h>
#include "Utils.h"

#include <map>
#include <vector>
#include <tuple>

Planner::Planner() {
	Commander commander;
	lastOrder = 0;
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 8);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 11);
	AddToOrder(BWAPI::UnitTypes::Protoss_Cybernetics_Core, 13);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 15);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 18, 5);
	AddToOrder(BWAPI::UpgradeTypes::Protoss_Ground_Weapons, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 21);
	AddToOrder(BWAPI::UnitTypes::Protoss_Robotics_Facility, 26);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 29);
	AddToOrder(BWAPI::UnitTypes::Protoss_Observatory, 34);
	AddToOrder(BWAPI::UnitTypes::Protoss_Observer, 35, 5);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 37);
}

void Planner::AddToOrder(BWAPI::UnitType type, int order)
{
	buildOrder[order] = type;
}

void Planner::AddToOrder(BWAPI::UnitType type, int order, int amount)
{
	trainOrder[order] = std::tuple<BWAPI::UnitType, int>(type, amount);
}

void Planner::AddToOrder(BWAPI::UpgradeType type, int order)
{
	upgradeOrder[order] = type;
}

void Planner::AddUnit(BWAPI::UnitType type)
{
	const BWAPI::UnitType builderType = type.whatBuilds().first;
	if (!producers.count(builderType))
	{
		producers[builderType] = std::vector<BWAPI::UnitType>();
	}
	producers[builderType].push_back(type);
}

void Planner::AddUpgrade(BWAPI::UpgradeType type)
{
	const BWAPI::UnitType builderType = type.whatUpgrades();
	if (!upgraders.count(builderType))
	{
		upgraders[builderType] = std::vector<BWAPI::UpgradeType>();
	}
	upgraders[builderType].push_back(type);
}

void Planner::AddFromOrder()
{
	const int supply = BWAPI::Broodwar->self()->supplyUsed() / 2;
	std::vector<int> buildOrders(GetKeysInRange(&buildOrder, lastOrder, supply));
	for (auto& i : buildOrders)
	{
		if (buildOrder.count(i))
		{
			AddUnit(buildOrder[i]);
			buildOrder.erase(i);
		}
	}
	std::vector<int> upgradeOrders(GetKeysInRange(&upgradeOrder, lastOrder, supply));
	for (auto& i : upgradeOrders)
	{
		if (upgradeOrder.count(i))
		{
			AddUpgrade(upgradeOrder[i]);
			upgradeOrder.erase(i);
		}
	}
	std::vector<int> trainOrders(GetKeysInRange(&trainOrder, lastOrder, supply));
	for (auto& i : trainOrders)
	{
		if (trainOrder.count(i))
		{
			BWAPI::UnitType type;
			int amount;
			std::tie(type, amount) = trainOrder[i];
			for (int i = 0; i < amount; i++)
			{
				AddUnit(type);
			}
			trainOrder.erase(i);
		}
	}
	lastOrder = supply;
}

void Planner::ExecuteOrders()
{
	for (auto& [type, vector] : producers)
	{
		std::vector<BWAPI::UnitType>::iterator mit;
		for (mit = vector.begin(); mit != vector.end();)
		{
			if (mit->isBuilding())
			{
				if (Utils::Build(*mit, &commander))
				{
					mit = vector.erase(mit);
					continue;
				}
			}
			else
			{
				if (Utils::Train(*mit))
				{
					mit = vector.erase(mit);
					continue;
				}
			}
			mit++;
		}
	}
}

void Planner::OnStart()
{
	BWAPI::Unitset units = BWAPI::Broodwar->self()->getUnits();
	for (auto& unit : units)
	{
		if (unit->getType().isWorker())
		{
			BWAPI::Unit closestMineral = Utils::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());
			commander.PushCommand(unit, BWAPI::UnitCommand::rightClick(unit, closestMineral));
		}
	}
}

void Planner::OnFrame()
{
	AddFromOrder();
	ExecuteOrders();
	commander.OnFrame();
}

void Planner::OnUnitComplete(BWAPI::Unit unit)
{
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
	{
		int probe_count = 0;
		for (auto& u : BWAPI::Broodwar->self()->getUnits())
		{
			if (u->getType() != BWAPI::UnitTypes::Protoss_Probe) { continue; }
			if (!u->isCompleted()) { continue; }
			Utils::RightClick(u, unit);
			commander.PushCommand(u, BWAPI::UnitCommand::rightClick(u, unit));
			probe_count++;
			if (probe_count >= 3)
			{
				break;
			}
		}
	}
	else if (unit->getType().isWorker())
	{
		BWAPI::Unit closestMineral = Utils::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());
		commander.PushCommand(unit, BWAPI::UnitCommand::rightClick(unit, closestMineral));
	}
}

template<typename T>
std::vector<int> Planner::GetKeysInRange(const std::map<int, T>* map, int lower, int upper)
{
	auto it = map->begin();
	std::vector<int> result;
	while (it != map->end()) {
		const int i = it->first;
		if (i >= lower && i <= upper)
		{
			result.push_back(i);
		}
		it++;
	}
	return result;
}
