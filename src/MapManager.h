#pragma once
#include <sc2api\sc2_api.h>
#include "ManagerBase.h"
class Bot;
class MapImpl;		//Never use the header in this file, it will "leak" Overseer data to the rest of the bot.

//TODO:  Add functionality as needed here, it's just a basic shell in place.  In fact, is it possible to get the header out of here?  Then we can never accidentally expose
//	this data.

//For now, map management is a layer over the Overseer project (https://github.com/pimmen89/Overseer).  We'll graft our own layer over this
//	in case we want to pull back from it in the future.
class MapManager : public ManagerBase
{
public:
	MapManager(Bot & b);
	~MapManager();
	virtual void OnStep();
	virtual void OnGameStart();

	size_t GetRegionIdFromPoint(sc2::Point2D pt);
	std::vector<sc2::Point2D> GetRegionChokePoints(size_t regionId);
	/*
	std::vector<TilePosition> GetRegionTiles(size_t regionId);
	std::vector<RegionEdge> GetRegionEdges(size_t regionId);
	*/
private:
	MapImpl* overseerMapImpl;
};
