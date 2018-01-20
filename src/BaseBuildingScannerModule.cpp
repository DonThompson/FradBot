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
	return ModuleNotificationRequirement(false, stepLoopCount, false, false, false, false, false, false, false, false);
}

void BaseBuildingScannerModule::OnStep()
{
	Units enemyBuildings = Utils::GetEnemyUnits(GetBot().Observation(), Utils::IsStructure(GetBot().Observation()));
	for (const Unit* unit : enemyBuildings) {
		//Put them in the appropriate base
		BaseLocation* base = GetBot().BaseLocations().GetLocationByPosition(unit->pos);
		if (!base->IsMyBase() && !base->IsEnemyBase()) {
			//it's enemy's now!
			base->SetEnemyBase();
		}
		base->AddKnownEnemyBuilding(unit);
	}
}
