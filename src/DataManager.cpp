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
