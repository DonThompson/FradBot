#pragma once
#include <sc2api/sc2_api.h>

class ArmyUnit
{
public:
	ArmyUnit();
	ArmyUnit(const sc2::Unit* _unit);

//TODO:  Make private?
public:
	const sc2::Unit* unit;
private:

};
