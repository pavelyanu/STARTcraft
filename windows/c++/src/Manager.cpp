#include <BWAPI.h>

#include <memory>
#include <iostream>

#include "Manager.h"
#include "Role.h"
#include "Command.h"


void Manager::OnFrame()
{
	for (auto it = roles.begin(); it != roles.end(); it++)
	{
		it->second.get()->OnFrame();
	}
}

void Manager::OnStart()
{
}

void Manager::OnUnitComplete(BWAPI::Unit unit)
{
	if (unit->getPlayer() != BWAPI::Broodwar->self()) return;
	if (unit->getType().isWorker())
	{
		roles[unit] = std::make_unique<Worker>(unit);
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
		std::cout << unit->getType().c_str() << '\n';
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
				if (it->first->getType() == builderType)
				{
					roles[it->first].get()->PushCommand(std::move(std::make_unique<Commands::Build>(it->first, type)));
					std::cout << "Manager accepted task of buiding " << type.c_str() << "\n";
					return true;
				}
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
	std::string text = "";
	//for ()

	//BWAPI::Broodwar->drawTextMap(0, 0, )
}
