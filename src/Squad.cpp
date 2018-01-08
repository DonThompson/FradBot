#include "Squad.h"
#include "Platoon.h"
#include <sstream>
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
