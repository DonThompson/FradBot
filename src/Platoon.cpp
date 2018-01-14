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
	, platoonClosedToNewMembers(false)
	, checkForSquadOrdersAchieved(false)
{
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
	//No new members!
	if (platoonClosedToNewMembers)
		return false;

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
		//Tell the new squad to go to the center point of the platoon.  If we're properly closing platoons when they move
		//	out, this should work fine.  If a platoon gets a unit while it's clear across the map... this unit is going to 
		//	hare off on its own.
		//TODO:  Not sure that's right?
		newSquad->SetOrders(SquadOrders(GetCurrentPosition()));
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

		//TODO:  Can we skip this if we're closed?
		//if(!platoonClosedToNewMembers) {

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

			//Done with our initial platoon gathering and we have a real order.  Close down the group
			//	so we don't pick up any stragglers.
			//TODO:  This really needs to be 'if we're leaving the base' sort of thing.  Best I've got is "attack" for now.
			if(currentOrders.orderType == PlatoonOrders::ORDER_TYPE::ATTACK)
				platoonClosedToNewMembers = true;
		}
		else {
			//std::cout << "Platoon {name} still gathering - orders on hold" << std::endl;
		}
	}
}

//Should be called each game step.  Should not be throttled.
void Platoon::DrawCurrentOrders()
{
	if (currentOrders.hasOrders) {
		//TODO:  3d pt.  completely guess at z order, may work on some maps and not others
		//TODO:  This should work?
		//float_t zPos = bot.Map().GetGroundHeightAtPoint(currentOrders.targetPoint);
		float_t zPos = 12.0f;
		Point3D pt3d(currentOrders.targetPoint.x, currentOrders.targetPoint.y, zPos);
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
			//TODO:  Maybe use pathing here?  I want to leave it on raw distance for now because this code is saving us when units are sent to unpathable points.
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
		//TODO:  Picked value at random
		if (bot.Query()->PathingDistance(squad->GetFirstRawUnit(), partialPoint) < 6.0f) {
			//We're pretty close to our target point, just jump right there, no more halfsies.
			partialPoint = currentOrders.targetPoint;
		}

		//Make sure the point is actually reachable
		//TODO:  Real pathing needed!
		if (!bot.Observation()->IsPathable(partialPoint)) {
			//TODO:  what do we do?  For now let's just jump to the end.  We really need some pathing.
			//bot.Query()->PathingDistance()
			partialPoint = currentOrders.targetPoint;
		}

		//TODO:  this doesn't seem to work
		//draw stuff
		Point3D interimPt3d(partialPoint.x, partialPoint.y, currentPos.z);
		bot.Draw().DrawLine(currentPos, interimPt3d);

		//execute orders.  TODO:  who should issue attack command?
		SquadOrders orders(partialPoint);
		squad->SetOrders(orders);
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

sc2::Point3D Platoon::GetCurrentPosition()
{
	//squad 1 leads the way
	if (squads.size() > 0) {
		return squads.front()->GetCurrentPosition();
	}

	//TODO:  Not sure this is a good idea either
	return Point3D(0, 0, 0);

}

bool Platoon::HasOrders()
{
	//TODO:  I think we could be in a state where we got pending orders, but are still gathering, 
	//	thus haven't moved them over -- but have no current orders.
	return currentOrders.hasOrders || pendingOrders.hasOrders;
}
