#include "BaseLocation.h"
#include "bot.h"

BaseLocation::BaseLocation(const Unit* startingMineralPatch)
{
	starterPointMinPatch = startingMineralPatch->pos;
	AddMineralPatch(startingMineralPatch);
}

bool BaseLocation::IsUnitInBase(const Unit* unit)
{
	//TODO:  For now, each base is 15.0f units in radius.  This isn't perfect.
	if (Distance3D(starterPointMinPatch, unit->pos) < baseRadius)
		return true;
	return false;
}

void BaseLocation::AddMineralPatch(const Unit* patch)
{
	mineralPatches.push_back(patch);
}

void BaseLocation::AddGeyser(const Unit* geyser)
{
	geysers.push_back(geyser);
}

void BaseLocation::DrawSelf(Bot & bot)
{
	//TODO:  us, green, enemy, red, neut, gray
	Color color = Colors::Gray;

	//First, simply draw a circle around us
	bot.Draw().DrawCircle(starterPointMinPatch, baseRadius, color);

	//and highlight our center point by drawing 5 concentric rings
	bot.Draw().DrawCircle(starterPointMinPatch, 0.1f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.2f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.3f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.4f, color);
	bot.Draw().DrawCircle(starterPointMinPatch, 0.5f, color);

	//Now highlight the mineral patches
	for (const Unit* patch : mineralPatches) {
		bot.Draw().DrawCircle(patch->pos, patch->radius, color);
	}

	//and geysers
	for (const Unit* geyser : geysers) {
		bot.Draw().DrawCircle(geyser->pos, geyser->radius, color);
	}
}
