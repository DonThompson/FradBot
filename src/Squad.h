#pragma once
#include <sc2api/sc2_api.h>
#include "ArmyUnit.h"
#include "SquadOrders.h"
class Bot;
class Platoon;

//TODO:  Flag to lock down the squad when it moves out.  Don't want to be adding units to a squad that's across the map

class Squad : public std::enable_shared_from_this<Squad>
{
public:
	Squad(Bot & b, const std::shared_ptr<Platoon> & _parent);

	void AddUnit(const sc2::Unit* unit);
	size_t CountUnitsByType(sc2::UNIT_TYPEID unitTypeID);
	size_t GetTotalSquadUnitCount();
	void OnStep();
	std::string GetDebugSummaryString();

	sc2::Units AsUnits();
	bool HasOrders();
	void SetOrders(SquadOrders newOrders);
	void ClearOrders();
	sc2::Point3D GetCurrentPosition();
	bool HasGathered();
	const sc2::Unit* GetFirstRawUnit();
	void DrawSquadDetails();

private:
	Bot & bot;
	std::weak_ptr<Platoon> parentPlatoon;
	//All units
	std::vector<std::shared_ptr<ArmyUnit>> squadUnits;
	//Track squad unit counts for fast access
	std::map<sc2::UNIT_TYPEID, size_t> squadUnitCounts;
	//Current squad orders
	SquadOrders squadOrders;
	void ExecuteOrdersActionOnSquad();
	void ExecuteOrdersActionOnArmyUnit(std::shared_ptr<ArmyUnit> u);
	//Have all the units gathered up close enough to each other?  Once we gather for the first time, this is permanent.
	bool isGathered;
	
};
