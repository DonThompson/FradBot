#pragma once
#include <sc2api/sc2_api.h>
class Bot;

//Exists primarily to document the specifics learned about distance measuring and provide more clear guidance on which 
//	methods should be used in various scenarios.
class DistanceUtils
{
public:
	static const sc2::Unit* FindClosestUnitUsingRawDistance(sc2::Units unitsToSearch, const sc2::Point2D startingPoint);
	static const sc2::Unit* FindClosestUnitUsingPathingDistance(Bot & bot, sc2::Units unitsToSearch, const sc2::Point2D startingPoint);
};
