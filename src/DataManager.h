#pragma once
#include <sc2api\sc2_api.h>
#include "ManagerBase.h"
#include "UnitData.h"
class Bot;

class DataManager : public ManagerBase
{
public:
	DataManager(Bot & b);
	virtual void OnStep();

	UnitData GetUnitData(sc2::UNIT_TYPEID unitTypeID);
	UnitData GetUnitData(sc2::ABILITY_ID abilityID);

private:
	bool mappedAbilityToUnitData;
	std::map<sc2::ABILITY_ID, sc2::UNIT_TYPEID> mapAbilityToUnitType;
	void MapAbilityToUnitData();

	bool mappedUnitData;
	std::map<sc2::UNIT_TYPEID, UnitData> gameUnitData;
	void MapUnitData();

	//Manual hardcoded additions
	void AddProducingBuildings(UnitData& data);
};
