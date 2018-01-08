#include "Squad.h"
#include "Platoon.h"
using namespace sc2;

Squad::Squad(Platoon* _parent)
	: parentPlatoon(_parent)
{
}

void Squad::AddUnit(const sc2::Unit* unit)
{
	ArmyUnit au(unit);
	squadUnits.push_back(au);

	//Update our counts appropriately
	squadUnitCounts[unit->unit_type]++;
}

size_t Squad::CountUnitsByType(sc2::UNIT_TYPEID unitTypeID)
{
	return squadUnitCounts[unitTypeID];
}
