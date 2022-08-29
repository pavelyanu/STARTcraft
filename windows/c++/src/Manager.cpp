#include <BWAPI.h>

#include <memory>
#include <iostream>

#include "Manager.h"
#include "Role.h"
#include "Command.h"


void Manager::OnFrame()
{
	std::cout << "Started OnFrame by manager. Size of the roles map " << roles.size() << "\n";
	for (auto it = roles.begin(); it != roles.end(); it++)
	{
		it->second.get()->OnFrame();
	}
	std::cout << "Finished OnFrame by manager. Size of the roles map " << roles.size() << "\n";
}

void Manager::OnStart()
{
}

void Manager::OnUnitComplete(BWAPI::Unit unit)
{
	std::cout << "Started OnUnitComplete by manager. Size of the roles map " << roles.size() << "\n";
	if (unit->getPlayer() != BWAPI::Broodwar->self() || unit->getType().isBuilding()) return;
	if (unit->getType().isWorker())
	{
		roles[unit] = std::make_unique<Worker>(unit);
	}
	else {
		std::cout << unit->getType().c_str() << '\n';
		roles[unit] = std::make_unique<Guard>(unit);
	}
	std::cout << "Finished OnUnitComplete by manager. Size of the roles map " << roles.size() << "\n";
}

bool Manager::Build(BWAPI::UnitType type)
{
	std::cout << "Started Build by manager. Size of the roles map " << roles.size() << "\n";
	//BWAPI::UnitType builderType = type.whatBuilds().first;
	//int mineralPrice = type.mineralPrice();
	//int gasPrice = type.gasPrice();
	//if (BWAPI::Broodwar->self()->minerals() >= mineralPrice && BWAPI::Broodwar->self()->gas() >= gasPrice)
	//{
	//	std::cout << "Enough of materials\n";
	//	std::cout << "Size of the role map: " << roles.size() << "\n";
	//	if (roles.size())
	//	{
	//		for (auto it = roles.begin(); it != roles.end(); )
	//		{
	//			if (it->first->getType() == builderType)
	//			{
	//				std::cout << "Found Builder\n";
	//				//roles[it->first].get()->PushCommand(std::move(std::make_unique<Commands::Build>(it->first, type)));
	//				return true;
	//			} else
	//			{
	//				std::cout << "Wrong builder " << it->first->getType().c_str() << " != " << builderType.c_str() << "\n";
	//			}
	//			++it;
	//		}
	//	}

		//auto units = BWAPI::Broodwar->self()->getUnits();
		//for (auto& unit : units)
		//{
		//	if (unit->getType() == builderType)
		//	{
		//		std::cout << "Found builder" << unit->getType().c_str() << unit->getID() << "\n";
		//		if (roles.count(unit)) {
		//			roles[unit].get()->PushCommand(std::move(std::make_unique<Commands::Build>(unit, type)));
		//		}
		//		std::cout << "No such builder\n";

		//	}
		//	std::cout << "Did not find builder\n";
		//}

	//} else
	//{
	//	std::cout << "Not enough materials\n";
	//}
	//std::cout << "Finished Build by manager. Size of the roles map " << roles.size() << "\n";
	return false;
}

void Manager::DrawRoles()
{
	std::string text = "";
	//for ()

	//BWAPI::Broodwar->drawTextMap(0, 0, )
}
