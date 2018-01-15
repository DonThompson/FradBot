#include "VespeneWorkerBalanceModule.h"
#include <sc2api/sc2_api.h>
#include "Structure.h"
#include "bot.h"
using namespace sc2;

VespeneWorkerBalanceModule::VespeneWorkerBalanceModule(Bot & b)
	: ModuleBase(b)
{
}

ModuleNotificationRequirement VespeneWorkerBalanceModule::GetNotificationRequirements()
{
	//We need every 30th step or so.  Doesn't have to be all that frequent.  Rough guess.
	return ModuleNotificationRequirement(false, 30, false, false, false, false, false, false, false, false);
}

//Will be called only as often as we required above, not every game frame.
void VespeneWorkerBalanceModule::OnStep()
{
	BalanceGasWorkers();
}

//Version 1:  SIMPLE.  If we have a refinery < ideal harvesters, assign there.  Otherwise, assign to minerals.
void VespeneWorkerBalanceModule::BalanceGasWorkers()
{
	std::vector<Structure> refineries = GetBot().Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_REFINERY);
	for (Structure r : refineries) {
		if (r.IsBuildingComplete() && r.assignedHarvesters() < r.idealHarvesters()) {
			std::cout << "Moving harvester to gas refinery.  Previously assigned: " << r.assignedHarvesters() << ",  ideal: " << r.idealHarvesters() << std::endl;
			//TODO:  Get a harvester from this base, not one from anywhere on the map
			const Unit* unit = Utils::GetRandomHarvester(GetBot().Observation());
			GetBot().Actions()->UnitCommand(unit, ABILITY_ID::SMART, r.building);
			//Only do one
			return;
		}
	}
}
