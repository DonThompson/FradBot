#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;

/* Usage


TODO:  This is a quick class encapsulation for random placement during construction

*/

class ConstructionPlacement
{
public:
	ConstructionPlacement();
	Point2D GetBuildPoint(const Unit* builder);
};
