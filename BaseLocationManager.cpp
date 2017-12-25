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

	//done!
	std::cout << "Base location manager initialized... found " << baseLocations.size() << " bases." << std::endl;
		
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation loc : baseLocations) {
		loc.DrawSelf(bot);
	}
}

BaseLocation BaseLocationManager::GetLocationById(uint32_t baseId)
{
	for (BaseLocation loc : baseLocations) {
		if (loc.GetBaseLocationId() == baseId)
			return loc;
	}

	//TODO:  Don't like this
	return BaseLocation(0, nullptr);
}
