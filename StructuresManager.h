#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "managers/ManagerBase.h"
#include "utils/Utils.h"
#include "Structure.h"

class Bot;

class StructuresManager : public ManagerBase
{
public:
	StructuresManager(Bot & b);

	virtual void OnStep();
	virtual void OnUnitDestroyed(const Unit* unit);
	virtual void OnBuildingConstructionComplete(const Unit* unit);

	//Public functionality
	std::vector<Structure> GetStructuresByType(UNIT_TYPEID unitTypeID);
	std::vector<Structure> GetAllStructures();

private:
	std::vector<Structure> structures;
};
