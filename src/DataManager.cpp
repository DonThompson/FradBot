#include "DataManager.h"
#include "bot.h"
using namespace sc2;

DataManager::DataManager(Bot & b)
	: ManagerBase(b)
	, mappedAbilityToUnitData(false)
{
}

void DataManager::OnStep()
{
	//Not implemented
}

sc2::UnitTypeData DataManager::GetUnitData(sc2::UNIT_TYPEID unitTypeID)
{
	UnitTypes ut = bot.Observation()->GetUnitTypeData();
	UnitTypeData data = ut[(UnitTypeID)unitTypeID];
	return data;
}

sc2::UnitTypeData DataManager::GetUnitData(sc2::ABILITY_ID abilityID)
{
	//Pre-map the ability to unit data, we need it constantly
	if (!mappedAbilityToUnitData) {
		MapAbilityToUnitData();
	}

	UNIT_TYPEID unitTypeID = mapAbilityToUnitType[abilityID];
	UnitTypes types = bot.Observation()->GetUnitTypeData();
	return types[(UnitTypeID)unitTypeID];
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
