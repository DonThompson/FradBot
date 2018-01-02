#include "StructuresManager.h"
#include "Utils.h"
#include "bot.h"
using namespace sc2;

StructuresManager::StructuresManager(Bot & b)
	: ManagerBase(b)
{
}

void StructuresManager::OnStep()
{

}

bool StructuresManager::IsBuilding(sc2::ABILITY_ID abilityID)
{
	switch (abilityID)
	{
	case ABILITY_ID::BUILD_ARMORY:
	case ABILITY_ID::BUILD_BARRACKS:
	case ABILITY_ID::BUILD_BUNKER:
	case ABILITY_ID::BUILD_COMMANDCENTER:
	case ABILITY_ID::BUILD_ENGINEERINGBAY:
	case ABILITY_ID::BUILD_FACTORY:
	case ABILITY_ID::BUILD_GHOSTACADEMY:
	case ABILITY_ID::BUILD_MISSILETURRET:
	case ABILITY_ID::BUILD_REACTOR:
	case ABILITY_ID::BUILD_REACTOR_BARRACKS:
	case ABILITY_ID::BUILD_REACTOR_FACTORY:
	case ABILITY_ID::BUILD_REACTOR_STARPORT:
	case ABILITY_ID::BUILD_REFINERY:
	case ABILITY_ID::BUILD_SENSORTOWER:
	case ABILITY_ID::BUILD_STARPORT:
	case ABILITY_ID::BUILD_SUPPLYDEPOT:
	case ABILITY_ID::BUILD_TECHLAB:
	case ABILITY_ID::BUILD_TECHLAB_BARRACKS:
	case ABILITY_ID::BUILD_TECHLAB_FACTORY:
	case ABILITY_ID::BUILD_TECHLAB_STARPORT:
		return true;
	}

	return false;
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

int32_t StructuresManager::CountStructuresByType(UNIT_TYPEID unitTypeID)
{
	//Cast it as a 32-bit int, we'll never have more than 32 bits of anything.  Makes all caller's lives easier.
	return static_cast<int32_t>(GetStructuresByType(unitTypeID).size());
}
