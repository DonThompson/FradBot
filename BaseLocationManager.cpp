#include "BaseLocationManager.h"
#include "bot.h"

BaseLocationManager::BaseLocationManager(Bot & b)
	: ManagerBase(b)
{
}

void BaseLocationManager::OnGameStart()
{
	std::vector<const Unit*> minPatches;

	Units units = Observation()->GetUnits(Unit::Alliance::Neutral, {});
	for (const Unit* u : units) {
		if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
			minPatches.push_back(u);
		}
	}

	std::cout << minPatches.size() << " mineral patches found" << std::endl;

	//TODO:  This only seems to find exactly 4 mineral patches per base.  Good enough for what we want... but why not all of them?
	

	//We now have all the mineral patches.  We want to sort them into base locations.

	//Assume a radius of 5.0f around each - no base is closer than that.
	float_t maxMinsRadius = 5.0f;
	for (const Unit* patch : minPatches) {
		//std::cout << "Patch: " << m->pos.x << "," << m->pos.y << std::endl;

		//Search all existing base locations to see if it's in any by radius
		bool found = false;
		for (BaseLocation &l : baseLocations) {
			if (l.IsUnitInBase(patch)) {
				found = true;
				//Already know this base, so add a mineral patch to it.
				l.AddMineralPatch(patch);
			}
		}

		if (!found) {
			//New base!  Add it to our known list
			BaseLocation locNew(patch);
			baseLocations.push_back(locNew);
		}
	}

	//done!
	std::cout << "Base location manager initialized... found " << baseLocations.size() << " bases." << std::endl;
		
}

void BaseLocationManager::OnStep()
{
	for (BaseLocation loc : baseLocations) {
		loc.DrawSelf(bot);
	}
}
