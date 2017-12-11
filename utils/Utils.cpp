#include "Utils.h"

//Get a random peon that is currently harvesting (not returning, not building, not moving somewhere on another mission, etc).
const Unit* Utils::GetRandomHarvester(const ObservationInterface* observation)
{
	Units units = GetOwnUnits(observation, UNIT_TYPEID::TERRAN_SCV);

	//Max 5 tries, if it fails, try again next step
	const int tries = 5;
	for (int i = 0; i < tries; i++)
	{
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
				//Got something else useful going on, skip him
				std::cout << "Skipping builder because he's busy with action " << order.ability_id << std::endl;
			}
		}
	}

	//None found, try again later
	return nullptr;
}

bool Utils::TryBuildStructure(const ObservationInterface* observation, ActionInterface* actions, ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type /*= UNIT_TYPEID::TERRAN_SCV*/)
{
	//Get a builder to work with
	const Unit* unit_to_build = Utils::GetRandomHarvester(observation);
	if (unit_to_build == nullptr) {
		return false;
	}

	//Find a random place to build
	float rx = GetRandomScalar();
	float ry = GetRandomScalar();

	//Issue the action to the unit via the command menu to build and where
	actions->UnitCommand(unit_to_build, ability_type_for_structure, Point2D(unit_to_build->pos.x + rx * 15.0f, unit_to_build->pos.y + ry * 15.0f));

	return true;
}

int32_t Utils::CountOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
{
	return (int32_t)observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID)).size();
}

Units Utils::GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
{
	return observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
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
