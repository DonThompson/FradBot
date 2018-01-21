#include "MapManager.h"
#include "bot.h"
#include "MapImpl.h"		//Should only ever be included in this cpp file.  Constrains us to never exposing the Overseer library map info directly.
using namespace sc2;
using namespace std;

MapManager::MapManager(Bot & b)
	: ManagerBase(b)
{
	overseerMapImpl = new MapImpl();
}

MapManager::~MapManager()
{
	if (overseerMapImpl != nullptr) {
		delete overseerMapImpl;
		overseerMapImpl = nullptr;
	}
}

void MapManager::OnStep()
{
	//Not implemented
}

void MapManager::OnGameStart()
{
	//Configure overseer
	overseerMapImpl->setBot(static_cast<sc2::Agent*>(&bot));
	overseerMapImpl->Initialize();	//Read the map

	std::cout << "Number of tiles on map: " << overseerMapImpl->size() << std::endl;
	std::cout << "Number of regions: " << overseerMapImpl->getRegions().size() << std::endl;
	std::cout << "Number of choke points: " << overseerMapImpl->getGraph().getChokePoints().size() << std::endl;
}

size_t MapManager::GetRegionIdFromPoint(sc2::Point2D pt)
{
	const Region* region = overseerMapImpl->getNearestRegion(pt);
	return region->getId();
}

//Convert to array of points to avoid Overseer lockin
std::vector<sc2::Point2D> MapManager::GetRegionChokePoints(size_t regionId)
{
	std::vector<sc2::Point2D> allChokePoints;

	std::vector<ChokePoint> chokes = overseerMapImpl->getGraph().getChokePoints();
	for (ChokePoint choke : chokes) {
		const std::pair<const Region *, const Region *> regions = choke.getRegions();
		if (regions.first->getId() == regionId || regions.second->getId() == regionId) {
			//this choke is a way in/out of the current region
			Point2D pt = choke.getMiddlePoint();
			allChokePoints.push_back(pt);
		}
	}

	return allChokePoints;
}

sc2::Point2D MapManager::GetRegionChokeClosestToMapCenter(size_t regionId)
{
	//Map center is just half the height/width
	const GameInfo& info = bot.Observation()->GetGameInfo();
	Point2D mapCenter(info.width / 2.0f, info.height / 2.0f);

	Point2D closestPointToCenter(0, 0);
	float_t minDistance = numeric_limits<float_t>::max();

	vector<ChokePoint> chokes = overseerMapImpl->getGraph().getChokePoints();
	for (ChokePoint choke : chokes) {
		const pair<const Region *, const Region *> regions = choke.getRegions();
		if (regions.first->getId() == regionId || regions.second->getId() == regionId) {
			//this choke is a way in/out of the current region
			Point2D pt = choke.getMiddlePoint();

			//It's possible (and likely:  See Abyssal Reef) that the center point isn't pathable.  In that case, 
			//	the pathing search returns 0 distance.  So we'll have to use raw distance to calculate.
			float_t distance = Distance2D(pt, mapCenter);
			if (distance < minDistance) {
				minDistance = distance;
				closestPointToCenter = pt;
			}
		}
	}

	return closestPointToCenter;
}

bool MapManager::IsPointWalkable(Point2D ptToTest)
{
	/*overseerMapImpl->getGraph()
	shared_ptr<Tile> tile = overseerMapImpl->GetTile(ptToTest);
	tile->iswa*/
		return false;
}

//Find the ground z position for any 2d tile.  Great way to turn a 2d into a 3d point
float_t MapManager::GetGroundHeightAtPoint(sc2::Point2D pt)
{
	shared_ptr<Tile> tile = overseerMapImpl->GetTile(pt);
	return static_cast<float_t>(tile->GroundHeight());
}

/*
//TODO:  Not sure we want to expose Overseer types publicly, locks us in
std::vector<TilePosition> MapManager::GetRegionTiles(size_t regionId)
{
	Region* region = overseerMapImpl->getRegion(regionId);
	return region->getTilePositions();
}

std::vector<RegionEdge> MapManager::GetRegionEdges(size_t regionId)
{
	Region* region = overseerMapImpl->getRegion(regionId);
	return region->getEdges();
}
*/