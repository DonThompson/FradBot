#pragma once
#include <sc2api/sc2_api.h>
#include "ArmyUnit.h"
#include "SquadOrders.h"
class Bot;
class Platoon;

//TODO:  Flag to lock down the squad when it moves out.  Don't want to be adding units to a squad that's across the map

class Squad
{
public:
	Squad(Bot & b, Platoon & _parent);

	void AddUnit(const sc2::Unit* unit);
	size_t CountUnitsByType(sc2::UNIT_TYPEID unitTypeID);
	size_t GetTotalSquadUnitCount();
	void OnStep();
	std::string GetDebugSummaryString();
	operator const sc2::Units();
	bool HasOrders();
	void SetOrders(SquadOrders newOrders);
	void ClearOrders();
	sc2::Point3D GetCurrentPosition();

private:
	Bot & bot;
	Platoon & parentPlatoon;
	//All units
	std::vector<ArmyUnit> squadUnits;
	//Track squad unit counts for fast access
	std::map<sc2::UNIT_TYPEID, size_t> squadUnitCounts;
	//Current squad orders
	SquadOrders squadOrders;
	
};
