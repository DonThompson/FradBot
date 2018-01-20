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

//Returns an ideal attack-move command for this unit.  Some units, like medivacs, can't attack and may need
//	another command for movement.
sc2::ABILITY_ID ArmyUnit::GetIdealAttackMoveCommand()
{
	//NOTE:  This is completely hardcoded by trial & error.  Medivacs are all I can think of right now.
	if (unit->unit_type.ToType() == UNIT_TYPEID::TERRAN_MEDIVAC) {
		//http://starcraft.wikia.com/wiki/Scan_Move
		return ABILITY_ID::SCAN_MOVE;
	}

	//Everyone else should attack.  Still not 100% settled on the difference between ATTACK AND ATTACK_ATTACK
	return ABILITY_ID::ATTACK_ATTACK;
}
