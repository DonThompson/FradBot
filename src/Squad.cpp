#include "Squad.h"
#include "Platoon.h"
#include "bot.h"
#include <sstream>
using namespace sc2;
using namespace std;

Squad::Squad(Bot & b, std::shared_ptr<Platoon> _parent)
	: bot(b)
	, parentPlatoon(_parent)
	, squadOrders(SquadOrders::Empty())
{
}

void Squad::AddUnit(const sc2::Unit* unit)
{
	shared_ptr<ArmyUnit> au = make_shared<ArmyUnit>(shared_from_this(), unit);
	squadUnits.push_back(au);

	//Update our counts appropriately
	squadUnitCounts[unit->unit_type]++;

	//This unit needs to follow any squad target orders.  If we don't have orders, he should
	//	try to group up with the rest of his squad.
	if(HasOrders())
		ExecuteOrdersActionOnArmyUnit(au);
	else {
		//TODO:  better way to handle this?  First unit in the squad will just move to themselves
		bot.Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, GetCurrentPosition());
	}
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
	//TEMP
	std::ostringstream oss;
	Color color = Colors::White;
	if (HasOrders()) {
		oss << "Squad => " << squadOrders.currentTargetPoint.x << ", " << squadOrders.currentTargetPoint.y;
		color = Colors::Purple;
	}
	else {
		oss << "Squad";
	}
	bot.Draw().DrawTextOnMap(oss.str(), GetCurrentPosition(), color);
	//END TEMP


	//TODO:  throttle this?

	//Have we reached our target? If so the orders should be cleared.
	//TODO:  Picked 3.0 because marines were pushing enough so that they somehow missed the point, but got to the 
	//	action destination.
	//TODO:  Should the squad detect lack of sc2::Unit::orders.size() with HasOrders()?
	if (sc2::Distance2D(GetCurrentPosition(), squadOrders.currentTargetPoint) < 3.0f) {
		//We've arrived!
		ClearOrders();

		//Let our parent platoon know
		shared_ptr<Platoon> p = parentPlatoon.lock();
		if (p) {
			p->OnSquadOrdersAchieved();
		}
	}

	/*
	for (shared_ptr<ArmyUnit> unit : squadUnits) {
		au->OnStep();
	}
	*/
}

//TODO:  Needs to be super fast.Worth duplicating tracking.  Is this fast enough?
Squad::operator const sc2::Units()
{
	sc2::Units units;
	for (shared_ptr<ArmyUnit> unit : squadUnits) {
		units.push_back(unit->unit);
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

	ExecuteOrdersActionOnSquad();
}

void Squad::ClearOrders()
{
	squadOrders = SquadOrders::Empty();
}

//TODO:  Define/manage this.  For now, we just grab the first member
sc2::Point3D Squad::GetCurrentPosition()
{
	if (squadUnits.size() > 0) {
		return squadUnits.front()->unit->pos;
	}

	//TODO:  Not sure this is a good idea either
	return Point3D(0, 0, 0);
}

void Squad::ExecuteOrdersActionOnSquad()
{
	//Don't execute if we don't have orders
	if (!HasOrders())
		return;

	std::cout << "Sending squad @ target: " << squadOrders.currentTargetPoint.x << ", " << squadOrders.currentTargetPoint.y << std::endl;
	bot.Actions()->UnitCommand(operator const sc2::Units(), ABILITY_ID::ATTACK_ATTACK, squadOrders.currentTargetPoint);
}

void Squad::ExecuteOrdersActionOnArmyUnit(shared_ptr<ArmyUnit> u)
{
	//Don't execute if we don't have orders
	if (!HasOrders())
		return;

	std::cout << "Sending single unit @ target: " << squadOrders.currentTargetPoint.x << ", " << squadOrders.currentTargetPoint.y << std::endl;
	bot.Actions()->UnitCommand(u->unit, ABILITY_ID::ATTACK_ATTACK, squadOrders.currentTargetPoint);
}