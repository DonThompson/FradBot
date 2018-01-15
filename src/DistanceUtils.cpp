#include "DistanceUtils.h"
#include "Utils.h"
#include "bot.h"
using namespace sc2;

//Given a set of units to search, finds the unit closest to the starting point.
//Utilizes raw distance, ignoring pathing or obstacles - this is the fastest search method.
//Raw distance is required if your target units might be in fog of war.
//Returns nullptr if no units are found.
/*static */const sc2::Unit* DistanceUtils::FindClosestUnitUsingRawDistance(sc2::Units unitsToSearch, const sc2::Point2D startingPoint)
{
	float closestDistance = std::numeric_limits<float>::max();
	const Unit* target = nullptr;

	//Iterate over all units in the search space, loooking for the closest
	for (const sc2::Unit* u : unitsToSearch) {
		float thisDistance = sc2::Distance2D(u->pos, startingPoint);
		if (thisDistance < closestDistance) {
			closestDistance = thisDistance;
			target = u;
		}
	}

	return target;
}


//Given a set of units to search, finds the unit closest to the starting point.
//Utilizes pathing distance.  Note that this may be slow, be cautious where you call it.
//Returns nullptr if no units are found.
/*static */const sc2::Unit* DistanceUtils::FindClosestUnitUsingPathingDistance(Bot & bot, sc2::Units unitsToSearch, const sc2::Point2D startingPoint)
{
	float closestDistance = std::numeric_limits<float>::max();
	const Unit* target = nullptr;

	//Iterate over all units in the search space, loooking for the closest
	for (const sc2::Unit* u : unitsToSearch) {
		//Use pathing distance, not air distance.
		//	NOTE:  Pathing using a unit appears to not work (returns 0) if the unit is in fog of war.  Explicitly use the position instead of PathingDistance(u, startingPoint).
		float thisDistance = bot.Query()->PathingDistance(u->pos, startingPoint);
		if (thisDistance < closestDistance && thisDistance > 0.0f) {
			closestDistance = thisDistance;
			target = u;
		}
	}

	return target;
}
