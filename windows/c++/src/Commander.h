#pragma once
#include <BWAPI.h>
#include <map>
#include <stack>
class Commander
{
public:
	Commander();

	void PushCommand(BWAPI::Unit unit, BWAPI::UnitCommand command);
	void PopCommand(BWAPI::Unit unit);
	void OnFrame();

	std::map<BWAPI::Unit, std::stack<BWAPI::UnitCommand>> commands;
};

