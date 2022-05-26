#include "Commander.h"

#include "Utils.h"

#include <BWAPI.h>
#include <map>
#include <stack>
#include <iostream>

Commander::Commander()
{
	commands = std::map<BWAPI::Unit, std::stack<BWAPI::UnitCommand>>();
}

void Commander::PushCommand(BWAPI::Unit unit, BWAPI::UnitCommand command)
{
	if (!commands.count(unit))
	{
		commands[unit] = std::stack<BWAPI::UnitCommand>();
	}
	commands[unit].push(command);
}

void Commander::PopCommand(BWAPI::Unit unit)
{
	if (!commands.count(unit)) { return; }
	if (commands[unit].empty()) { return; }
	commands[unit].pop();
}

void Commander::OnFrame()
{
	auto it = commands.begin();
	while (it != commands.end())
	{
		const BWAPI::UnitCommand command = it->second.top();
		const BWAPI::UnitCommandType commandType = command.getType();
		const BWAPI::Unit unit = it->first;
		if (commandType == BWAPI::UnitCommandTypes::Build)
		{
			const BWAPI::TilePosition position = command.getTargetTilePosition();
			if (
				!BWAPI::Broodwar->canBuildHere(position, command.getUnitType())
				)
			{
				const BWAPI::Unit closestMineral = Utils::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());
				PopCommand(unit);
			}
			else
			{
				Utils::IssueCommand(unit, command);
			}
		} else if (commandType == BWAPI::UnitCommandTypes::Right_Click_Unit)
		{
			if (unit->isIdle())
			{
				Utils::IssueCommand(unit, command);
			}
		}
		it++;
	}
}
