#include "BuildQueueManager.h"
#include "bot.h"
using namespace sc2;

BuildQueueManager::BuildQueueManager(Bot & b)
	: ManagerBase(b)
{
}

void BuildQueueManager::Push(BuildQueueItem item)
{
	buildQueue.push_back(item);
}

void BuildQueueManager::OnStep()
{
	//Check for no work
	if (buildQueue.size() == 0) {
		return;
	}

	//The queue system doesn't seem to work well at full speed - issued commands are not
	//	accepted fast enough, so the next command may get issued but fail.  TODO:  I picked
	//	5 arbitrarily, play with this.
	//Example:  Game starts, first scv queue happens, but by frame 2, we still have 50 minerals 
	//	and the game attempts to queue a second one - but we really don't have 50 minerals.
	//Bumped from 5 to 10.  Running into even more issues -- I tell the construction manager (which
	//	has a lengthy frame-based queue of its own) to build a depot... and before the command can get
	//	issued, the build order queue builds another worker, eating up the resources that we were 
	//	going to use.
	//TODO2:  CCbot saves a 'reserved minerals' and 'reserved gas' flag into its construction manager.  I guess
	//	I now know why.  We can probably ramp this back down if we implement that.
	if (bot.Observation()->GetGameLoop() % 10 != 0) {
		return;
	}

	//Work on the first item
	BuildQueueItem& item = buildQueue[0];

	//For now, we break down into 3 types of things to build:  Worker, Building, any other unit
	if (IsWorker(item.abilityToTrain)) {
		if (bot.Econ().TrainWorker()) {
			buildQueue.erase(buildQueue.begin());
			std::cout << "Worker queued" << std::endl;
		}
	}
	else if (IsBuilding(item.abilityToTrain)) {
		//TODO:  We should move the item to a "building" state or hold it in a separate queue -- if it fails, re-queue it at the top, 
		//	if it succeeds, fully remove it then.
		if (HasResourcesFor(item.abilityToTrain)) {
			//The construction manager will do its best to make sure this building gets constructed.
			uint64_t queueId = bot.Construction().BuildStructure(item.abilityToTrain,
				std::bind(&BuildQueueManager::OnConstructionSuccess, this, std::placeholders::_1),
				std::bind(&BuildQueueManager::OnConstructionFailed, this, std::placeholders::_1));

			std::cout << "Starting new building(" << AbilityTypeToName(item.abilityToTrain) << "), task id:  " << queueId << std::endl;

			buildQueue.erase(buildQueue.begin());
		}
	}
	else if (IsUnit(item.abilityToTrain)) {
		//TODO
	}
	else {
		//We must have missed one
		std::cout << "Unknown build queue item:  " << sc2::AbilityTypeToName(item.abilityToTrain) << std::endl;
	}
}

void BuildQueueManager::OnConstructionSuccess(int64_t taskId)
{
	std::cout << "Construction success, task:  " << taskId << std::endl;
}

void BuildQueueManager::OnConstructionFailed(int64_t taskId)
{
	std::cout << "Construction failed, task:  " << taskId << std::endl;
}

bool BuildQueueManager::IsWorker(sc2::ABILITY_ID abilityID)
{
	//Only one type of worker in our terran-only bot
	return abilityID == ABILITY_ID::TRAIN_SCV;
}

bool BuildQueueManager::IsBuilding(sc2::ABILITY_ID abilityID)
{
	//Leverage our structures manager - if it knows the unit type, it's a building.  otherwise it must not be.
	UNIT_TYPEID u = StructuresManager::UnitTypeFromBuildAbility(abilityID);
	if (u == UNIT_TYPEID::INVALID)
		return false;
	return true;
}

bool BuildQueueManager::IsUnit(sc2::ABILITY_ID abilityID)
{
	//If it's not the other 2, must be us.
	if (!IsWorker(abilityID) && !IsBuilding(abilityID))
		return true;
	return false;
}

//TODO:  complete hack for now, get rid of this.
bool BuildQueueManager::HasResourcesFor(sc2::ABILITY_ID abilityID)
{
	int32_t currentMinerals = bot.Observation()->GetMinerals();
	int32_t currentVespene = bot.Observation()->GetVespene();
	int32_t currentFood = bot.Observation()->GetFoodUsed();
	int32_t currentFoodCap = bot.Observation()->GetFoodCap();

	//Get the unit data
	UnitTypes ut = bot.Observation()->GetUnitTypeData();
	UnitTypeData data = ut[(UnitTypeID)StructuresManager::UnitTypeFromBuildAbility(abilityID)];

	//Ensure we have the minerals, gas, and enough supply
	if (data.mineral_cost <= currentMinerals && data.vespene_cost <= currentVespene && currentFood + data.food_required <= currentFoodCap)
		return true;
	return false;
}
