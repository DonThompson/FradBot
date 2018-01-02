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
		gameUnitData.insert(std::pair<sc2::UNIT_TYPEID, UnitData>(data.unit_type_id, myData));
	}

	mappedUnitData = true;
}
