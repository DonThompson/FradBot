#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;

class Bot;

class BaseLocation
{
public:
	BaseLocation(const Unit* startingMineralPatch);

	bool IsUnitInBase(const Unit* unit);

	void AddMineralPatch(const Unit* patch);
	void AddGeyser(const Unit* geyser);

	void DrawSelf(Bot & bot);

private:
	Point3D starterPointMinPatch;

	std::vector<const Unit*> mineralPatches;
	std::vector<const Unit*> geysers;


	//TODO:  Temp for now.  We should maintain a radius ourselves.  Or maybe an actual complex shape?
	const float_t baseRadius = 15.0f;
};

