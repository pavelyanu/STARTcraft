#pragma once
#include <BWAPI.h>

#include <vector>
#include <memory>

#include "Role.h"

class Manager
{
public:
	void OnFrame();
	void OnStart();
	void OnUnitComplete(BWAPI::Unit unit);
	bool Build(BWAPI::UnitType type);
	void DrawRoles();
	std::map<BWAPI::Unit, std::unique_ptr<Role>> roles;
};