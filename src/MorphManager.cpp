#include "MorphManager.h"
#include "StructuresManager.h"
#include "bot.h"
using namespace sc2;

MorphManager::MorphManager(Bot & b)
	: ManagerBase(b)
{
}

//Stick addons (kinda morphs) and morphs (orbital, planetary) here
/*static*/bool MorphManager::IsMorph(sc2::ABILITY_ID abilityID)
{
	switch (abilityID) {
	case ABILITY_ID::BUILD_REACTOR_BARRACKS:
	case ABILITY_ID::BUILD_REACTOR_FACTORY:
	case ABILITY_ID::BUILD_REACTOR_STARPORT:
	case ABILITY_ID::BUILD_TECHLAB_BARRACKS:
	case ABILITY_ID::BUILD_TECHLAB_FACTORY:
	case ABILITY_ID::BUILD_TECHLAB_STARPORT:

	case ABILITY_ID::MORPH_ORBITALCOMMAND:
	case ABILITY_ID::MORPH_PLANETARYFORTRESS:
		return true;

	default:
		//Not a morph
	}

	return false;
}

void MorphManager::OnStep()
{
	//Not used
}

bool MorphManager::PerformMorph(sc2::ABILITY_ID abilityID)
{
	switch (abilityID) {
	case ABILITY_ID::BUILD_TECHLAB_BARRACKS:
		//Strangely, BUILD_TECHLAB_BARRACKS isn't actually an executable command.
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_BARRACKS, ABILITY_ID::BUILD_TECHLAB);
		break;
	case ABILITY_ID::BUILD_REACTOR_BARRACKS:
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_BARRACKS, ABILITY_ID::BUILD_REACTOR);
		break;

	case ABILITY_ID::BUILD_TECHLAB_FACTORY:
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_FACTORY, ABILITY_ID::BUILD_TECHLAB);
		break;
	case ABILITY_ID::BUILD_REACTOR_FACTORY:
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_FACTORY, ABILITY_ID::BUILD_REACTOR);
		break;

	case ABILITY_ID::BUILD_TECHLAB_STARPORT:
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_STARPORT, ABILITY_ID::BUILD_TECHLAB);
		break;
	case ABILITY_ID::BUILD_REACTOR_STARPORT:
		return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_STARPORT, ABILITY_ID::BUILD_REACTOR);
		break;

	case ABILITY_ID::MORPH_ORBITALCOMMAND:
	case ABILITY_ID::MORPH_PLANETARYFORTRESS:
		return MorphCommandCenter(abilityID);
		break;

	default:
		//Not a morph
		break;
	}

	//Not a morph, will never succeed
	return false;
}

bool MorphManager::MorphCommandCenter(sc2::ABILITY_ID abilityID)
{
	return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_COMMANDCENTER, abilityID);
}

bool MorphManager::AddOnToBarracks(sc2::ABILITY_ID abilityID)
{
	return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_BARRACKS, abilityID);
}

bool MorphManager::AddOnToFactory(sc2::ABILITY_ID abilityID)
{
	return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_FACTORY, abilityID);
}

bool MorphManager::AddOnToStarport(sc2::ABILITY_ID abilityID)
{
	return PerformAbilityOnFirstBuilding(UNIT_TYPEID::TERRAN_STARPORT, abilityID);
}

//Share code.  Might not use this in the future if we add more logic to pick which to upgrade
//TODO:  AddOns aren't quite right.  If it can't be built because of placement, the ability
//	will still be available.  As soon as 1 hits, we'll forever fail to upgrade anything.
bool MorphManager::PerformAbilityOnFirstBuilding(sc2::UNIT_TYPEID buildingID, sc2::ABILITY_ID abilityID)
{
	//TODO:  Any future logic to these?  Just pick them at random now
	for (Structure s : bot.Structures().GetStructuresByType(buildingID)) {
		if (s.HasAbilityAvailable(bot, abilityID)) {
			Actions()->UnitCommand(s.building, abilityID);
			return true;
		}
	}
	return false;
}
