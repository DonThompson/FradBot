#pragma once
#include <sc2api/sc2_api.h>
class Squad;

class ArmyUnit
{
public:
	explicit ArmyUnit(const std::shared_ptr<Squad> & _parentSquad);
	ArmyUnit(const std::shared_ptr<Squad> & _parentSquad, const sc2::Unit* _unit);
	void OnStep();
	
	sc2::Point3D Position();
	size_t GetOrderCount();

//TODO:  Make private?
public:
	const sc2::Unit* unit;
private:
	std::weak_ptr<Squad> parentSquad;

};
