#include "BaseLocationManager.h"
#include "bot.h"
#include "BaseLocationInitializer.h"
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

	AssignNaturalExpansions();

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

	}
}

void BaseLocationManager::InitializeNaturalExpansions()
{
	//bot.Observation()->GetGameInfo().enemy_start_locations
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
