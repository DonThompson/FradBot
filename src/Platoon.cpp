#include "Platoon.h"
#include <sstream>
using namespace sc2;

//Hardcoded limits to work with for now
size_t maxSquadMarines = 5;
size_t maxSquadMarauders = 3;
size_t maxSquadGhosts = 1;
size_t maxSquadMedivacs = 1;
size_t maxSquadsPerPlatoon = 3;

Platoon::Platoon()
	: maxSquadCount(maxSquadsPerPlatoon)
	, currentOrders(PLATOON_ORDERS::GATHER)
{
}

size_t Platoon::GetTotalPlatoonUnitCount()
{
	size_t size = 0;
	for (Squad & squad : squads) {
		size += squad.GetTotalSquadUnitCount();
	}
	return size;
}

//Return true if the unit was accepted.  False if there is no room.
bool Platoon::AddUnit(const sc2::Unit* unit)
{
	//Ask each squad how many of this unit type they have.  First squad with an opening takes it
	size_t max = GetMaxOfTypeInSquad(unit->unit_type);

	for (Squad & squad : squads) {
		size_t size = squad.CountUnitsByType(unit->unit_type);
		if (size < max) {
			squad.AddUnit(unit);
			return true;
		}
		else {
			//No more room, try the next squad
		}
	}

	//No room available in our current squads.  Do we have room to add another squad in this platoon?
	if (squads.size() < maxSquadCount) {
		//Make a new squad
		Squad newSquad(this);
		newSquad.AddUnit(unit);
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
	for (Squad & squad : squads) {
		oss << squad.GetDebugSummaryString() << std::endl;
	}

	return oss.str();
}

void Platoon::SetOrders(PLATOON_ORDERS orders, Point2D targetPoint)
{
	currentOrders = orders;
	currentTargetPoint = targetPoint;
}

//Called each game step
void Platoon::OnStep()
{
	for (Squad & squad : squads) {
		squad.OnStep();
	}
}
