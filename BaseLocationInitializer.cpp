#include "BaseLocationInitializer.h"

BaseLocationInitializer::BaseLocationInitializer(Bot & b, std::vector<BaseLocation> *locs)
	: bot(b)
	, baseLocations(locs)
	, nextBaseLocationId(1)
{
}

//Initialization routine - one time only.
//	Finds all base locations on the map by analyzing the resources available.
//	At completion, will have filled the baseLocations array with each found.
void BaseLocationInitializer::InitializeBaseLocations()
{
	std::vector<const Unit*> mineralPatches;
	std::vector<const Unit*> geysers;
	FindAllMineralsAndGeysers(&mineralPatches, &geysers);

	//We now have all the resources.  We want to sort them into base locations by distance from each other.  It's important that we
	//	do this by mineral patches first, not by geysers - they being on the ends is often too far away to detect without risk.
	CreateBaseLocationsFromResourceNodes(mineralPatches);
	CreateBaseLocationsFromResourceNodes(geysers);

	//Determine the center of the map
	Point2D centerOfMap(static_cast<float_t>(bot.Observation()->GetGameInfo().width) / 2.0f, static_cast<float_t>(bot.Observation()->GetGameInfo().height) / 2.0f);

	//Now let each base location handle any consolidation
	for (BaseLocation &l : (*baseLocations)) {
		l.Initialize(bot, centerOfMap);
	}
}

//Searches all neutral units to find mineral patches and vespene geysers.  Fills the 2 provided arrays with the resulting units
void BaseLocationInitializer::FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers)
{
	Units units = bot.Observation()->GetUnits(Unit::Alliance::Neutral, {});
	for (const Unit* u : units) {
		if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD)
			mineralPatches->push_back(u);
		else if (u->unit_type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER)
			geysers->push_back(u);
	}
}

//Evaluates all the given resource nodes.  If its within a known base, adds the node to that base.  Otherwise, 
//	creates a new base location at the given position.
void BaseLocationInitializer::CreateBaseLocationsFromResourceNodes(std::vector<const Unit*> nodes)
{
	for (const Unit* node : nodes) {
		//Search all existing base locations to see if it's in any by radius
		bool found = false;
		for (BaseLocation &l : (*baseLocations)) {
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
			BaseLocation locNew(UseNextBaseLocationId(), node);
			baseLocations->push_back(locNew);
		}
	}
}

uint32_t BaseLocationInitializer::UseNextBaseLocationId()
{
	uint32_t useThis = nextBaseLocationId;
	nextBaseLocationId++;
	return useThis;
}
