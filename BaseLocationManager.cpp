#include "BaseLocationManager.h"
#include "bot.h"
#include "BaseLocationInitializer.h"
#include <limits>
using namespace sc2;

BaseLocationManager::BaseLocationManager(Bot & b)
	: ManagerBase(b)
{
}

void BaseLocationManager::OnGameStart()
{
	//Find all the base locations and fill our baseLocations array
	BaseLocationInitializer baseInitializer(bot, &baseLocations);
	baseInitializer.InitializeBaseLocations();

	InitializeKnownEnemyBase();

	InitializeNaturalExpansions();

	//done!
	std::cout << "Base location manager initialized... found " << baseLocations.size() << " bases." << std::endl;
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
	for (BaseLocation &startBase : baseLocations) {
		if (startBase.IsStartingPosition()) {
			//Find nearest expansion - that's our natural
			uint32_t closestBaseId = 0;
			float_t closestDistance = std::numeric_limits<float_t>::max();
			for (BaseLocation &destBase : baseLocations) {
				//Another start can't be a natural
				if (destBase.IsStartingPosition())
					continue;
				float_t dist = DistanceSquared3D(startBase.GetResourceDepotLocation(), destBase.GetResourceDepotLocation());
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


	
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation& loc : baseLocations) {
		loc.DrawSelf(bot);
	}
}

//TODO:  not sure i need this at all.
BaseLocation* BaseLocationManager::GetLocationById(uint32_t baseId)
{
	for (BaseLocation& loc : baseLocations) {
		if (loc.GetBaseLocationId() == baseId)
			return &loc;
	}

	return nullptr;
}

BaseLocation* BaseLocationManager::GetLocationByPosition(Point2D pt)
{
	for (BaseLocation& loc : baseLocations) {
		if (loc.IsPointInBase(pt)) {
			return &loc;
		}
	}

	return nullptr;
}
