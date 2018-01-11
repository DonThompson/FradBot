#pragma once
#include <sc2api/sc2_api.h>
class Squad;

class ArmyUnit
{
public:
	ArmyUnit(std::shared_ptr<Squad> _parentSquad);
	ArmyUnit(std::shared_ptr<Squad> _parentSquad, const sc2::Unit* _unit);
	void OnStep();

//TODO:  Make private?
public:
	const sc2::Unit* unit;
private:
	std::weak_ptr<Squad> parentSquad;

};
