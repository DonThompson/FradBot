#include "ArmyManager.h"
#include "bot.h"
#include <sstream>
using namespace sc2;

/*static */bool ArmyManager::IsMilitaryUnit(const sc2::Unit* unit)
{
	switch (static_cast<UNIT_TYPEID>(unit->unit_type)) {
	case UNIT_TYPEID::TERRAN_BANSHEE:
	case UNIT_TYPEID::TERRAN_BATTLECRUISER:
	case UNIT_TYPEID::TERRAN_CYCLONE:
	case UNIT_TYPEID::TERRAN_GHOST:
	case UNIT_TYPEID::TERRAN_HELLION:
	case UNIT_TYPEID::TERRAN_HELLIONTANK:
	case UNIT_TYPEID::TERRAN_LIBERATOR:
	case UNIT_TYPEID::TERRAN_LIBERATORAG:
	case UNIT_TYPEID::TERRAN_MARAUDER:
	case UNIT_TYPEID::TERRAN_MARINE:
	case UNIT_TYPEID::TERRAN_MEDIVAC:
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
		return true;
	}

	return false;
}

ArmyManager::ArmyManager(Bot & b)
	: ManagerBase(b)
	, lastBalanceClock(clock_t())
	, raxInProgress(0)
	, currentStrategy(0)
{
}

void ArmyManager::OnStep()
{
	//Rebalance supply every few seconds.  We really don't need to do this every step.
	//TODO:  Clock is REAL time.  Should we use game time?  In super fast AI mode this might actually be 5-6 game seconds.
	const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
	if (clock() - lastBalanceClock > rebalanceTime) {

		//Only if we're autonomous
		if (actAutonomously)
		{
			//Specific logic
			if (BarracksNeeded()) {
				BuildBarracks();
			}
		}

		//Do these things whether autonomous or not, under timer
		ManageMilitary();

		lastBalanceClock = clock();
	}

	//Do these things whether autonomous or not, w/o timer
	for (Platoon & platoon : armyPlatoons) {
		platoon.OnStep();
	}
}

void ArmyManager::OnUnitCreated(const sc2::Unit* unit)
{
	//Ignore anything non-military
	if (!ArmyManager::IsMilitaryUnit(unit))
		return;

	//Add it to a platoon
	AddUnitToPlatoon(unit);
}

void ArmyManager::AddUnitToPlatoon(const sc2::Unit* unit)
{
	//Iterate through all platoons and try to add the unit
	for (Platoon & platoon : armyPlatoons) {
		if (platoon.AddUnit(unit)) {
			//Met our goal
			return;
		}
	}

	//No room in that platoon, we'll have to create a new one.
	Platoon newPlatoon;
	newPlatoon.AddUnit(unit);
	armyPlatoons.push_back(newPlatoon);
}

void ArmyManager::OnUnitDestroyed(const sc2::Unit* unit)
{
	//TODO:  Not sure if we should remove the units or not when they die?
	//	Make sure we aren't making assumptions on army sizes that don't exist.
}

bool ArmyManager::BarracksNeeded()
{
	const ObservationInterface* observation = Observation();

	//Includes under construction and completed
	//TODO:  Better to not count in progress so we can use our own counter?
	int32_t countBarracks = bot.Structures().CountStructuresByType(UNIT_TYPEID::TERRAN_BARRACKS);
	int32_t currentSupply = observation->GetFoodUsed();
	int32_t currentMinerals = observation->GetMinerals();

	//Build our first barracks at 16 and build one every time we hit 400 minerals in the bank after that.  Max at 10.
	//Cap to no more than 1 per 10 food - we were getting hung up sometimes and building 9 rax at ~35 supply
	if (countBarracks == 0 && raxInProgress == 0 && currentSupply >= 16) {
		return true;
	}
	else {
		//TODO:  In progress but not yet started doesn't fit in here.
		int32_t maxDesiredRax = currentSupply / 10;
		if (countBarracks < maxDesiredRax && currentMinerals >= 300) {
			return true;
		}
	}

	return false;
}

void ArmyManager::BuildBarracks()
{
	uint64_t queueId = bot.Construction().BuildStructure(ABILITY_ID::BUILD_BARRACKS,
		std::bind(&ArmyManager::OnBarracksSuccess, this, std::placeholders::_1),
		std::bind(&ArmyManager::OnBarracksFailed, this, std::placeholders::_1));

	std::cout << "Starting new barracks, task id:  " << queueId << std::endl;

	raxInProgress++;
}

void ArmyManager::OnBarracksSuccess(int64_t taskId)
{
	raxInProgress--;

	std::cout << "Barracks build success, task:  " << taskId << std::endl;
}

void ArmyManager::OnBarracksFailed(int64_t taskId)
{
	raxInProgress--;

	std::cout << "Barracks build failed, task:  " << taskId << std::endl;
}

//TODO:  DELETE
void ArmyManager::TryAttackInGroups()
{
	const ObservationInterface* observation = Observation();

	Units idleArmy = Utils::GetIdleUnits(observation, UNIT_TYPEID::TERRAN_MARINE, UNIT_TYPEID::TERRAN_MARAUDER);
	if (idleArmy.size() > 15) {
		LaunchAttackGroup(idleArmy);
	}
}

//TODO:  DELETE
void ArmyManager::LaunchAttackGroup(Units unitsToAttack)
{
	const GameInfo& gameInfo = Observation()->GetGameInfo();
	//What is attack_attack vs attack?
	Actions()->UnitCommand(unitsToAttack, ABILITY_ID::ATTACK_ATTACK, gameInfo.enemy_start_locations.front());
}

void ArmyManager::OnBarracksIdle(const Unit* unit)
{
	//If we're not autonomous, don't build anything
	if (!actAutonomously)
		return;

	//0 = marine, 1 = marauder
	unsigned int thingToBuild = 0;

	//Got from examples:
	//  https://github.com/Blizzard/s2client-api/blob/master/examples/common/bot_examples.cc
	const Unit* raxAddOn = Observation()->GetUnit(unit->add_on_tag);
	if (raxAddOn == nullptr) {
		//Not upgraded, let's upgrade it 10% of the time.  Short circuit out if we do this.
		if (rand() % 10 == 0) {
			Actions()->UnitCommand(unit, ABILITY_ID::BUILD_TECHLAB);
			return;
		}

		//Only marines
		thingToBuild = 0;
	}
	else if (raxAddOn->unit_type == UNIT_TYPEID::TERRAN_BARRACKSREACTOR) {
		//Marines only
		thingToBuild = 0;
	}
	else if (raxAddOn->unit_type == UNIT_TYPEID::TERRAN_BARRACKSTECHLAB) {
		thingToBuild = rand() % 2;
	}

	switch (thingToBuild) {
	case 0:
		Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARINE);
		break;
	case 1:
		Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARAUDER);
		break;
	}
}


void ArmyManager::OnUnitIdle(const Unit* unit)
{
	switch (unit->unit_type.ToType()) {
	case UNIT_TYPEID::TERRAN_BARRACKS:      OnBarracksIdle(unit);       break;
	default:    break;
	}
}

bool ArmyManager::TrainUnit(sc2::ABILITY_ID abilityID)
{
	//TODO:  This could be way better:
	//	* Keep an ordered set of buildings where we should build based on game state (advanced)
	//	* Only build marines from non-lab/reactors, not techlabs.  same for hellions/etc
	//	* Use the tech_requirement to determine need
	//	* Know about reactors
	//	* lots of stuff for other building types too

	//What produces this unit?
	UnitData data = bot.Data().GetUnitData(abilityID);
	UNIT_TYPEID producer = data.producingBuilding;

	//////////////////////////////////////////////////////////
	//V1:  Jump through all producing buildings that match	//
	//////////////////////////////////////////////////////////

	//Iterate through all the producers of this unit type
	for (const Unit* u : Utils::GetOwnUnits(Observation(), producer)) {
		//Can it handle the request?  A factory requires a tech lab to build a siege tank, for example
		Structure s(u);
		if (s.HasAbilityAvailable(bot, abilityID)) {
			//Is it doing something else?  We never want to queue anything, just wait until it clears
			if (s.getOrderCount() == 0) {
				//Yay, let's build here
				Actions()->UnitCommand(u, abilityID);
				return true;
			}
		}
	}

	//failed to find a spot.  Try again later.
	return false;
}

std::string ArmyManager::GetDebugSummaryString()
{
	std::ostringstream oss;
	oss << "Army Summary.  Platoons:  " << armyPlatoons.size() << std::endl;
	for (Platoon & platoon : armyPlatoons) {
		oss << platoon.GetDebugSummaryString() << std::endl;
	}

	return oss.str();
}

void ArmyManager::ManageMilitary()
{
	//V1:  Replicate the old terrible strategy with platoons.
	switch (currentStrategy) {
	case 0:	//Game initialization, build shit and don't do anything.
		{
			size_t armyUnitCount = GetTotalArmyUnitCount();
			if (armyUnitCount > 0) {
				//Move to defend
				currentStrategy = 1;

				//TODO:  Position.  Picking the natural choke for now
				Point2D targetPoint;
				std::vector<Point2D> chokes = bot.Map().GetRegionChokePoints(bot.BaseLocations().Natural()->GetRegionId());
				if (chokes.size() > 0) {
					targetPoint = chokes[0];
				}
				else {
					//TODO
					std::cout << "WARNING:  No choke points available to set defense target" << std::endl;
				}

				//Tell each platoon to defend
				for (Platoon & platoon : armyPlatoons) {
					platoon.SetOrders(Platoon::PLATOON_ORDERS::DEFEND, targetPoint);
				}

				//TODO:  New platoons
			}
		}
		break;
	case 1:	//Defend the base!
		{
			size_t armyUnitCount = GetTotalArmyUnitCount();
			if (armyUnitCount > 20) {
				//Move to offense
				currentStrategy = 2;

				//TODO:  Position.  Picking the enemy start for now
				Point2D targetPoint = bot.Observation()->GetGameInfo().enemy_start_locations.front();

				//Tell each platoon to attack
				for (Platoon & platoon : armyPlatoons) {
					platoon.SetOrders(Platoon::PLATOON_ORDERS::ATTACK, targetPoint);
				}

				//TODO:  New platoons
			}
		}
		break;
	case 2:	//Offense, defeat the enemy
		{
			//TODO:  Come back to defend?
		}
		break;
	}
}

size_t ArmyManager::GetTotalArmyUnitCount()
{
	size_t size = 0;
	for (Platoon & platoon : armyPlatoons) {
		size += platoon.GetTotalPlatoonUnitCount();
	}
	return size;
}
