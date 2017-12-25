#include "BaseLocationManager.h"
#include "bot.h"
#include "BaseLocationInitializer.h"

BaseLocationManager::BaseLocationManager(Bot & b)
	: ManagerBase(b)
{
}

void BaseLocationManager::OnGameStart()
{
	//Find all the base locations and fill our baseLocations array
	BaseLocationInitializer baseInitializer(bot, &baseLocations);
	baseInitializer.InitializeBaseLocations();

	InitializeKnownBases();

	//done!
	std::cout << "Base location manager initialized... found " << baseLocations.size() << " bases." << std::endl;
}

void BaseLocationManager::InitializeKnownBases()
{
	//1)  Find our command center and assign it to our base.
	Structure cc = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_COMMANDCENTER).front();
	//Figure out which base it's in
	BaseLocation& loc = GetLocationByPosition(cc.building->pos);
	if (loc != Empty)
		loc.SetStartingBase(cc.building);

	//2)  If the map is 1v1, then we know where the enemy is.  If it's not 1v1, we'll have to leave it up to a scout.
	std::vector<Point2D> startLocations = bot.Observation()->GetGameInfo().enemy_start_locations;
	if (startLocations.size() == 1) {
		//Enemy must be here
		BaseLocation& loc = GetLocationByPosition(startLocations[0]);
		if (loc != Empty) {
			loc.SetEnemyBase();
		}
	}
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation& loc : baseLocations) {
		loc.DrawSelf(bot);
	}
}

BaseLocation& BaseLocationManager::GetLocationById(uint32_t baseId)
{
	for (BaseLocation& loc : baseLocations) {
		if (loc.GetBaseLocationId() == baseId)
			return loc;
	}

	return Empty;
}

BaseLocation& BaseLocationManager::GetLocationByPosition(Point2D pt)
{
	for (BaseLocation& loc : baseLocations) {
		if (loc.IsPointInBase(pt)) {
			return loc;
		}
	}

	return Empty;
}
