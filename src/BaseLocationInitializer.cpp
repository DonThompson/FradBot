#include "BaseLocationInitializer.h"
#include "MineralLine.h"
#include "Utils.h"
using namespace sc2;

BaseLocationInitializer::BaseLocationInitializer(Bot & b, std::vector<BaseLocation> *locs)
	: bot(b)
	, baseLocations(locs)
	, nextBaseLocationId(1)
{
}

//Initialization routine - one time only.
//	Finds all base locations on the map by analyzing the resources available.
//	At completion, will have filled the baseLocations array with each found.
void BaseLocationInitializer::InitializeBaseLocations()
{
	//First, create a base where we've spawned.
	Structure cc = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_COMMANDCENTER).front();
	BaseLocation locMine(UseNextBaseLocationId(), cc.building->pos);
	locMine.SetMyStartingBase();
	locMine.SetResourceDepot(cc.building);
	locMine.SetRegionId(bot.Map().GetRegionIdFromPoint(cc.building->pos));
	locMine.SetChokePoints(bot.Map().GetRegionChokePoints(locMine.GetRegionId()));

	//Now figure out where all the mineral lines are throughout the map.
	std::vector<const Unit*> mineralPatches;
	std::vector<const Unit*> geysers;
	FindAllMineralsAndGeysers(&mineralPatches, &geysers);

	//Turn these into mineral lines
	std::vector<MineralLine> mineralLines = FindMineralLines(mineralPatches);

	//We now should have a set of mineral lines for each base location on the map.  Create BaseLocation objects
	//	from these.
	for (MineralLine line : mineralLines) {
		if (locMine.IsPointInBase(line.GetMineralCenterLocation())) {
			//Already there, just add the mineral patches
			for (const Unit* patch : line.GetMineralPatches()) {
				locMine.AddMineralPatch(patch);
			}
		}
		else {
			//New base
			baseLocations->push_back(SetupNewBaseLocation(line));
		}
	}
	//Lastly add our location
	baseLocations->push_back(locMine);

	//Lastly add our geysers and we're initialized
	AddGeysersToBases(geysers);
}

//Searches all neutral units to find mineral patches and vespene geysers.  Fills the 2 provided arrays with the resulting units
void BaseLocationInitializer::FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers)
{
	Units units = bot.Observation()->GetUnits(Unit::Alliance::Neutral, {});
	for (const Unit* u : units) {
		if (Utils::IsMineralPatch(u->unit_type))
			mineralPatches->push_back(u);
		else if(Utils::IsVespeneGeyser(u->unit_type))
			geysers->push_back(u);
	}
}

//Using a list of all minerals on the map, groups them into mineral lines based on their proximity to each other.
std::vector<MineralLine> BaseLocationInitializer::FindMineralLines(std::vector<const Unit*> nodes)
{
	std::vector<MineralLine> mineralLines;
	for (const Unit* node : nodes) {
		//See if it's in a mineral line
		bool found = false;
		for (MineralLine& line : mineralLines) {
			if (line.AreTheseMineralsNear(node)) {
				found = true;
				//Add it to this mineral line
				line.AddMineralPatch(node);
			}
		}

		if (!found) {
			//Must be a new mineral line
			mineralLines.push_back(MineralLine(node));
		}
	}
	return mineralLines;
}

//Adds a list of geyser locations to the already known bases on the map
void BaseLocationInitializer::AddGeysersToBases(std::vector<const Unit*>geysers)
{
	//Assumes every geyser fits in a base.  Any abandoned ones will just be ignored.
	for (const Unit* geyser : geysers) {
		//find the right base
		bool found = false;
		for (BaseLocation &l : (*baseLocations)) {
			if (l.IsUnitInBase(geyser) && !found) {
				l.AddGeyser(geyser);
				found = true;
			}
		}
	}
}

//Given a starting point, finds all points within an arbitrary bounding box that could be the home of a base's resource depot.
std::vector<Point2D> BaseLocationInitializer::GetBuildableStartingPoints(Point3D startingPoint)
{
	//NOTE:  This could be optimized.  The position is always between the startingPoint and middle of the map.  If we could eliminate
	//	everything outside that, we could reduce the search space significantly.  Debatable ROI at the moment.

	//Arbitrarily chosen based on just looking at the size of things in game and assuming this will be small enough to not consume too many resources but still find the spot.
	//	Examining 'BelShirVestigeLE.sc2map', we come out with anywhere from 16 to 40 buildable points from our search space.  This seems reasonable.
	const int boundingSize = 10;

	std::vector<Point2D> buildablePoints;
	//We intentionally search tiles as integer points only for simplicity.  This works out fine in practice.
	Point2DI topLeft(static_cast<int>(startingPoint.x) - boundingSize, static_cast<int>(startingPoint.y) - boundingSize);
	//Fill the map by iterating across the x and y axis.
	for (int x = topLeft.x; x < topLeft.x + (2 * boundingSize); x++)
	{
		for (int y = topLeft.y; y < topLeft.y + (2 * boundingSize); y++)
		{
			//Output needs to be back in normal float 2D points
			Point2D testPoint(static_cast<float_t>(x), static_cast<float_t>(y));
			bool buildable = bot.Query()->Placement(ABILITY_ID::BUILD_COMMANDCENTER, testPoint);

			if (buildable) {
				buildablePoints.push_back(testPoint);
			}
		}
	}
	return buildablePoints;
}

//Examines the pointList and finds the one closes to the starting point
Point2D BaseLocationInitializer::FindClosestPointTo(Point2D startingPoint, std::vector<Point2D> pointList)
{
	Point2D closestPoint(0, 0);
	float_t closestDistance = 100.0f;		//pick something far enough away to be further than anything this function might encounter
	for (Point2D pt : pointList) {
		//Calc distance.  OK to use raw distance, we're just looking in a small predefined space
		float_t thisDistance = Distance2D(startingPoint, pt);
		if (thisDistance < closestDistance) {
			//Winner, save it
			closestPoint = pt;
			closestDistance = thisDistance;
		}
	}

	return closestPoint;
}

BaseLocation BaseLocationInitializer::SetupNewBaseLocation(MineralLine mineralLine)
{
	//Here's roughly how we define the base location...
	//	* Using the center of a mineral line, define an arbitrary search space of 'tiles' around that line
	//	* The search space should be big enough to surely fit a command center, but not so big that
	//		we're searching tons of unnecessary tiles.
	//	* For each tile, check the Placement engine to see if a command center can be built there.  Save the winners.
	//	* Now figure out which winner is closest in distance to our starting mineral patch.  That's where the command 
	//		center should be built.
	
	//Starting point
	Point3D startingPoint(mineralLine.GetMineralCenterLocation());
	//List of buildable spots in search space around it
	std::vector<Point2D> buildablePoints = GetBuildableStartingPoints(startingPoint);
	//Find closest point
	Point2D closestPoint = FindClosestPointTo(startingPoint, buildablePoints);
	//And now we have our winning point.  Z matches our mineral patch.
	Point3D resourceDepotLocation = Point3D(closestPoint.x, closestPoint.y, startingPoint.z);
	//Setup a new base location at it.
	BaseLocation loc(UseNextBaseLocationId(), resourceDepotLocation);
	//Finally add all the mineral patches to the location for future use.
	for (const Unit* patch : mineralLine.GetMineralPatches()) {
		loc.AddMineralPatch(patch);
	}

	//What region is it in?
	//TODO:  region stuff is getting clunky
	loc.SetRegionId(bot.Map().GetRegionIdFromPoint(startingPoint));
	loc.SetChokePoints(bot.Map().GetRegionChokePoints(loc.GetRegionId()));

	return loc;
}

//Retrieves the next available location Id and increments the counter for the next caller
uint32_t BaseLocationInitializer::UseNextBaseLocationId()
{
	uint32_t useThis = nextBaseLocationId;
	nextBaseLocationId++;
	return useThis;
}
