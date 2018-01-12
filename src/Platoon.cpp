#include "Platoon.h"
#include "bot.h"
#include <sstream>
using namespace sc2;
using namespace std;

//Hardcoded limits to work with for now
size_t maxSquadMarines = 5;
size_t maxSquadMarauders = 3;
size_t maxSquadGhosts = 1;
size_t maxSquadMedivacs = 1;
size_t maxSquadsPerPlatoon = 3;

Platoon::Platoon(Bot & b)
	: maxSquadCount(maxSquadsPerPlatoon)
	, bot(b)
{
	checkForSquadOrdersAchieved = false;
}

size_t Platoon::GetTotalPlatoonUnitCount()
{
	size_t size = 0;
	for (shared_ptr<Squad> squad : squads) {
		size += squad->GetTotalSquadUnitCount();
	}
	return size;
}

//Return true if the unit was accepted.  False if there is no room.
bool Platoon::AddUnit(const sc2::Unit* unit)
{
	//Ask each squad how many of this unit type they have.  First squad with an opening takes it
	size_t max = GetMaxOfTypeInSquad(unit->unit_type);

	for (shared_ptr<Squad> squad : squads) {
		size_t size = squad->CountUnitsByType(unit->unit_type);
		if (size < max) {
			squad->AddUnit(unit);
			return true;
		}
		else {
			//No more room, try the next squad
		}
	}

	//No room available in our current squads.  Do we have room to add another squad in this platoon?
	if (squads.size() < maxSquadCount) {
		//Make a new squad
		shared_ptr<Squad> newSquad = make_shared<Squad>(bot, shared_from_this());
		newSquad->AddUnit(unit);
		squads.push_back(newSquad);
		return true;
	}
	
	//No room for any more squads, sorry buddy
	return false;
}

size_t Platoon::GetMaxOfTypeInSquad(sc2::UNIT_TYPEID unitTypeID)
{
	switch (unitTypeID) {
	case UNIT_TYPEID::TERRAN_MARINE:
		return maxSquadMarines;
	case UNIT_TYPEID::TERRAN_MARAUDER:
		return maxSquadMarauders;
	case UNIT_TYPEID::TERRAN_MEDIVAC:
		return maxSquadMedivacs;
	case UNIT_TYPEID::TERRAN_GHOST:
		return maxSquadGhosts;

	case UNIT_TYPEID::TERRAN_BANSHEE:
	case UNIT_TYPEID::TERRAN_BATTLECRUISER:
	case UNIT_TYPEID::TERRAN_CYCLONE:
	case UNIT_TYPEID::TERRAN_HELLION:
	case UNIT_TYPEID::TERRAN_HELLIONTANK:
	case UNIT_TYPEID::TERRAN_LIBERATOR:
	case UNIT_TYPEID::TERRAN_LIBERATORAG:
	case UNIT_TYPEID::TERRAN_RAVEN:
	case UNIT_TYPEID::TERRAN_REAPER:
	case UNIT_TYPEID::TERRAN_SIEGETANK:
	case UNIT_TYPEID::TERRAN_SIEGETANKSIEGED:
	case UNIT_TYPEID::TERRAN_THOR:
	case UNIT_TYPEID::TERRAN_THORAP:
	case UNIT_TYPEID::TERRAN_VIKINGASSAULT:
	case UNIT_TYPEID::TERRAN_VIKINGFIGHTER:
	case UNIT_TYPEID::TERRAN_WIDOWMINE:
	case UNIT_TYPEID::TERRAN_WIDOWMINEBURROWED:
		//TODO:  Future. Cram them all in the first squad
		break;
	}

	return 200;
}

std::string Platoon::GetDebugSummaryString()
{
	//TODO:  Name these
	std::string _name = "(TODO: Name)";

	std::ostringstream oss;
	oss << "* Platoon " << _name << std::endl;
	for (shared_ptr<Squad> squad : squads) {
		oss << squad->GetDebugSummaryString() << std::endl;
	}

	return oss.str();
}

void Platoon::SetOrders(PlatoonOrders orders)
{
	//Load the new orders as pending.  Each step we'll analyze if we're ready to switch orders.
	pendingOrders = orders;
}

//Should be called each game step.  Can be throttled.
void Platoon::ProcessPendingOrders()
{
	//Figure out if we have any new orders pending
	if (pendingOrders.hasOrders)
	{
		bool applyNewOrders = false;
		//Make sure every squad has finished gathering.
		bool isAnyStillGathering = false;
		for (shared_ptr<Squad> squad : squads) {
			if (!squad->HasGathered()) {
				isAnyStillGathering = true;
			}
		}

		if (!isAnyStillGathering) {
			//Everyone is gathered, go forward with our new orders.
			applyNewOrders = true;
		}

		//TODO:  close squad
		if (applyNewOrders) {
			currentOrders = pendingOrders;
			pendingOrders.Clear();
			//Clear all squad orders
			for (shared_ptr<Squad> squad : squads) {
				squad->ClearOrders();
			}
		}
		else {
			std::cout << "Platoon {name} still gathering - orders on hold" << std::endl;
		}
	}
}

//Should be called each game step.  Should not be throttled.
void Platoon::DrawCurrentOrders()
{
	if (currentOrders.hasOrders) {
		//TODO:  3d pt.  completely guess at z order, may work on some maps and not others
		Point3D pt3d(currentOrders.targetPoint.x, currentOrders.targetPoint.y, 12.0f);
		//Draw a circle at our target with a label
		bot.Draw().DrawCircle(pt3d, 1.0f, sc2::Colors::Purple);
		//TODO:  Platoon name
		bot.Draw().DrawTextOnMap("Platoon {name} Target!", pt3d);
	}
}

//Called each game step
void Platoon::OnStep()
{
	ProcessPendingOrders();
	DrawCurrentOrders();

	//TODO:  Should we throttle the speed here?

	//Ensure the platoon has orders.  If it doesn't, we can't pass to the squads.
	if (!currentOrders.hasOrders)
		return;

	//First we check if one of our squads achieved an objective.  If so, we might have achieved our entire objective for the platoon.

	//TODO:  hack because we can't iterate over squads in the callback because it's already iterating squads
	if (checkForSquadOrdersAchieved) {
		//One time check, don't do it again
		checkForSquadOrdersAchieved = false;
		//TODO:  functionify

		//See if all squads have reached our target point.

		//Within 2 tiles is fine.
		//TODO:  can we go higher?  Just how far is 2 tiles?  We may have a clumping of units.  More range gives us
		//	more flexibility.
		float winRange = 2.0f;

		//TODO:  maybe we keep another vector of squads "working on" the target.  if 2 finish and 3rd isn't yet, we'll keep re-tasking the first 2.
		//TODO:  we're already iterating through squads -- this callback is probably in OnStep()
		bool atLeastOneFailed = false;
		for (shared_ptr<Squad> squad : squads) {
			float currentDistance = sc2::Distance2D(squad->GetCurrentPosition(), currentOrders.targetPoint);
			if (currentDistance > winRange) {
				//At least squad is not there yet, so keep issuing orders
				atLeastOneFailed = true;
			}
		}

		if (!atLeastOneFailed) {
			//If we got here, all squads are within range.  We can clear our orders
			currentOrders.Clear();
			//Quit out - we don't need to iterate over the squads
			return;
		}
	}

	//TODO:  if no squads have orders, we've accomplished our orders.  need to flag that as not having orders anymore
	//	Maybe each squad should message back up the parent that they've finished?

	for (shared_ptr<Squad> squad : squads) {
		//If the squad already has orders, let them continue them
		if (squad->HasOrders())
			continue;

		//Only move a short distance toward that target.  We'll work our way their slowly.
		Point3D currentPos = squad->GetCurrentPosition();

		//50% of the way between points.
		//	TODO:  this will keep shrinking to crazy small values?
		Point2D partialPoint((currentPos.x + currentOrders.targetPoint.x) / 2, (currentPos.y + currentOrders.targetPoint.y) / 2);

		//TODO:  this doesn't seem to work
		//draw stuff
		Point3D interimPt3d(partialPoint.x, partialPoint.y, currentPos.z);
		bot.Draw().DrawLine(currentPos, interimPt3d);

		//execute orders.  TODO:  who should issue attack command?
		SquadOrders orders(partialPoint);
		squad->SetOrders(orders);
		
		/* order type kinda useless?
		if (currentOrders == PLATOON_ORDERS::ATTACK) {
			bot.Actions()->UnitCommand(squad, ABILITY_ID::ATTACK_ATTACK, currentTargetPoint);
		}
		else if (currentOrders == PLATOON_ORDERS::DEFEND) {
			bot.Actions()->UnitCommand(squad, ABILITY_ID::ATTACK_ATTACK, currentTargetPoint);
		}
		else if (currentOrders == PLATOON_ORDERS::GATHER) {
			bot.Actions()->UnitCommand(squad, ABILITY_ID::ATTACK_ATTACK, currentTargetPoint);
		}*/
	}


	//TODO:  Or throttle here?  Or inside squad?  Or above platoon?
	for (shared_ptr<Squad> squad : squads) {
		squad->OnStep();
	}
}

//Called when a squad achieves their orders
void Platoon::OnSquadOrdersAchieved()
{
	//TODO:  hack because we can't iterate over squads in the callback because it's already iterating squads
	//TODO:  still an issue with shared ptrs?  probably, infinite recursion.
	checkForSquadOrdersAchieved = true;
}
