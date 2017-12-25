#include "BaseLocation.h"
#include "bot.h"
#include <sstream>
#include "Utils.h"

BaseLocation::BaseLocation(uint32_t _baseLocationId, Point3D _resourceDepotLocation)
	: baseLocationId(_baseLocationId)
{
	resourceDepotLocation = _resourceDepotLocation;
}

bool BaseLocation::IsUnitInBase(const Unit* unit)
{
	if (Distance3D(resourceDepotLocation, unit->pos) < baseRadius)
		return true;
	return false;
}

void BaseLocation::AddMineralPatch(const Unit* patch)
{
	mineralPatches.push_back(patch);
}

void BaseLocation::AddGeyser(const Unit* geyser)
{
	geysers.push_back(geyser);
}

void BaseLocation::DrawSelf(Bot & bot)
{
	//TODO:  us, green, enemy, red, neut, gray
	Color color = Colors::Gray;

	//Highlight where the resource center is
	bot.Draw().DrawCircle(resourceDepotLocation, 2.75f, color);	//CC Radius is 2.75
	//And set a radius around the whole base location
	bot.Draw().DrawCircle(resourceDepotLocation, baseRadius, color);

	std::ostringstream oss;
	oss << "Base location " << baseLocationId << std::endl;
	bot.Draw().DrawText(oss.str(), resourceDepotLocation, color);
}

uint32_t BaseLocation::GetBaseLocationId()
{
	return baseLocationId;
}

Point3D BaseLocation::GetResourceDepotLocation()
{
	return resourceDepotLocation;
}
