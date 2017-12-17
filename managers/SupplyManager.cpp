#include "SupplyManager.h"
#include "../bot.h"

void SupplyManager::successTest(int64_t x)
{
	//Reduce our depot in progress count, this one completed
	depotsInProgress--;

	std::cout << "Supply build success, task:  " << x << std::endl;
}

void SupplyManager::failTest(int64_t y)
{
	//Reduce our depot in progress count, this one failed out
	depotsInProgress--;

	std::cout << "Supply build failed, task:  " << y << std::endl;
}

SupplyManager::SupplyManager(Bot & b)
	: ManagerBase(b)
	, depotsInProgress(0)
	, lastBalanceClock(clock_t())
{
}

SupplyManager::~SupplyManager()
{
}

void SupplyManager::OnStep()
{
	//Rebalance supply every few seconds.  We really don't need to do this every step.
	//TODO:  Clock is REAL time.  Should we use game time?  In super fast AI mode this might actually be 5-6 game seconds.
	const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
	if (clock() - lastBalanceClock > rebalanceTime) {

		//Specific logic
		if (SupplyDepotNeeded()) {
			BuildSupplyDepot();
		}

		lastBalanceClock = clock();
	}
}

//Evaluate if we need a depot by predicting what we may build imminently.
bool SupplyManager::SupplyDepotNeeded()
{
	const ObservationInterface* observation = Observation();
	int32_t supplyUsed = observation->GetFoodUsed();
	int32_t currentSupplyCap = observation->GetFoodCap();

	//Shortcut - never go over 200
	if (currentSupplyCap >= 200) {
		return false;
	}

	//Predict how many we need to be building.
	int32_t numDepotNeeded = PredictSupplyDepotsNeeded();

	if (numDepotNeeded > depotsInProgress) {
		return true;
	}

	return false;
}

//Predicts a count of supply depots needed based on what units we are likely to build in the near term.
//	Implementation basically counts what we're currently building and assumes that once those all finish, we'll build more of the same.
//	Also always builds a depot if we're > 90% current capacity.
//TODO:  This is probably not a great long term option.
int32_t SupplyManager::PredictSupplyDepotsNeeded()
{
	const int32_t supplyDepotFood = 8;

	const ObservationInterface* observation = Observation();
	int32_t supplyUsed = observation->GetFoodUsed();
	int32_t currentSupplyCap = observation->GetFoodCap();

	//Total units currently being built.
	int32_t supplyCurrentlyBeingProduced = CalculateSupplyCurrentlyBeingProduced();

	//Assumption:  Once the current units get built, we'll shortly want to be able to build at least that much more again.  If that is going to put us
	//  at a supply disadvantage, then we'd better start a new depot.
	int32_t extraNeeded = supplyUsed + supplyCurrentlyBeingProduced - currentSupplyCap;
	if (extraNeeded <= 0) {
		//Shortcut:  If we're over 90% capacity, build a new depot, always.
		if (supplyUsed >= (int32_t)(currentSupplyCap * 0.90f))
			return 1;
		return 0;
	}

	return (int32_t)ceil((double)extraNeeded / (double)supplyDepotFood);
}

int32_t SupplyManager::CalculateSupplyCurrentlyBeingProduced()
{
	const ObservationInterface* observation = Observation();
	ActionInterface* actions = Actions();

	//Loop through all units and see if they're building a unit right now, and if so what the cost of that unit is
	int32_t supplyBeingProduced = 0;

	Units units = observation->GetUnits(Unit::Alliance::Self);
	for (const auto& unit : units) {
		for (const auto& order : unit->orders) {
			supplyBeingProduced += GetUnitSupplyActivelyProducing(order);
		}
	}

	return supplyBeingProduced;
}

int32_t SupplyManager::GetUnitSupplyActivelyProducing(UnitOrder order)
{
	//NOTE:  I manually ripped these out of ABILITY_ID enum and manually entered the food cost by lookup.  Any way to calculate these?
	switch ((sc2::ABILITY_ID)order.ability_id) {
	case ABILITY_ID::TRAINWARP_ADEPT:
	case ABILITY_ID::TRAINWARP_DARKTEMPLAR:
	case ABILITY_ID::TRAINWARP_HIGHTEMPLAR:
	case ABILITY_ID::TRAINWARP_SENTRY:
	case ABILITY_ID::TRAINWARP_STALKER:
	case ABILITY_ID::TRAINWARP_ZEALOT:
	case ABILITY_ID::TRAIN_ADEPT:
	case ABILITY_ID::TRAIN_BANELING:
	case ABILITY_ID::TRAIN_BANSHEE:
	case ABILITY_ID::TRAIN_BATTLECRUISER:
	case ABILITY_ID::TRAIN_CARRIER:
	case ABILITY_ID::TRAIN_COLOSSUS:
	case ABILITY_ID::TRAIN_CORRUPTOR:
	case ABILITY_ID::TRAIN_CYCLONE:
	case ABILITY_ID::TRAIN_DARKTEMPLAR:
	case ABILITY_ID::TRAIN_DISRUPTOR:
	case ABILITY_ID::TRAIN_DRONE:
	case ABILITY_ID::TRAIN_GHOST:
	case ABILITY_ID::TRAIN_HELLBAT:
	case ABILITY_ID::TRAIN_HELLION:
	case ABILITY_ID::TRAIN_HIGHTEMPLAR:
	case ABILITY_ID::TRAIN_HYDRALISK:
	case ABILITY_ID::TRAIN_IMMORTAL:
	case ABILITY_ID::TRAIN_INFESTOR:
	case ABILITY_ID::TRAIN_LIBERATOR:
	case ABILITY_ID::TRAIN_MARAUDER:
	case ABILITY_ID::TRAIN_MARINE:
	case ABILITY_ID::TRAIN_MEDIVAC:
	case ABILITY_ID::TRAIN_MOTHERSHIPCORE:
	case ABILITY_ID::TRAIN_MUTALISK:
	case ABILITY_ID::TRAIN_OBSERVER:
	case ABILITY_ID::TRAIN_ORACLE:
		//case ABILITY_ID::TRAIN_OVERLORD:  //no supply
	case ABILITY_ID::TRAIN_PHOENIX:
	case ABILITY_ID::TRAIN_PROBE:
	case ABILITY_ID::TRAIN_QUEEN:
	case ABILITY_ID::TRAIN_RAVEN:
	case ABILITY_ID::TRAIN_REAPER:
	case ABILITY_ID::TRAIN_ROACH:
	case ABILITY_ID::TRAIN_SCV:
	case ABILITY_ID::TRAIN_SENTRY:
	case ABILITY_ID::TRAIN_SIEGETANK:
	case ABILITY_ID::TRAIN_STALKER:
	case ABILITY_ID::TRAIN_SWARMHOST:
	case ABILITY_ID::TRAIN_TEMPEST:
	case ABILITY_ID::TRAIN_THOR:
	case ABILITY_ID::TRAIN_ULTRALISK:
	case ABILITY_ID::TRAIN_VIKINGFIGHTER:
	case ABILITY_ID::TRAIN_VIPER:
	case ABILITY_ID::TRAIN_VOIDRAY:
	case ABILITY_ID::TRAIN_WARPPRISM:
		//case ABILITY_ID::TRAIN_WIDOWMINE: //No supply used
	case ABILITY_ID::TRAIN_ZEALOT:
	case ABILITY_ID::TRAIN_ZERGLING:
		//TODO:  Need to actually find the value for each unit.  Until then we'll just use an incorrect value of 1.
		return 1;
		break;
	default:
		//Any other ID is not a unit being trained
		return 0;
		break;
	}
}

void SupplyManager::BuildSupplyDepot()
{
	//The build manager now ensures your command is completed or it sends a callback failure to you.

	//TODO:  We probably should map queue tasks to completed/failed tasks.
	//TODO:  Document this crazy bind syntax somewhere.  It's much simpler on a generic function, but 
	//	we'll probably always be using class instance members.
	int64_t queueId = bot.Building().BuildStructure(ABILITY_ID::BUILD_SUPPLYDEPOT, 
		std::bind(&SupplyManager::successTest, this, std::placeholders::_1), 
		std::bind(&SupplyManager::failTest, this, std::placeholders::_1));

	std::cout << "Starting new depot, task id:  " << queueId << std::endl;


	depotsInProgress++;
}
