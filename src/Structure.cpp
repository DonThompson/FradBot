#include "Structure.h"
#include "bot.h"
using namespace sc2;

//Float calculations
//	I mostly ran empirical tests and compared against what others have done for float-type safety in determining where things stand.
//	Generally, it seems safe to assume 0.0f is set if progress hasn't started yet and 1.0f is set if the building is complete.

Structure::Structure()
{
	building = nullptr;
}

Structure::Structure(const Unit* _building)
{
	building = _building;
}

bool Structure::IsBuildingComplete() {
	if (building->build_progress >= 1.0f)
		return true;
	return false;
}

bool Structure::IsBuildingInProgress()
{
	if (building->build_progress >= 1.0f) {
		//Building is done
		return false;
	}

	if (building->build_progress > 0.0f ) {
		//Building is in progress.
		return true;
	}

	//Building is unstarted, it is also not in progress.  It may become in progress in the next loop.
	return false;
}

int32_t Structure::assignedHarvesters()
{
	if (building != nullptr)
		return building->assigned_harvesters;
	else 
		return 0;
}

int32_t Structure::idealHarvesters()
{
	if (building != nullptr)
		return building->ideal_harvesters;
	else
		return 0;
}

bool Structure::HasAbilityAvailable(Bot & bot, sc2::AbilityID abilityID)
{
	AvailableAbilities aa = bot.Query()->GetAbilitiesForUnit(building);
	for (AvailableAbility a : aa.abilities) {
		if (a.ability_id == abilityID)
			return true;
	}

	return false;
}

bool Structure::IsTrainingUnit(sc2::ABILITY_ID abilityID)
{
	if (building == nullptr)
		return false;

	return std::find_if(building->orders.begin(), building->orders.end(),
		[=](UnitOrder o) -> bool { return o.ability_id == abilityID; }) != building->orders.end();
}

bool Structure::NeedsMoreWorkers()
{
	if (building == nullptr)
		return false;

	if (assignedHarvesters() < idealHarvesters())
		return true;
	return false;
}
