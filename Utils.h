#ifndef __UTILS_H
#define __UTILS_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;

class Utils {
public:
	static const Unit* GetRandomHarvester(const ObservationInterface* observation);
	static int32_t CountOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID);
	static Units GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID);
	static Units GetOwnUnits(const ObservationInterface* observation);
	static Units GetIdleUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID, UNIT_TYPEID unitTypeID2);
};


#endif //__UTILS_H
