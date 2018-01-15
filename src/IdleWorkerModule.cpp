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
	return ModuleNotificationRequirement(false, 0, true, false, false, false, false, false, false, false);
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


//TODO:  At the start of the game, I iterate all neutral units to find mineral patches & build bases.  I keep those mineral patches for quick-access later.
//	I just learned that the const Unit* pointers that I'm keeping are actually "snapshot" type, not "visible" type, presumably because they're 
//	in fog of war.  I can't use snapshot type units in the command to tell an scv to mine.
//
//So I leave this as the 'ideal' implementation, but until I get better unit pointers for mineral patches, I'll have to search the whole space

//Finds the closest mineral patch to the given starting point.
const sc2::Unit* IdleWorkerModule::FindNearestMineralPatch__IDEAL_NOT_USED(const sc2::Point2D& start)
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
	target = DistanceUtils::FindClosestUnitUsingPathingDistance(bot, minerals, start);
	return target;
}

//See _IDEAL_ for more details.  We'll use this simple version for now.
const sc2::Unit* IdleWorkerModule::FindNearestMineralPatch(const sc2::Point2D& start)
{
	//If we didn't find minerals in our base, just try the whole map.
	Units units = bot.Observation()->GetUnits(Unit::Alliance::Neutral);
	Units minerals;
	for (const auto& u : units) {
		if (Utils::IsMineralPatch(u->unit_type)) {
			minerals.push_back(u);
		}
	}
	//Go ahead and use the pathing distance.  This keeps our scvs from going down to expansions because they just finished building near a cliff or such.
	return DistanceUtils::FindClosestUnitUsingPathingDistance(bot, minerals, start);
}
