#include "SupplyDepotBuilderModule.h"
#include "bot.h"
#include "BuildQueueItem.h"
using namespace sc2;

SupplyDepotBuilderModule::SupplyDepotBuilderModule(Bot & b)
	: ModuleBase(b)
{
}

ModuleNotificationRequirement SupplyDepotBuilderModule::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, 0, false, false, true, false, true, false, false, false);
}

void SupplyDepotBuilderModule::OnBuildingConstructionComplete(const sc2::Unit* unit)
{
	//Stop the module when we hit 200 food
	if (GetBot().Observation()->GetFoodCap() >= 200) {
		DisableModule();
		return;
	}

	//Simply build a new supply depot each time one finishes
	if (unit->unit_type.ToType() == UNIT_TYPEID::TERRAN_SUPPLYDEPOT)
		QueueSupplyDepot();
}

void SupplyDepotBuilderModule::OnUnitDestroyed(const sc2::Unit* unit)
{
	//If we lose a depot, and our module is disabled, re-enable it.  We also need to force a rebuild now.
	if (!IsEnabled() && unit->unit_type.ToType() == UNIT_TYPEID::TERRAN_SUPPLYDEPOT) {
		QueueSupplyDepot();
		EnableModule();
	}
}

void SupplyDepotBuilderModule::QueueSupplyDepot()
{
	GetBot().BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));
}
