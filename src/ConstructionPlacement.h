#pragma once

#include <sc2api/sc2_api.h>

/* Usage


TODO:  This is a quick class encapsulation for random placement during construction

*/

class ConstructionPlacement
{
public:
	ConstructionPlacement();
	sc2::Point2D GetBuildPoint(const sc2::Unit* builder);
};
