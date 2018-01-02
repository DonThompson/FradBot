#pragma once
#include <sc2api\sc2_api.h>
#include "ManagerBase.h"
class Bot;

class DataManager : public ManagerBase
{
public:
	DataManager(Bot & b);
	virtual void OnStep();

	sc2::UnitTypeData GetUnitData(sc2::UNIT_TYPEID unitTypeID);
	sc2::UnitTypeData GetUnitData(sc2::ABILITY_ID abilityID);

private:
	bool mappedAbilityToUnitData;
	std::map<sc2::ABILITY_ID, sc2::UNIT_TYPEID> mapAbilityToUnitType;
	void MapAbilityToUnitData();
};
