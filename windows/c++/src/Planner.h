#pragma once
#include <BWAPI.h>
#include "Commander.h"
#include <map>
#include <vector>
#include <tuple>

#include "Manager.h"

class Planner
{
public:
	const int BUILDORDERCAP = 40;
	const int PROBECAP = 75;
	int lastOrder;

	Planner();

	void AddToOrder(BWAPI::UnitType type, int order);
	void AddToOrder(BWAPI::UnitType type, int order, int amount);
	void AddToOrder(BWAPI::UpgradeType type, int order);
	void AddFromOrder();
	void ExecuteOrders();
	void OnStart();
	void AddUnit(BWAPI::UnitType type);
	void AddUpgrade(BWAPI::UpgradeType type);
	void OnFrame();

	void OnUnitComplete(BWAPI::Unit unit);

	std::map<int, BWAPI::UnitType> buildOrder;
	std::map<int, BWAPI::UpgradeType> upgradeOrder;
	std::map<int, std::tuple<BWAPI::UnitType, int>> trainOrder;
	std::map<BWAPI::UnitType, std::vector<BWAPI::UnitType>> producers;
	std::map<BWAPI::UnitType, std::vector<BWAPI::UpgradeType>> upgraders;

	Commander commander;
	Manager manager;
	template<typename T>
	std::vector<int> GetKeysInRange(const std::map<int, T>* map, int lower, int upper);
};

