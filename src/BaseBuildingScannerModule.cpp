#include "BaseBuildingScannerModule.h"
#include "ModuleBase.h"
#include "bot.h"
using namespace sc2;
using namespace std;

BaseBuildingScannerModule::BaseBuildingScannerModule(Bot & b)
	: ModuleBase(b)
{
}

ModuleNotificationRequirement BaseBuildingScannerModule::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, stepLoopCount, false, false, true, false, false, false, false, false);
}

void BaseBuildingScannerModule::OnStep()
{
	Units enemyBuildings = Utils::GetEnemyUnits(GetBot().Observation(), Utils::IsStructure(GetBot().Observation()));
	for (const Unit* unit : enemyBuildings) {
		//Put them in the appropriate base
		BaseLocation* base = GetBot().BaseLocations().GetLocationByPosition(unit->pos);
		if (base != nullptr) {
			if (!base->IsMyBase() && !base->IsEnemyBase()) {
				//it's enemy's now!
				base->SetEnemyBase();
			}
			base->AddKnownEnemyBuilding(unit);
		}
		else {
			//TODO:  What about buildings not in any base?
		}
	}
}

void BaseBuildingScannerModule::OnUnitDestroyed(const sc2::Unit* unit)
{
	//Is this an enemy unit?
	if (unit->alliance != Unit::Alliance::Enemy)
		return;

	//Is it a structure?
	if (Utils::IsStructure(GetBot().Observation()).operator()(*unit)) {
		BaseLocation* base = GetBot().BaseLocations().GetLocationByPosition(unit->pos);
		if (base != nullptr) {
			base->RemoveKnownEnemyBuilding(unit);

			//If this base now has no enemy buildings, reset it to neutral.
			if (base->GetKnownEnemyBuildings().size() == 0) {
				base->SetUnownedBase();
			}
		}
	}


}
