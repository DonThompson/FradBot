#include "BaseLocation.h"
#include "bot.h"
#include <sstream>

BaseLocation::BaseLocation(uint32_t _baseLocationId, const Unit* startingMineralPatch)
	: baseLocationId(_baseLocationId)
{
	starterPointMinPatch = startingMineralPatch->pos;
	AddMineralPatch(startingMineralPatch);
}

bool BaseLocation::IsUnitInBase(const Unit* unit)
{
	//TODO:  For now, each base is 15.0f units in radius.  This isn't perfect.
	if (Distance3D(starterPointMinPatch, unit->pos) < baseRadius)
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

	//First, simply draw a circle around us
	bot.Draw().DrawCircle(starterPointMinPatch, baseRadius, color);

	/*
	//and highlight our center point by drawing 5 concentric rings
	bot.Draw().DrawCircle(starterPointMinPatch, 0.1f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.2f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.3f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.4f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.5f, color);
	*/

	//and highlight our resource center
	bot.Draw().DrawCircle(resourceCenterLocation, 0.1f, color);
	bot.Draw().DrawCircle(resourceCenterLocation, 0.2f, color);
	bot.Draw().DrawCircle(resourceCenterLocation, 0.3f, color);

	//and box in resource depot center
	bot.Draw().DrawBox(resourceDepotLocation, 0.5f, 0.5f, color);		//TODO:  Get cc size

	//Now highlight the mineral patches
	for (const Unit* patch : mineralPatches) {
		bot.Draw().DrawCircle(patch->pos, patch->radius, color);
	}

	//and geysers
	for (const Unit* geyser : geysers) {
		bot.Draw().DrawCircle(geyser->pos, geyser->radius, color);
	}

	std::ostringstream oss;
	oss << "Base location " << baseLocationId << std::endl;
	bot.Draw().DrawText(oss.str(), resourceCenterLocation, color);
}

//Only call once
//Takes a base location with mineral patches and geysers and performs some one time calculations
//	and setup on positioning.
void BaseLocation::Initialize(Bot & bot, Point2D centerOfMap)
{
	//Here's a rough algorithm we'll try to follow...
	//	* find the center of our minerals x/y pos
	//	* we know minerals are typically curved surrounding the middle of the map
	//	* move a slight distance toward the center of the map
	//	* test to see if it's possible to build a resource depot here
	//	* repeat until we find a good spot
	//	* todo:  do we need to find a few then determine the closest?

	float_t totalX = 0.0f, totalY = 0.0f, lastZ = 0.0f;
	for (const Unit* patch : mineralPatches) {
		totalX += patch->pos.x;
		totalY += patch->pos.y;
		lastZ = patch->pos.z;
	}
	float_t centerX = totalX / mineralPatches.size();
	float_t centerY = totalY / mineralPatches.size();
	//TEMP;  house in class so we can draw it
	resourceCenterLocation = Point3D(centerX, centerY, lastZ);

	//Center the map at the same z-coord
	Point3D mapCenter(centerOfMap.x, centerOfMap.y, lastZ);
	
	//This doesn't work
	/*
	uint32_t numTries = 0;
	const uint32_t maxTries = 20;
	bool found = false;
	Point2D lastPoint(resourceCenterLocation.x, resourceCenterLocation.y);
	while (!found && numTries < maxTries) {
		Point2D testPoint2D(lastPoint + Point3D(0.5f, 0.5f, 0.0f));
		Point3D testPoint3D(testPoint2D.x, testPoint2D.y, lastZ);
		bot.Draw().DrawCircle(testPoint3D, 0.2f, Colors::Red);

		if (bot.Query()->Placement(ABILITY_ID::BUILD_COMMANDCENTER, testPoint2D)) {
			//Yes, it's buildable!
			found = true;
			resourceDepotLocation = testPoint3D;
		}
		else {
			//nope, keep trying
			numTries++;
		}
	}
	*/
}
