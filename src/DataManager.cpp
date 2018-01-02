#include "DataManager.h"
#include "bot.h"
using namespace sc2;

DataManager::DataManager(Bot & b)
	: ManagerBase(b)
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
	//TODO:  We should pre-map this

	//Iterate over all unit types and find one that has this ability mapped to it
	UnitTypes types = bot.Observation()->GetUnitTypeData();
	for (UnitTypeData data : types) {
		if (data.ability_id == abilityID)
			return data;
	}

	return types[(UnitTypeID)UNIT_TYPEID::INVALID];
}
