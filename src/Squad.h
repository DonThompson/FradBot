#pragma once
#include <sc2api/sc2_api.h>
#include "ArmyUnit.h"

class Platoon;

//TODO:  Flag to lock down the squad when it moves out.  Don't want to be adding units to a squad that's across the map

class Squad
{
public:
	Squad(Platoon* _parent);

	void AddUnit(const sc2::Unit* unit);
	size_t CountUnitsByType(sc2::UNIT_TYPEID unitTypeID);
	size_t GetTotalSquadUnitCount();
	void OnStep();

	std::string GetDebugSummaryString();

private:
	Platoon* parentPlatoon;
	//All units
	std::vector<ArmyUnit> squadUnits;
	//Track squad unit counts for fast access
	std::map<sc2::UNIT_TYPEID, size_t> squadUnitCounts;

	//TODO:  operator casting as sc2::Units seems quite useful.  Needs to be super fast.  Worth duplicating tracking.
};
