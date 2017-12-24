#include "BaseLocationManager.h"
#include "bot.h"

BaseLocationManager::BaseLocationManager(Bot & b)
	: ManagerBase(b)
{
}

void BaseLocationManager::OnGameStart()
{
	//Find all the base locations and fill our baseLocations array
	InitializeBaseLocations();

	

	//done!
	std::cout << "Base location manager initialized... found " << baseLocations.size() << " bases." << std::endl;
		
}

//Initialization routine - one time only.
//	Finds all base locations on the map by analyzing the resources available.
//	At completion, will have filled the baseLocations array with each found.
void BaseLocationManager::InitializeBaseLocations()
{
	std::vector<const Unit*> mineralPatches;
	std::vector<const Unit*> geysers;
	FindAllMineralsAndGeysers(&mineralPatches, &geysers);

	//We now have all the resources.  We want to sort them into base locations by distance from each other.  It's important that we
	//	do this by mineral patches first, not by geysers - they being on the ends is often too far away to detect without risk.
	CreateBaseLocationsFromResourceNodes(mineralPatches);
	CreateBaseLocationsFromResourceNodes(geysers);

	//Now let each base location handle any consolidation
	for (BaseLocation &l : baseLocations) {
		l.Initialize();
	}
}

void BaseLocationManager::FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers)
{
	//TODO:  This only seems to find exactly 4 mineral patches per base.  Good enough for what we want... but why not all of them?
	Units units = Observation()->GetUnits(Unit::Alliance::Neutral, {});
	for (const Unit* u : units) {
		if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD)
			mineralPatches->push_back(u);
		else if (u->unit_type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER)
			geysers->push_back(u);
	}
}

void BaseLocationManager::CreateBaseLocationsFromResourceNodes(std::vector<const Unit*> nodes)
{
	for (const Unit* node : nodes) {
		//Search all existing base locations to see if it's in any by radius
		bool found = false;
		for (BaseLocation &l : baseLocations) {
			if (l.IsUnitInBase(node)) {
				found = true;
				//Already know this base, so add the node to it.
				if (node->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD)
					l.AddMineralPatch(node);
				else
					l.AddGeyser(node);
			}
		}

		if (!found) {
			//New base!  Add it to our known list
			BaseLocation locNew(node);
			baseLocations.push_back(locNew);
		}
	}
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation loc : baseLocations) {
		loc.DrawSelf(bot);
	}
}
