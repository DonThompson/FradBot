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
	static sc2::Units GetEnemyUnits(const sc2::ObservationInterface* observation, sc2::Filter filter = {});
	static sc2::Units GetIdleUnits(const sc2::ObservationInterface* observation, sc2::UNIT_TYPEID unitTypeID, sc2::UNIT_TYPEID unitTypeID2);
	static bool IsMineralPatch(sc2::UNIT_TYPEID unitTypeID);
	static bool IsVespeneGeyser(sc2::UNIT_TYPEID unitTypeID);


	//TODO:  From blizzard example code.  Get more of these implemented if they work
	//	https://github.com/Blizzard/s2client-api/blob/master/examples/common/bot_examples.cc
	struct IsStructure {
		explicit IsStructure(const sc2::ObservationInterface* obs) : observation_(obs) {};

		bool operator()(const sc2::Unit& unit) {
			auto& attributes = observation_->GetUnitTypeData().at(unit.unit_type).attributes;
			bool is_structure = false;
			for (const auto& attribute : attributes) {
				if (attribute == sc2::Attribute::Structure) {
					is_structure = true;
				}
			}
			return is_structure;
		}

		const sc2::ObservationInterface* observation_;
	};
};

#endif //__UTILS_H
