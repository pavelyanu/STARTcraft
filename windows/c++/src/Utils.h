#pragma once
#include <BWAPI.h>
#include "Commander.h"

namespace Utils
{
	const int MAXMAPSIZE = 150;
	void RightClick(BWAPI::Unit unit, BWAPI::Unit target);
	void RightClick(BWAPI::Unit unit, BWAPI::Position target);
	bool IssueCommand(BWAPI::Unit unit, BWAPI::UnitCommand command);
	bool IssueBuild(BWAPI::Unit unit, BWAPI::UnitType type, BWAPI::TilePosition position);
	BWAPI::Unit GetUnitOfType(BWAPI::UnitType type);
	BWAPI::Unit GetClosestUnitTo(BWAPI::Position p, const BWAPI::Unitset& units);
	BWAPI::Unit GetClosestUnitTo(BWAPI::Unit unit, const BWAPI::Unitset& units);
	int GetSupplyTotoal();
	bool Build(BWAPI::Unit builder, BWAPI::UnitType type, BWAPI::Position p);
	bool Build(BWAPI::UnitType type, BWAPI::Position p);
	bool Build(BWAPI::UnitType type);
	bool Build(BWAPI::UnitType type, Commander* commander);
	bool Train(BWAPI::UnitType type);
	bool Upgrade(BWAPI::UpgradeType type);
	bool IsProducable(BWAPI::UnitType type);
	bool IsProducable(BWAPI::UpgradeType type);
	void DrawUnitCommands();
	void DrawUnitIDs();
	void DrawExpansionBase();
	int CountUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units);
	void TrainWorkers(int max);
}
