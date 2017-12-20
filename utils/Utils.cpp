#include "Utils.h"

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
				switch (static_cast<ABILITY_ID>(order.ability_id)) {
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

Units Utils::GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
{
	return observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
}

Units Utils::GetOwnUnits(const ObservationInterface* observation)
{
	return observation->GetUnits(Unit::Alliance::Self, nullptr);
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

//We are queueing buildings based on an ability -- but we want to evaluate buildings based on
//	the unit ID.  No mapping exists, so we must do this manually.  I found the big chunk of
//	defines in sc2_typeenums.h and ripped out the build ones only.
//TODO:  Only supporting terran for now.
UNIT_TYPEID Utils::UnitTypeFromBuildAbility(ABILITY_ID abilityID)
{
	UNIT_TYPEID type = UNIT_TYPEID::INVALID;

	switch (abilityID) 
	{
	case ABILITY_ID::BUILD_ARMORY:
		return UNIT_TYPEID::TERRAN_ARMORY;
		break;
	case ABILITY_ID::BUILD_BARRACKS:
		return UNIT_TYPEID::TERRAN_BARRACKS;
		break;
	case ABILITY_ID::BUILD_BUNKER:
		return UNIT_TYPEID::TERRAN_BUNKER;
		break;
	case ABILITY_ID::BUILD_COMMANDCENTER:
		return UNIT_TYPEID::TERRAN_COMMANDCENTER;
		break;
	case ABILITY_ID::BUILD_ENGINEERINGBAY:
		return UNIT_TYPEID::TERRAN_ENGINEERINGBAY;
		break;
	case ABILITY_ID::BUILD_FACTORY:
		return UNIT_TYPEID::TERRAN_FACTORY;
		break;
	case ABILITY_ID::BUILD_GHOSTACADEMY:
		return UNIT_TYPEID::TERRAN_GHOSTACADEMY;
		break;
	case ABILITY_ID::BUILD_MISSILETURRET:
		return UNIT_TYPEID::TERRAN_MISSILETURRET;
		break;
		//	BUILD_NUKE = 710,   // Target: None.
	case ABILITY_ID::BUILD_REACTOR:
		return UNIT_TYPEID::TERRAN_REACTOR;
		break;
	case ABILITY_ID::BUILD_REACTOR_BARRACKS:
		return UNIT_TYPEID::TERRAN_BARRACKSREACTOR;
		break;
	case ABILITY_ID::BUILD_REACTOR_FACTORY:
		return UNIT_TYPEID::TERRAN_FACTORYREACTOR;
		break;
	case ABILITY_ID::BUILD_REACTOR_STARPORT:
		return UNIT_TYPEID::TERRAN_STARPORTREACTOR;
		break;
	case ABILITY_ID::BUILD_REFINERY:
		return UNIT_TYPEID::TERRAN_REFINERY;
		break;
	case ABILITY_ID::BUILD_SENSORTOWER:
		return UNIT_TYPEID::TERRAN_SENSORTOWER;
		break;
	case ABILITY_ID::BUILD_STARPORT:
		return UNIT_TYPEID::TERRAN_STARPORT;
		break;
	case ABILITY_ID::BUILD_SUPPLYDEPOT:
		return UNIT_TYPEID::TERRAN_SUPPLYDEPOT;
		break;
	case ABILITY_ID::BUILD_TECHLAB:
		return UNIT_TYPEID::TERRAN_TECHLAB;
		break;
	case ABILITY_ID::BUILD_TECHLAB_BARRACKS:
		return UNIT_TYPEID::TERRAN_BARRACKSTECHLAB;
		break;
	case ABILITY_ID::BUILD_TECHLAB_FACTORY:
		return UNIT_TYPEID::TERRAN_FACTORYTECHLAB;
		break;
	case ABILITY_ID::BUILD_TECHLAB_STARPORT:
		return UNIT_TYPEID::TERRAN_STARPORTTECHLAB;
		break;

	}

	return type;

	/*
	BUILD_ASSIMILATOR = 882,   // Target: Unit.
	BUILD_BANELINGNEST = 1162,  // Target: Point.
	BUILD_CREEPTUMOR = 3691,  // Target: Point.
	BUILD_CREEPTUMOR_QUEEN = 1694,  // Target: Point.
	BUILD_CREEPTUMOR_TUMOR = 1733,  // Target: Point.
	BUILD_CYBERNETICSCORE = 894,   // Target: Point.
	BUILD_DARKSHRINE = 891,   // Target: Point.
	BUILD_EVOLUTIONCHAMBER = 1156,  // Target: Point.
	BUILD_EXTRACTOR = 1154,  // Target: Unit.
	BUILD_FLEETBEACON = 885,   // Target: Point.
	BUILD_FORGE = 884,   // Target: Point.
	BUILD_FUSIONCORE = 333,   // Target: Point.
	BUILD_GATEWAY = 883,   // Target: Point.
	BUILD_HATCHERY = 1152,  // Target: Point.
	BUILD_HYDRALISKDEN = 1157,  // Target: Point.
	BUILD_INFESTATIONPIT = 1160,  // Target: Point.
	BUILD_INTERCEPTORS = 1042,  // Target: None.
	BUILD_NEXUS = 880,   // Target: Point.
	BUILD_NYDUSNETWORK = 1161,  // Target: Point.
	BUILD_NYDUSWORM = 1768,  // Target: Point.
	BUILD_PHOTONCANNON = 887,   // Target: Point.
	BUILD_PYLON = 881,   // Target: Point.
	BUILD_ROACHWARREN = 1165,  // Target: Point.
	BUILD_ROBOTICSBAY = 892,   // Target: Point.
	BUILD_ROBOTICSFACILITY = 893,   // Target: Point.
	BUILD_SPAWNINGPOOL = 1155,  // Target: Point.
	BUILD_SPINECRAWLER = 1166,  // Target: Point.
	BUILD_SPIRE = 1158,  // Target: Point.
	BUILD_SPORECRAWLER = 1167,  // Target: Point.
	BUILD_STARGATE = 889,   // Target: Point.
	BUILD_STASISTRAP = 2505,  // Target: Point.
	BUILD_TEMPLARARCHIVE = 890,   // Target: Point.
	BUILD_TWILIGHTCOUNCIL = 886,   // Target: Point.
	BUILD_ULTRALISKCAVERN = 1159,  // Target: Point.
	*/
}
