#include "BaseLocationManager.h"
#include "bot.h"
#include "BaseLocationInitializer.h"
#include <limits>
using namespace sc2;

BaseLocationManager::BaseLocationManager(Bot & b)
	: ManagerBase(b)
	, mainBase(nullptr)
	, naturalBase(nullptr)
{
}

void BaseLocationManager::OnGameStart()
{
	//Find all the base locations and fill our baseLocations array
	BaseLocationInitializer baseInitializer(bot, &mapBaseLocations);
	baseInitializer.InitializeBaseLocations();

	InitializeKnownEnemyBase();
	InitializeNaturalExpansions();

	//Set some internal flags for quick access to main and natural
	for (BaseLocation &l : mapBaseLocations) {
		if (l.IsMyBase()) {
			mainBase = &l;
			naturalBase = GetLocationById(l.GetNaturalExpansionId());
			break;
		}
	}

	//done!
	std::cout << "Base location manager initialized... found " << mapBaseLocations.size() << " bases." << std::endl;
}

void BaseLocationManager::InitializeKnownEnemyBase()
{
	std::vector<Point2D> startLocations = bot.Observation()->GetGameInfo().enemy_start_locations;
	if (startLocations.size() == 1) {
		//Enemy must be here
		BaseLocation* loc = GetLocationByPosition(startLocations[0]);
		if (loc != nullptr) {
			loc->SetEnemyBase();
		}
	}

	//In all cases, flag all the possible start locations
	for (Point2D startingPt : startLocations) {
		BaseLocation* loc = GetLocationByPosition(startingPt);
		if (loc != nullptr) {
			loc->SetIsStartingPosition();
		}
	}
}

void BaseLocationManager::InitializeNaturalExpansions()
{
	//Find all the start locations...
	for (BaseLocation &startBase : mapBaseLocations) {
		//Naturals are defined as attached to a start position.  Skip if we're not a start.
		if (!startBase.IsStartingPosition())
			continue;

		//TODO:  Might drop this in favor of pathing distance
		//Every start should have a choke.  We want to find the base closest to that point as the natural.  In some cases
		//	(Abyssal Reef), the 3rd base is actually straight-line closer to the main.
		//TODO:  Some maps might have 2 chokes?  Not sure.

		//Find nearest expansion - that's the natural
		uint32_t closestBaseId = 0;
		float_t closestDistance = std::numeric_limits<float_t>::max();
		for (BaseLocation &destBase : mapBaseLocations) {
			//Another start can't be a natural
			if (destBase.IsStartingPosition())
				continue;

			float_t dist = bot.Query()->PathingDistance(startBase.GetResourceDepotLocation(), destBase.GetResourceDepotLocation());
			//float_t dist = DistanceSquared3D(startBase.GetResourceDepotLocation(), destBase.GetResourceDepotLocation());
			if (dist < closestDistance) {
				closestDistance = dist;
				closestBaseId = destBase.GetBaseLocationId();
			}
		}

		//Map the start base to its natural - the closest base we found
		startBase.SetNaturalExpansionId(closestBaseId);
		//Then map the natural back to its start base
		BaseLocation* natural = GetLocationById(closestBaseId);
		if(natural != nullptr)
			natural->SetParentOfNaturalId(startBase.GetBaseLocationId());
	}
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation& loc : mapBaseLocations) {
		loc.DrawSelf(bot);
	}
}

BaseLocation* BaseLocationManager::GetLocationById(uint32_t baseId)
{
	for (BaseLocation& loc : mapBaseLocations) {
		if (loc.GetBaseLocationId() == baseId)
			return &loc;
	}

	return nullptr;
}

BaseLocation* BaseLocationManager::GetLocationByPosition(Point2D pt)
{
	for (BaseLocation& loc : mapBaseLocations) {
		if (loc.IsPointInBase(pt)) {
			return &loc;
		}
	}

	return nullptr;
}

BaseLocation* BaseLocationManager::Main()
{
	return mainBase;
}

BaseLocation* BaseLocationManager::Natural()
{
	return naturalBase;
}

//TODO:  Not yet implemented, will never have results.
std::vector<BaseLocation*> BaseLocationManager::OtherBases()
{
	return otherBases;
}

std::vector<BaseLocation*> BaseLocationManager::GetNeutralBases()
{
	std::vector<BaseLocation*> neutralLocations;
	for (BaseLocation& loc : mapBaseLocations) {
		if (!loc.IsMyBase() && !loc.IsEnemyBase()) {
			neutralLocations.push_back(&loc);
		}
	}
	return neutralLocations;
}


BaseLocation* BaseLocationManager::FindNearestAvailableExpansionLocation()
{
	//first try to expand to the natural
	BaseLocation* natural = Natural();
	if (natural != nullptr && !natural->IsMyBase()) {
		return natural;
	}
	//Find closest to main
	Point2D mainPt = Main()->GetResourceDepotLocation();
	BaseLocation* winner = nullptr;
	float_t distanceFromMain = std::numeric_limits<float_t>::max();
	for (BaseLocation* loc : GetNeutralBases()) {
		float_t dist = Distance2D(loc->GetResourceDepotLocation(), mainPt);
		if (dist < distanceFromMain) {
			//new winner
			winner = loc;
			distanceFromMain = dist;
		}
	}
	return winner;
}

void BaseLocationManager::ClaimBaseByPosition(Point2D resourceDepotLocation, Structure & resourceDepot)
{
	BaseLocation* loc = GetLocationByPosition(resourceDepotLocation);
	if (loc) {
		loc->ClaimBaseForPlayer();
		loc->SetResourceDepot(resourceDepot);
	}
}
