#include "StructuresManager.h"
#include "Utils.h"

StructuresManager::StructuresManager(Bot & b)
	: ManagerBase(b)
{
}

void StructuresManager::OnStep()
{

}

//We are queueing buildings based on an ability -- but we want to evaluate buildings based on
//	the unit ID.  No mapping exists, so we must do this manually.  I found the big chunk of
//	defines in sc2_typeenums.h and ripped out the build ones only.
//TODO:  Only supporting terran for now.
/*static*/UNIT_TYPEID StructuresManager::UnitTypeFromBuildAbility(ABILITY_ID abilityID)
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


//Called when workers, army units are destroyed.
//Also called when buildings are destroyed by violence.
//Also called when buildings are canceled during construction
void StructuresManager::OnUnitDestroyed(const Unit* unit)
{

}

void StructuresManager::OnBuildingConstructionComplete(const Unit* unit)
{

}

std::vector<Structure> StructuresManager::GetStructuresByType(UNIT_TYPEID unitTypeID)
{
	std::vector<Structure> matchingStructures;

	Units units = Utils::GetOwnUnits(Observation(), unitTypeID);
	for(const Unit* unit : units) {
		Structure s(unit);
		matchingStructures.push_back(s);
	}

	return matchingStructures;
}

//We may need to find a building but only know what we requested to build - the ability ID.  In that case, we 
//	need to map over to the real structure by ability ID.  For example, ABILITY_ID::BUILD_SUPPLYDEPOT maps to
//	UNIT_TYPEID::TERRAN_SUPPLYDEPOT
std::vector<Structure> StructuresManager::GetStructuresByBuildAbility(ABILITY_ID abilityID)
{
	//Just do the lookup then let our standard GetStructures* handle the work
	return GetStructuresByType(StructuresManager::UnitTypeFromBuildAbility(abilityID));
}

int32_t StructuresManager::CountStructuresByType(UNIT_TYPEID unitTypeID)
{
	//Cast it as a 32-bit int, we'll never have more than 32 bits of anything.  Makes all caller's lives easier.
	return static_cast<int32_t>(GetStructuresByType(unitTypeID).size());
}
