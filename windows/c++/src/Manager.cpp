#include <BWAPI.h>

#include <memory>
#include <iostream>

#include "Manager.h"
#include "Role.h"
#include "Command.h"
#include "Utils.h"


void Manager::OnFrame()
{
	for (auto it = roles.begin(); it != roles.end();)
	{
		if (!it->first->exists())
		{
			it = roles.erase(it);
		}
		else
		{
			it->second.get()->OnFrame();
			++it;
		}
	}
	for (auto it = buildOrders.begin(); it != buildOrders.end();)
	{
		if (buildOrders[it->first] > 100)
		{
			Build(it->first);
		}
		else
		{
			buildOrders[it->first] = it->second + 1;
		}
		++it;
	}
	if (BWAPI::Broodwar->self()->supplyUsed() / 2 > 110 && raidTimeOut < 0)
	{
		raidTimeOut += 100;
		for (auto& it : roles)
		{
			BWAPI::Unit unit = it.first;
			BWAPI::UnitType type = unit->getType();
			if (type == BWAPI::UnitTypes::Protoss_Zealot || type == BWAPI::UnitTypes::Protoss_Dragoon)
			{
				roles[unit] = std::make_unique<Attacker>(unit, maptools);
			}
			
		}
		std::cout << "Sending raid!\n";
	}
	raidTimeOut--;
}

void Manager::OnStart()
{
	sentSpy = false;
	raidTimeOut = 0;
	enemyBase = BWAPI::Broodwar->self()->getStartLocation();;
}

void Manager::OnUnitComplete(BWAPI::Unit unit)
{
	if (buildOrders.count(unit->getType()))
	{
		buildOrders.erase(unit->getType());
	}
	if (unit->getPlayer() != BWAPI::Broodwar->self()) return;
	if (unit->getType().isWorker())
	{
		roles[unit] = std::make_unique<Worker>(unit);
	}
	else if (unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
	{
		if (sentSpy)
		{
			roles[unit] = std::make_unique<Observer>(unit, maptools);
		}
		else
		{
			roles[unit] = std::make_unique<Spy>(unit, maptools);
			sentSpy = true;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
	{
		BWAPI::Unitset workers = unit->getUnitsInRadius(
			1024,
			BWAPI::Filter::GetPlayer == BWAPI::Broodwar->self() &&
			BWAPI::Filter::IsWorker
		);
		int i = 0;
		auto& it = workers.begin();
		while (i < std::min(3, static_cast<int>(workers.size())))
		{
			if (!roles.count(*it))
			{
				++it;
			}
			roles[*it] = std::make_unique<Worker>(*it, unit);
			++i;
			++it;
		}
	}
	else if (!unit->getType().isBuilding())
	{
		roles[unit] = std::make_unique<Guard>(unit);
	}
}

bool Manager::Build(BWAPI::UnitType type)
{
	BWAPI::UnitType builderType = type.whatBuilds().first;
	int mineralPrice = type.mineralPrice();
	int gasPrice = type.gasPrice();
	if (BWAPI::Broodwar->self()->minerals() >= mineralPrice && BWAPI::Broodwar->self()->gas() >= gasPrice)
	{
		if (roles.size())
		{
			for (auto it = roles.begin(); it != roles.end(); )
			{
				if (it->first->getType() == builderType && BWAPI::Broodwar->canMake(type, it->first))
				{
					if (it->second.get()->GetCurrentCommand())
					{
						if (it->second.get()->GetCurrentCommand()->type() == build)
						{
							++it;
							//std::cout << "Skipping worker " << it->first->getID() << ". It is busy\n";
							continue;
						}
					}
					//std::cout << "Manager accepted task of buiding " << type.c_str() << "\n";
					it->second.get()->PushCommand(std::move(std::make_unique<Commands::Build>(it->first, type)));
					buildOrders[type] = 1;
					return true;
				}
				//std::cout << "Manager looking for builder\n";
				++it;
			}
		}
	} else
	{
	}
	return false;
}

void Manager::DrawRoles()
{
	if (roles.empty()) { return;  }
	std::string text = "";
	BWAPI::Unit unit;
	Role* role;
	Command* command;
	std::string commandName;
	for (auto it = roles.begin(); it != roles.end(); it++)
	{
		unit = it->first;
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Probe) continue;
		role = it->second.get();
		command = role->GetCurrentCommand();
		if (command)
		{
			commandName = command->name();
		}
		else
		{
			commandName = "Nothing";
		}
		text = text
			+ it->first->getType().c_str()
			+ ": " + role->name()
			+ std::to_string(unit->getID())
			+ " doing "
			+ commandName + "\n";
	}
	BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Small);
	BWAPI::Broodwar->drawTextScreen(0, 0, text.c_str());
	BWAPI::Broodwar->setTextSize();
}

