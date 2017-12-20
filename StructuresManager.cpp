#include "StructuresManager.h"

StructuresManager::StructuresManager(Bot & b)
	: ManagerBase(b)
{
}

void StructuresManager::OnStep()
{

}

//Called when workers, army units are destroyed.
//Also called when buildings are destroyed by violence.
//Also called when buildings are canceled during construction
void StructuresManager::OnUnitDestroyed(const Unit* unit)
{

}

void StructuresManager::OnBuildingConstructionComplete(const Unit* unit)
{

}

std::vector<Structure> GetStructuresByType(UNIT_TYPEID unitTypeID)
{
	//TODO - need unit->structure conversion
	//Utils::blahblah
	return std::vector<Structure>();
}

std::vector<Structure> GetAllStructures()
{
	//TODO - need unit->structure conversion
	//Utils::blahblah
	return std::vector<Structure>();
}
