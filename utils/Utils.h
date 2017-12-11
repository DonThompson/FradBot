#ifndef __UTILS_H
#define __UTILS_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;

class Utils {
public:
	static const Unit* GetRandomHarvester(const ObservationInterface* observation);
	static bool TryBuildStructure(const ObservationInterface* observation, ActionInterface* actions, ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type = UNIT_TYPEID::TERRAN_SCV);
	static int32_t CountOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID);
	static Units GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID);
	static Units GetIdleUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID, UNIT_TYPEID unitTypeID2);

};


#endif //__UTILS_H
