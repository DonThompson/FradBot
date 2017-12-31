#include "ConstructionPlacement.h"
using namespace sc2;

ConstructionPlacement::ConstructionPlacement()
{
}

//TODO:  Setup as minimum needed to just move the code out of the construction manager
Point2D ConstructionPlacement::GetBuildPoint(const Unit* builder)
{
	//Find a random place to build
	float rx = sc2::GetRandomScalar();
	float ry = sc2::GetRandomScalar();
	Point2D buildPoint(builder->pos.x + rx * 15.0f, builder->pos.y + ry * 15.0f);
	return buildPoint;
}
