#include "IdleWorkerModule.h"
#include "bot.h"
#include "DistanceUtils.h"
using namespace sc2;

IdleWorkerModule::IdleWorkerModule(Bot & b)
	: ModuleBase(b)
{
}

//We only need to be notified for idle units, nowhere else
ModuleNotificationRequirement IdleWorkerModule::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, 0, true, false, false);
}

//Handle idle unit notifications.  We only care about SCVs
void IdleWorkerModule::OnUnitIdle(const sc2::Unit* unit)
{
	switch (unit->unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_SCV:			OnSCVIdle(unit);			break;
		default:															break;	//Not interested in any other units
	}
}

//Called when any SCV becomes idle.  Any idle scv should be put back to mineral gathering duty.
void IdleWorkerModule::OnSCVIdle(const sc2::Unit* unit)
{
	//Find the nearest mineral patch
	const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
	if (mineral_target == nullptr) {
		//FUTURE:  Seems like this will only happen if we mine the entire map clear.  Presumably all scv's will remain infinitely 
		//	idle and just keep blasting this function forever.
		std::cout << "FATAL:  No mineral targets available for idle SCV.  IdleWorkerModule self-disabling." << std::endl;
		//Shut down the module
		DisableModule();
		return;
	}
	bot.Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
}

//Finds the closest mineral patch to the given starting point.
const Unit* IdleWorkerModule::FindNearestMineralPatch(const Point2D& start)
{
	//First, just look at the minerals in the base we're already standing in.  Almost certainly that's the winner.  It also keeps us from running
	//	off to a nearby expansion after we build something near an edge.  Mine local!
	BaseLocation *loc = bot.BaseLocations().GetLocationByPosition(start);
	Units thisLocationMinerals = loc->GetMineralPatches();
	const Unit* target = DistanceUtils::FindClosestUnitUsingRawDistance(thisLocationMinerals, start);
	//Did we find minerals in this base location?
	if (target != nullptr)
		return target;

	//If we didn't find minerals in our base, just try the whole map.
	//	FUTURE:  This could obviously be much improved (search the next closest base, search all our bases, find a base with low workers, etc), but
	//	good enough for now.
	Units units = bot.Observation()->GetUnits(Unit::Alliance::Neutral);
	Units minerals;
	for (const auto& u : units) {
		if (Utils::IsMineralPatch(u->unit_type)) {
			minerals.push_back(u);
		}
	}
	//Go ahead and use pathing distance.  This is a reasonably rare event (we must have just mined out our base), so it's worth the slight
	//	performance hit to get a better destination.
	const Unit* target = DistanceUtils::FindClosestUnitUsingPathingDistance(bot, minerals, start);
	return target;
}
