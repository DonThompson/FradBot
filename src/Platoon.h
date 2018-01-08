#pragma once
#include <sc2api/sc2_api.h>
#include "Squad.h"


//Presently hardcoding the size parameters.
//	One platoon is up to three squads
//	Each squad is 5 marines, 3 marauders, 1 ghost, and 1 medivac (10 units, 15 supply)
//Aim to keep this logic entirely inside platoon for ease of changing it later.


class Platoon
{
public:
	Platoon();

	bool AddUnit(const sc2::Unit* unit);

	std::string GetDebugSummaryString();

private:
	std::vector<Squad> squads;
	size_t maxSquadCount;

	size_t GetMaxOfTypeInSquad(sc2::UNIT_TYPEID unitTypeID);
};
