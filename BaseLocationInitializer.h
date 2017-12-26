#pragma once

#include <sc2api/sc2_api.h>
#include "bot.h"
#include "BaseLocation.h"
#include "MineralLine.h"

//One time initialization of all bases on the map
class BaseLocationInitializer
{
public:
	BaseLocationInitializer(Bot & b, std::vector<BaseLocation> *locs);
	//Call to initialize base locations at start of game.  This should only ever be initialized once.
	//	Will use the given Bot to evaluate the known map units and fill the base location array
	//	with the results of its search.
	void InitializeBaseLocations();

private:
	Bot& bot;
	std::vector<BaseLocation> *baseLocations;

	void FindAllMineralsAndGeysers(std::vector<const sc2::Unit*> *mineralPatches, std::vector<const sc2::Unit*> *geysers);
	std::vector<sc2::Point2D> GetBuildableStartingPoints(sc2::Point3D startingPoint);
	sc2::Point2D FindClosestPointTo(sc2::Point2D startingPoint, std::vector<sc2::Point2D> pointList);
	BaseLocation BaseLocationInitializer::SetupNewBaseLocation(MineralLine mineralLine);
	void AddGeysersToBases(std::vector<const sc2::Unit*>geysers);
	std::vector<MineralLine> FindMineralLines(std::vector<const sc2::Unit*> nodes);
	uint32_t UseNextBaseLocationId();
	uint32_t nextBaseLocationId;
};
