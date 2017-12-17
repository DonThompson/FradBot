#include "ArmyManager.h"
#include "../bot.h"

ArmyManager::ArmyManager(Bot & b)
	: ManagerBase(b)
	, lastBalanceClock(clock_t())
	, raxInProgress(0)
{
}

ArmyManager::~ArmyManager()
{
}

void ArmyManager::OnStep()
{
	//Rebalance supply every few seconds.  We really don't need to do this every step.
	//TODO:  Clock is REAL time.  Should we use game time?  In super fast AI mode this might actually be 5-6 game seconds.
	const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
	if (clock() - lastBalanceClock > rebalanceTime) {

		//Specific logic
		if (BarracksNeeded()) {
			BuildBarracks();
		}

		//The whole strategy!
		TryAttackInGroups();

		lastBalanceClock = clock();
	}
}

bool ArmyManager::BarracksNeeded()
{
	const ObservationInterface* observation = Observation();

	//Includes under construction and completed
	//TODO:  Better to not count in progress so we can use our own counter?
	int32_t countBarracks = Utils::CountOwnUnits(observation, UNIT_TYPEID::TERRAN_BARRACKS);
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
		if (countBarracks < maxDesiredRax && currentMinerals >= 400) {
			return true;
		}
	}

	return false;
}

void ArmyManager::BuildBarracks()
{
	int64_t queueId = bot.Construction().BuildStructure(ABILITY_ID::BUILD_BARRACKS,
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

void ArmyManager::TryAttackInGroups()
{
	const ObservationInterface* observation = Observation();

	Units idleArmy = Utils::GetIdleUnits(observation, UNIT_TYPEID::TERRAN_MARINE, UNIT_TYPEID::TERRAN_MARAUDER);
	if (idleArmy.size() > 15) {
		LaunchAttackGroup(idleArmy);
	}
}

void ArmyManager::LaunchAttackGroup(Units unitsToAttack)
{
	const GameInfo& gameInfo = Observation()->GetGameInfo();
	//What is attack_attack vs attack?
	Actions()->UnitCommand(unitsToAttack, ABILITY_ID::ATTACK_ATTACK, gameInfo.enemy_start_locations.front());
}

void ArmyManager::OnBarracksIdle(const Unit* unit)
{
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

void ArmyManager::OnMarineIdle(const Unit* unit)
{
	/* too aggressive
	const GameInfo& gameInfo = Observation()->GetGameInfo();
	//What is attack_attack vs attack?
	Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, gameInfo.enemy_start_locations.front());
	*/
}

void ArmyManager::OnUnitIdle(const Unit* unit)
{
	switch (unit->unit_type.ToType()) {
	case UNIT_TYPEID::TERRAN_BARRACKS:      OnBarracksIdle(unit);       break;
	case UNIT_TYPEID::TERRAN_MARINE:        OnMarineIdle(unit);         break;
	default:    break;
	}
}
