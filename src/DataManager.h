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
};
