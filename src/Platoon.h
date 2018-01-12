#pragma once
#include <sc2api/sc2_api.h>
#include "Squad.h"
#include "PlatoonOrders.h"
class Bot;


//Presently hardcoding the size parameters.
//	One platoon is up to three squads
//	Each squad is 5 marines, 3 marauders, 1 ghost, and 1 medivac (10 units, 15 supply)
//Aim to keep this logic entirely inside platoon for ease of changing it later.


//Orders
//	Orders work as follows:
//	None/Gather:  Units will gather as they are added around each squad.  The squads will attempt to gather near each other.
//	Defend:  Same.
//	Attack:  Once an attack command has been initiated, the platoon will be closed.  No more units should be added.  Note:  This is not enforced, only suggested.
//
//Pending Orders
//	When orders are given, they are put in a pending queue.  If the order is changed to attack, the platoon will be closed.  When the previous order is to gather, orders
//	will be held until gathering finishes.
//TODO:  Should defend do this?
//TODO:  Probably want an override like "oh crap go now"

class Platoon : public std::enable_shared_from_this<Platoon>
{
//TODO.  Move elsewhere?  Class'ify it?  Something.
public:
	//TODO: MOVE
	enum PLATOON_ORDERS {
		GATHER = 0,
		ATTACK,
		DEFEND,
	};

public:
	Platoon(Bot & b);
	size_t GetTotalPlatoonUnitCount();
	bool AddUnit(const sc2::Unit* unit);
	void SetOrders(PlatoonOrders orders);
	void OnSquadOrdersAchieved();
	void OnStep();
	sc2::Point3D GetCurrentPosition();

	std::string GetDebugSummaryString();

private:
	Bot & bot;
	std::vector<std::shared_ptr<Squad>> squads;
	size_t maxSquadCount;

	//Orders to be loaded once we're ready
	PlatoonOrders pendingOrders;
	//Current orders the platoon is undertaking
	PlatoonOrders currentOrders;
	bool checkForSquadOrdersAchieved;	//TEMP

	void ProcessPendingOrders();
	void DrawCurrentOrders();
	size_t GetMaxOfTypeInSquad(sc2::UNIT_TYPEID unitTypeID);
};
