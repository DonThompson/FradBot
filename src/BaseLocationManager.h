#pragma once

#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BaseLocation.h"

//TODO:  SharedPtr usage
class BaseLocationManager : public ManagerBase
{
public:
	explicit BaseLocationManager(Bot & b);
	
	virtual void OnStep();
	virtual void OnGameStart();

	BaseLocation* GetLocationById(uint32_t baseId);
	BaseLocation* GetLocationByPosition(sc2::Point2D pt);

	//Always returns a base once base initialization has happened.  Even if you lose your main base, this will always
	//	return a base location where you started.
	BaseLocation* Main();
	//Returns the location of your starting natural even if you have yet to expand there.
	BaseLocation* Natural();
	//Returns an array of all other bases you have established.  Will never include your main or natural.  Only
	//	contains values once a base has been established in a location.
	std::vector<BaseLocation*> OtherBases();
	//Returns the location closest to your main that is available for expansion.  WARNING:  This is updated with known 
	//	map information.  Your enemy may have expanded here since the last time you scouted it.
	BaseLocation* FindNearestAvailableExpansionLocation();
	//Claims ownership of a base for the player by position
	void ClaimBaseByPosition(sc2::Point2D resourceDepotLocation, Structure & resourceDepot);
	//Returns a list of all enemy bases
	std::vector<BaseLocation*> EnemyBases();
	//Returns a list of all possible bases that aren't mine
	std::vector<BaseLocation*> AllBasesNotMine();

private:
	void InitializeKnownEnemyBase();
	void InitializeNaturalExpansions();

	//Save reference for quick access
	BaseLocation* mainBase;
	BaseLocation* naturalBase;
	std::vector<BaseLocation*> otherBases;
	//Returns an array of all neutral bases.  WARNING:  This is updated with known map information.  Your enemy may have
	//	expanded here since the last time you scouted it.
	std::vector<BaseLocation*> GetNeutralBases();

	//Evaluation of the map - all possible base locations, whether yours, enemies, or unowned.
	std::vector<BaseLocation> mapBaseLocations;
};
