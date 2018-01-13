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