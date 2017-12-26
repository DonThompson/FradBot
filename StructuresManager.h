#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "Structure.h"

class Bot;

/*
	v1 Usage:  A thin wrapper over structure related functionality.

	vFuture:  If we can do it better/for good reason, actually maintain our own list of all structures and 
		use them directly.  No clear need at present, document as it comes up.

*/

class StructuresManager : public ManagerBase
{
public:
	StructuresManager(Bot & b);
	static sc2::UNIT_TYPEID StructuresManager::UnitTypeFromBuildAbility(sc2::ABILITY_ID abilityID);

	virtual void OnStep();
	virtual void OnUnitDestroyed(const sc2::Unit* unit);
	virtual void OnBuildingConstructionComplete(const sc2::Unit* unit);

	//Public functionality
	std::vector<Structure> GetStructuresByType(sc2::UNIT_TYPEID unitTypeID);
	std::vector<Structure> GetStructuresByBuildAbility(sc2::ABILITY_ID abilityID);

	int32_t CountStructuresByType(sc2::UNIT_TYPEID unitTypeID);

private:
	//Perhaps in vFuture
	//std::vector<Structure> structures;

};
