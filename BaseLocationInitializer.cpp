#include "BaseLocationInitializer.h"

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
	std::vector<const Unit*> mineralPatches;
	std::vector<const Unit*> geysers;
	FindAllMineralsAndGeysers(&mineralPatches, &geysers);

	//We now have all the resources.  We want to sort them into base locations by distance from each other.  It's important that we
	//	do this by mineral patches first, not by geysers - they being on the ends is often too far away to detect without risk.
	CreateBaseLocationsFromResourceNodes(mineralPatches);
	CreateBaseLocationsFromResourceNodes(geysers);
}

//Searches all neutral units to find mineral patches and vespene geysers.  Fills the 2 provided arrays with the resulting units
void BaseLocationInitializer::FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers)
{
	Units units = bot.Observation()->GetUnits(Unit::Alliance::Neutral, {});
	for (const Unit* u : units) {
		if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD)
			mineralPatches->push_back(u);
		else if (u->unit_type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER)
			geysers->push_back(u);
	}
}

//Evaluates all the given resource nodes.  If its within a known base, adds the node to that base.  Otherwise, 
//	creates a new base location at the given position.
void BaseLocationInitializer::CreateBaseLocationsFromResourceNodes(std::vector<const Unit*> nodes)
{
	for (const Unit* node : nodes) {
		//Search all existing base locations to see if it's in any by radius
		bool found = false;
		for (BaseLocation &l : (*baseLocations)) {
			if (l.IsUnitInBase(node)) {
				found = true;
				//Already know this base, so add the node to it.
				if (node->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD)
					l.AddMineralPatch(node);
				else
					l.AddGeyser(node);
			}
		}

		if (!found) {
			//TODO: NEW
			BaseLocation baseNew(SetupNewBaseLocation(node));
			baseNew.AddMineralPatch(node);

			//TODO: OLD
			//New base!  Add it to our known list
			//BaseLocation locNew(UseNextBaseLocationId(), node);
			baseLocations->push_back(baseNew);
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
		//Calc distance
		float_t thisDistance = Distance2D(startingPoint, pt);
		if (thisDistance < closestDistance) {
			//Winner, save it
			closestPoint = pt;
			closestDistance = thisDistance;
		}
	}

	return closestPoint;
}

BaseLocation BaseLocationInitializer::SetupNewBaseLocation(const Unit* startingMineralPatch)
{
	//Here's roughly how we define the base location...
	//	* Using a new mineral patch, define an arbitrary search space of 'tiles' around that patch
	//	* The search space should be big enough to surely fit a command center, but not so big that
	//		we're searching tons of unnecessary tiles.
	//	* For each tile, check the Placement engine to see if a command center can be built there.  Save the winners.
	//	* Now figure out which winner is closest in distance to our starting mineral patch.  That's where the command 
	//		center should be built.
	
	//Starting point
	Point3D startingPoint(startingMineralPatch->pos);
	//List of buildable spots in search space around it
	std::vector<Point2D> buildablePoints = GetBuildableStartingPoints(startingPoint);
	//Find closest point
	Point2D closestPoint = FindClosestPointTo(startingPoint, buildablePoints);
	//And now we have our winning point.  Z matches our mineral patch.
	Point3D resourceDepotLocation = Point3D(closestPoint.x, closestPoint.y, startingPoint.z);
	//Setup a new base location at it.
	return BaseLocation(UseNextBaseLocationId(), resourceDepotLocation);
}

uint32_t BaseLocationInitializer::UseNextBaseLocationId()
{
	uint32_t useThis = nextBaseLocationId;
	nextBaseLocationId++;
	return useThis;
}
