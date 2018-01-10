#pragma once
#include <sc2api/sc2_api.h>
#include "Squad.h"
class Bot;

//Presently hardcoding the size parameters.
//	One platoon is up to three squads
//	Each squad is 5 marines, 3 marauders, 1 ghost, and 1 medivac (10 units, 15 supply)
//Aim to keep this logic entirely inside platoon for ease of changing it later.


class Platoon
{
//TODO.  Move elsewhere?  Class'ify it?  Something.
public:
	enum PLATOON_ORDERS {
		GATHER = 0,
		ATTACK,
		DEFEND,
	};

public:
	Platoon(Bot & b);
	size_t GetTotalPlatoonUnitCount();
	bool AddUnit(const sc2::Unit* unit);
	void SetOrders(PLATOON_ORDERS orders, sc2::Point2D targetPoint);
	void OnSquadOrdersAchieved();
	void OnStep();

	std::string GetDebugSummaryString();

private:
	Bot & bot;
	std::vector<Squad> squads;
	size_t maxSquadCount;

	PLATOON_ORDERS currentOrders;
	//order point
	sc2::Point2D currentTargetPoint;
	bool hasOrders;
	bool checkForSquadOrdersAchieved;	//TEMP

	size_t GetMaxOfTypeInSquad(sc2::UNIT_TYPEID unitTypeID);
};
