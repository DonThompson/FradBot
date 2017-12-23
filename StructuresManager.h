#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "managers/ManagerBase.h"
#include "utils/Utils.h"
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
	static UNIT_TYPEID StructuresManager::UnitTypeFromBuildAbility(ABILITY_ID abilityID);

	virtual void OnStep();
	virtual void OnUnitDestroyed(const Unit* unit);
	virtual void OnBuildingConstructionComplete(const Unit* unit);

	//Public functionality
	std::vector<Structure> GetStructuresByType(UNIT_TYPEID unitTypeID);
	std::vector<Structure> GetStructuresByBuildAbility(ABILITY_ID abilityID);

	int32_t CountStructuresByType(UNIT_TYPEID unitTypeID);

private:
	//Perhaps in vFuture
	//std::vector<Structure> structures;

};
