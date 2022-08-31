#pragma once
#include <BWAPI.h>

#include <vector>
#include <memory>

#include "Role.h"
#include "MapTools.h"

class Manager
{
public:
	void OnFrame();
	void OnStart();
	void OnUnitComplete(BWAPI::Unit unit);
	bool Build(BWAPI::UnitType type);
	void DrawRoles();
	bool sentSpy;
	int raidTimeOut;
	std::map<BWAPI::Unit, std::unique_ptr<Role>> roles;
	std::map<BWAPI::UnitType, int> buildOrders;
	MapTools* maptools;
	BWAPI::TilePosition enemyBase;
};
