#ifndef __UTILS_H
#define __UTILS_H

#include <iostream>
#include <sc2api/sc2_api.h>

class Utils {
public:
	static const sc2::Unit* GetRandomHarvester(const sc2::ObservationInterface* observation);
	static int32_t CountOwnUnits(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unitTypeID);
	static sc2::Units GetOwnUnits(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unitTypeID);
	static sc2::Units GetOwnUnits(const sc2::ObservationInterface* observation);
	static sc2::Units GetIdleUnits(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unitTypeID, sc2::UNIT_TYPEID unitTypeID2);
	static bool IsMineralPatch(sc2::UNIT_TYPEID unitTypeID);
	static bool IsVespeneGeyser(sc2::UNIT_TYPEID unitTypeID);
};

#endif //__UTILS_H
