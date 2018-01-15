#include "DataManager.h"
#include "bot.h"
using namespace sc2;

DataManager::DataManager(Bot & b)
	: ManagerBase(b)
	, mappedAbilityToUnitData(false)
	, mappedUnitData(false)
{
}

void DataManager::OnStep()
{
	//Not implemented
}

UnitData DataManager::GetUnitData(sc2::UNIT_TYPEID unitTypeID)
{
	return gameUnitData[unitTypeID];
}

UnitData DataManager::GetUnitData(sc2::ABILITY_ID abilityID)
{
	//Pre-map the ability to unit data, we need it constantly
	if (!mappedAbilityToUnitData) {
		MapAbilityToUnitData();
	}

	if (!mappedUnitData) {
		MapUnitData();
	}

	UNIT_TYPEID unitTypeID = mapAbilityToUnitType[abilityID];
	return GetUnitData(unitTypeID);
}

//TODO:  I'm just doing a 1-1 map of ids.  Still will require frequent lookups to game data.  Should we cache more?  Need
//	to do some speed analysis on this.
void DataManager::MapAbilityToUnitData()
{
	UnitTypes types = bot.Observation()->GetUnitTypeData();
	for (UnitTypeData data : types) {
		 mapAbilityToUnitType.insert(std::pair<sc2::ABILITY_ID, sc2::UNIT_TYPEID>(data.ability_id, data.unit_type_id));
	}

	mappedAbilityToUnitData = true;
}

//One time mapping of the game unit data.  None of this stuff changes, so there's no point going back to Observation()->GetUnitTypeData()
//	every frame to find it.
void DataManager::MapUnitData()
{
	UnitTypes types = bot.Observation()->GetUnitTypeData();
	for (UnitTypeData data : types) {
		//Convert to our local type
		UnitData myData(data);

		//Manual additions.  This is stuff we've just hardcoded so we've got access to it.
		AddProducingBuildings(myData);
		//... more here as needed

		gameUnitData.insert(std::pair<sc2::UNIT_TYPEID, UnitData>(data.unit_type_id, myData));
	}


	mappedUnitData = true;
}

void DataManager::AddProducingBuildings(UnitData& data)
{
	switch (data.ability_id.ToType()) {
	case ABILITY_ID::TRAIN_GHOST:
	case ABILITY_ID::TRAIN_MARAUDER:
	case ABILITY_ID::TRAIN_MARINE:
	case ABILITY_ID::TRAIN_REAPER:
		data.producingBuilding = UNIT_TYPEID::TERRAN_BARRACKS;
		break;

	case ABILITY_ID::TRAIN_HELLION:
	case ABILITY_ID::TRAIN_CYCLONE:
	case ABILITY_ID::TRAIN_SIEGETANK:
	case ABILITY_ID::TRAIN_THOR:
	case ABILITY_ID::TRAIN_WIDOWMINE:
		data.producingBuilding = UNIT_TYPEID::TERRAN_FACTORY;
		break;

	case ABILITY_ID::BUILD_NUKE:
		data.producingBuilding = UNIT_TYPEID::TERRAN_GHOSTACADEMY;
		break;

	case ABILITY_ID::TRAIN_BANSHEE:
	case ABILITY_ID::TRAIN_BATTLECRUISER:
	case ABILITY_ID::TRAIN_LIBERATOR:
	case ABILITY_ID::TRAIN_VIKINGFIGHTER:
	case ABILITY_ID::TRAIN_RAVEN:
	case ABILITY_ID::TRAIN_MEDIVAC:
		data.producingBuilding = UNIT_TYPEID::TERRAN_STARPORT;
		break;

	case ABILITY_ID::EFFECT_CALLDOWNMULE:
		data.producingBuilding = UNIT_TYPEID::TERRAN_ORBITALCOMMAND;
		break;

	}
}
