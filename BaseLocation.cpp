#include "BaseLocation.h"
#include "bot.h"
#include <sstream>
#include "Utils.h"

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
	bot.Draw().DrawBox(resourceDepotLocation, 5.50f, 5.50f, color);	//CC Radius is 2.75
	//And set a radius around the whole thing
	bot.Draw().DrawCircle(resourceDepotLocation, baseRadius, color);


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
	//	[BAD] * move a slight distance toward the center of the map
	//	* build a map of tiles, let's say 20x20?, around the resource point.  we'll define these as integer points
	//	* test to see if it's possible to build a resource depot here
	//	* repeat until we find all spots
	//	* figure out which is closest

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

	//TEST2
	//Build an integer-based tile system bounding the resourcecenter

	//Arbitrarily chosen based on just looking at the size of things in game and assuming this will be small enough to not consume too many resources but still find the spot.
	const int boundingSize = 10;
	//std::unordered_map<Point2D, bool> testMap;
	std::vector<Point2D> buildablePoints;

	Point2D topLeft(resourceCenterLocation.x - boundingSize, resourceCenterLocation.y - boundingSize);
	//Fill the map
	for (int x = static_cast<int>(topLeft.x); x < static_cast<int>(topLeft.x) + (2 * boundingSize); x++)
	{
		for (int y = static_cast<int>(topLeft.y); y < static_cast<int>(topLeft.y) + (2 * boundingSize); y++)
		{
			Point2D testPoint(static_cast<float_t>(x), static_cast<float_t>(y));
			bool buildable = bot.Query()->Placement(ABILITY_ID::BUILD_COMMANDCENTER, testPoint);

			if(buildable)
				buildablePoints.push_back(testPoint);
		}
	}

	//TODO:  too many winners?  can we throttle the size down?

	//The above works, finds lots of buildable places.  Iterate through them all and find the closest to our resource center
	Point2D closestPoint(0, 0);
	float_t closestDistance = 100.0f;
	for (Point2D pt : buildablePoints) {
		//Calc distance
		float_t thisDistance = Distance2D(resourceCenterLocation, pt);
		if (thisDistance < closestDistance) {
			//Winner, save it
			closestPoint = pt;
			closestDistance = thisDistance;
		}
	}

	//And now we have our winning point
	resourceDepotLocation = Point3D(closestPoint.x, closestPoint.y, lastZ * 1.05f);

	//NOTE:  This works!  These points are correct, they are indeed closest (I built expansions at all and lifted/dropped and they were correct).
	//	Additionally, the test doesn't seem too bad -- anywhere from 20-40 buildable tiles are found at each location to be tested.
}

uint32_t BaseLocation::GetBaseLocationId()
{
	return baseLocationId;
}

Point3D BaseLocation::GetResourceDepotLocation()
{
	return resourceDepotLocation;
}
