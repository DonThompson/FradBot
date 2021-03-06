#include "Utils.h"
using namespace sc2;

//Get a random peon that is currently harvesting (not returning, not building, not moving somewhere on another mission, etc).
const Unit* Utils::GetRandomHarvester(const ObservationInterface* observation)
{
	Units units = GetOwnUnits(observation, UNIT_TYPEID::TERRAN_SCV);

	//Max 5 tries, if it fails, try again next step
	const int tries = 5;
	for (int i = 0; i < tries; i++)
	{
		//We've probably lost at this point...
		if (units.size() == 0) {
			return nullptr;
		}

		//Get a completely random harvester
		const Unit* harvester = units[rand() % units.size()];
		if (harvester == nullptr) {
			return nullptr;
		}

		//See if this unit already has another order, we don't want to hijack him.
		for (const auto& order : harvester->orders) {
			//  We'll avoid anyone doing HARVEST_RETURN to ensure we maximize income
			if (order.ability_id == ABILITY_ID::HARVEST_GATHER) {
				return harvester;
			}
			else {
				switch (order.ability_id.ToType()) {
				case ABILITY_ID::HARVEST_RETURN:		//Money in hand, we don't want to take this away!
					break;
				default:
					break;
				}
			}
		}
	}

	//None found, try again later
	return nullptr;
}
int32_t Utils::CountOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
{
	return (int32_t)observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID)).size();
}

//Previously Utils::
Units Utils::GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
{
	return observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
}

Units Utils::GetOwnUnits(const ObservationInterface* observation)
{
	return observation->GetUnits(Unit::Alliance::Self, nullptr);
}

/*static*/Units Utils::GetEnemyUnits(const ObservationInterface* observation, sc2::Filter filter)
{
	return observation->GetUnits(Unit::Alliance::Enemy, filter);
}

Units Utils::GetIdleUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID, UNIT_TYPEID unitTypeID2)
{
	//TODO:  Need to find some info on Filter, this should be simpler.
	Units units1 = observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
	Units units2 = observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID2));

	Units idleUnits;
	for (const Unit* unit : units1) {
		if (unit->orders.size() == 0) {
			idleUnits.push_back(unit);
		}
	}
	for (const Unit* unit : units2) {
		if (unit->orders.size() == 0) {
			idleUnits.push_back(unit);
		}
	}

	return idleUnits;
}

//There are numerous types of mineral patches.  Consolidate logic.
bool Utils::IsMineralPatch(sc2::UNIT_TYPEID unitTypeID)
{
	switch (unitTypeID)
	{
	case UNIT_TYPEID::NEUTRAL_MINERALFIELD:
	case UNIT_TYPEID::NEUTRAL_MINERALFIELD750:
	case UNIT_TYPEID::NEUTRAL_LABMINERALFIELD:
	case UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750:
	case UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD:
	case UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD750:
		return true;
	default:
		//Not a mineral patch
		break;
	}
	return false;
}

//There are numerous types of geysers.  Consolidate logic.
bool Utils::IsVespeneGeyser(sc2::UNIT_TYPEID unitTypeID)
{
	switch (unitTypeID)
	{
	case UNIT_TYPEID::NEUTRAL_VESPENEGEYSER:
	case UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER:
	case UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER:
		return true;
	default:
		//Not a geyser
		break;
	}
	return false;
}

