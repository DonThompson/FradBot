#include "Squad.h"
#include "Platoon.h"
#include "bot.h"
#include <sstream>
using namespace sc2;

Squad::Squad(Bot & b, Platoon* _parent)
	: bot(b)
	, parentPlatoon(_parent)
	, squadOrders(SquadOrders::Empty())
{
}

void Squad::AddUnit(const sc2::Unit* unit)
{
	ArmyUnit au(this, unit);
	squadUnits.push_back(au);

	//Update our counts appropriately
	squadUnitCounts[unit->unit_type]++;
}

size_t Squad::CountUnitsByType(sc2::UNIT_TYPEID unitTypeID)
{
	return squadUnitCounts[unitTypeID];
}

size_t Squad::GetTotalSquadUnitCount()
{
	size_t size = 0;
	for (std::pair<sc2::UNIT_TYPEID, size_t> unitTypes : squadUnitCounts) {
		size += unitTypes.second;
	}
	return size;
}

std::string Squad::GetDebugSummaryString()
{
	//TODO:  Names
	std::string _name = "(TODO: Name)";

	std::ostringstream oss;
	oss << "  * Squad " << _name << std::endl;
	for (std::pair<sc2::UNIT_TYPEID, size_t> unitTypes : squadUnitCounts) {
		oss << "    * " << unitTypes.second << " " << sc2::UnitTypeToName(unitTypes.first) << std::endl;
	}

	return oss.str();
}

//Called each game step
void Squad::OnStep()
{
	//TODO:  throttle this?

	//Have we reached our target? If so the orders should be cleared.
	if (sc2::Distance2D(GetCurrentPosition(), squadOrders.currentTargetPoint) < 1.0f) {
		//We've arrived!
		ClearOrders();
	}

	/*
	for (ArmyUnit & au : squadUnits) {
		au.OnStep();
	}
	*/
}

//TODO:  Needs to be super fast.Worth duplicating tracking.  Is this fast enough?
Squad::operator const sc2::Units()
{
	sc2::Units units;
	for (ArmyUnit unit : squadUnits) {
		units.push_back(unit.unit);
	}
	return units;
}

bool Squad::HasOrders()
{
	return (squadOrders != SquadOrders::Empty());
}

void Squad::SetOrders(SquadOrders newOrders)
{
	squadOrders = newOrders;

	bot.Actions()->UnitCommand(operator const sc2::Units(), ABILITY_ID::ATTACK_ATTACK, squadOrders.currentTargetPoint);
}

void Squad::ClearOrders()
{
	squadOrders = SquadOrders::Empty();
}

//TODO:  Define/manage this.  For now, we just grab the first member
sc2::Point3D Squad::GetCurrentPosition()
{
	if (squadUnits.size() > 0) {
		return squadUnits.front().unit->pos;
	}

	//TODO:  Not sure this is a good idea either
	return Point3D(0, 0, 0);
}
