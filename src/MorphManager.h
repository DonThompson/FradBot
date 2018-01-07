#pragma once
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"

class MorphManager : public ManagerBase
{
public:
	static bool IsMorph(sc2::ABILITY_ID abilityID);

public:
	MorphManager(Bot & b);
	virtual void OnStep();
	bool PerformMorph(sc2::ABILITY_ID abilityID);

private:
	bool MorphCommandCenter(sc2::ABILITY_ID abilityID);
	bool AddOnToBarracks(sc2::ABILITY_ID abilityID);
	bool AddOnToFactory(sc2::ABILITY_ID abilityID);
	bool AddOnToStarport(sc2::ABILITY_ID abilityID);

	bool PerformAbilityOnFirstBuilding(sc2::UNIT_TYPEID buildingID, sc2::ABILITY_ID abilityID);
};
