#include "Planner.h"

#include <BWAPI.h>
#include <bwem.h>

#include "Utils.h"

#include <map>
#include <vector>
#include <tuple>
#include <iostream>

Planner::Planner() {
	Commander commander;
	lastOrder = 0;
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 5);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 11, 5);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 12);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 13);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Cybernetics_Core, 14);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 15);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 18, 5);
	//AddToOrder(BWAPI::UpgradeTypes::Protoss_Ground_Weapons, 20);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 21);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Robotics_Facility, 26);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 29);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Observatory, 34);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Observer, 35, 5);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 37);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 37);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Observatory, 34);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 38, 50);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 38, 30);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 45);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 50);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 55);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 60);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 65);


	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 5);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 15);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 25);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 30);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 35);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 40);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 45);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 50);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 55);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 60);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 65);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 70);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 75);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 80);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 85);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 90);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 95);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 100);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 105);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 110);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 115);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 120);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 125);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 130);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 135);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 140);
	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 145);

	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 11);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 12, 20);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 13);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 14);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 16);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Cybernetics_Core, 17);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Forge, 18);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 19);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 21);
	//AddToOrder(BWAPI::UpgradeTypes::Protoss_Ground_Weapons, 24);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Robotics_Facility, 26);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Observatory, 34);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Observer, 35, 5);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 48, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 49, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 51, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 52, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 53, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 54, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 71, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 72, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 73, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 74, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 91, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 92, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 93, 10);
	//AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 94, 10);

	AddToOrder(BWAPI::UnitTypes::Protoss_Pylon, 5);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 9);
	AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 11);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 12);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 13);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 14);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 16);
	AddToOrder(BWAPI::UnitTypes::Protoss_Gateway, 17);
	AddToOrder(BWAPI::UnitTypes::Protoss_Cybernetics_Core, 18);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 19, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 21, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 22, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 23, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 24, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 26, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 27, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 28, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 29, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 31, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 32, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 33, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 34, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 36, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 37, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Zealot, 38, 20);
	AddToOrder(BWAPI::UnitTypes::Protoss_Dragoon, 39, 10);
	AddToOrder(BWAPI::UnitTypes::Protoss_Assimilator, 41);



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
	//std::cout << "Pushed " << type.c_str() << " to producers\n";
}

void Planner::AddUpgrade(BWAPI::UpgradeType type)
{
	const BWAPI::UnitType builderType = type.whatUpgrades();
	if (!upgraders.count(builderType))
	{
		upgraders[builderType] = std::vector<BWAPI::UpgradeType>();
	}
	upgraders[builderType].push_back(type);
	//std::cout << "Pushed " << type.c_str() << " to upgrades\n";
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
				if (manager->Build(*mit)) {
					//std::cout << "Sent " << mit->c_str() << " to manager\n";
					mit = vector.erase(mit);
					continue;
				}
				else
				{
					mit++;
				}
			}
			else
			{
				if (Utils::Train(*mit))
				{
					//std::cout << "Initiated training of " << mit->c_str() << "\n";
					mit = vector.erase(mit);
					continue;
				}
				else
				{
					mit++;
				}
			}
		}
	}
}

void Planner::OnStart()
{
}

void Planner::OnFrame()
{
	AddFromOrder();
	ExecuteOrders();
	commander.OnFrame();
}

void Planner::OnUnitComplete(BWAPI::Unit unit)
{
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
