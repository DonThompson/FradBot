#include "ArmyUnit.h"
#include "Squad.h"
using namespace sc2;

ArmyUnit::ArmyUnit(const std::shared_ptr<Squad> & _parentSquad)
	: ArmyUnit(_parentSquad, (const sc2::Unit*)nullptr)
{
	
}

ArmyUnit::ArmyUnit(const std::shared_ptr<Squad> & _parentSquad, const sc2::Unit* _unit)
	: parentSquad(_parentSquad)
	, unit(_unit)
{
}

//Called each game step
void ArmyUnit::OnStep()
{
	//TODO:  Do something
}

Point3D ArmyUnit::Position()
{
	return unit->pos;
}

size_t ArmyUnit::GetOrderCount()
{
	return unit->orders.size();
}
