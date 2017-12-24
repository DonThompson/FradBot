#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "bot.h"
#include "BaseLocation.h"

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

	//Base location initialization
	void CreateBaseLocationsFromResourceNodes(std::vector<const Unit*> nodes);
	void FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers);

	uint32_t UseNextBaseLocationId();
	uint32_t nextBaseLocationId;
};
