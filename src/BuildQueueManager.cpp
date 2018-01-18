#include "BuildQueueManager.h"
#include "bot.h"
#include "BuildQueueModuleItem.h"
#include "BuildQueueItem.h"
using namespace sc2;
using namespace std;

BuildQueueManager::BuildQueueManager(Bot & b)
	: ManagerBase(b)
{
}

void BuildQueueManager::Push(std::shared_ptr<BuildQueueItemBase> item)
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
	//Our resource reservation system for construction helps us keep this reasonably low.
	//TODO:  It seems like a unit reservation system would be equally useful (and include food too).
	uint32_t gameLoop = bot.Observation()->GetGameLoop();
	if (gameLoop % 5 != 0) {
		return;
	}

	//Work on the first item
	std::shared_ptr<BuildQueueItemBase> & item = buildQueue.at(0);

	//First, we put a safety check for timeout.  Don't want items stuck in the build queue forever that we can never
	//	build for some reason.  Early in dev, this handles unforeseen situations and things not yet build into the
	//	manager.  Later, this should be a catch against edge cases... for exampleyou lose all your gas workers and 
	//	the upgrade queued for 200 gas keeps anything else from building.
	if (item->CheckTimeout(gameLoop)) {
		//It timed out, dequeue this request
		std::cout << "WARNING:  ITEM TIMED OUT.  REMOVING FROM QUEUE.  (" << item->GetDescription() << ")" << std::endl;
		buildQueue.erase(buildQueue.begin());
		return;
	}

	switch (item->itemType) {
	case BUILD_QUEUE_TYPE::GAME_MODULE:
		TryHandleModule(static_pointer_cast<BuildQueueModuleItem>(item));
		break;
	case BUILD_QUEUE_TYPE::GAME_ABILITY:
		TryHandleGameAbility(static_pointer_cast<BuildQueueItem>(item));
		break;
	default:
		std::cout << "ERROR:  Unknown build queue item type" << std::endl;
		break;
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
	//Leverage our structures manager - it knows all
	return StructuresManager::IsBuilding(abilityID);
}

bool BuildQueueManager::IsUnit(sc2::ABILITY_ID abilityID)
{
	//TODO:  Needs redone.  Requires we keep remembering to update this

	//If it's not the others, must be us.
	if (!IsWorker(abilityID) && !IsBuilding(abilityID) && !UpgradesManager::IsUpgrade(abilityID) && !MorphManager::IsMorph(abilityID))
		return true;
	return false;
}

bool BuildQueueManager::HasResourcesFor(sc2::ABILITY_ID abilityID)
{
	//What do we have currently?  Remove any construction reservation that is currently ongoing
	int32_t currentMinerals = bot.Observation()->GetMinerals() - bot.Construction().GetReservedMinerals();
	int32_t currentVespene = bot.Observation()->GetVespene() - bot.Construction().GetReservedVespene();
	int32_t currentFood = bot.Observation()->GetFoodUsed();
	int32_t currentFoodCap = bot.Observation()->GetFoodCap();

	//Get the unit data
	UnitData data = bot.Data().GetUnitData(abilityID);

	//Ensure we have the minerals, gas, and enough supply
	if (data.mineral_cost <= currentMinerals && data.vespene_cost <= currentVespene && currentFood + data.food_required <= currentFoodCap)
		return true;
	return false;
}

void BuildQueueManager::TryHandleModule(const std::shared_ptr<BuildQueueModuleItem> & item)
{
	//Enable the module given
	bot.Modules().EnableModule(item->moduleID);
	buildQueue.erase(buildQueue.begin());
}

void BuildQueueManager::TryHandleGameAbility(const std::shared_ptr<BuildQueueItem> & item)
{
	//Ensure we have enough resources available to train this item
	if (!HasResourcesFor(item->abilityToTrain)) {
		return;
	}

	//For now, we break down into types of things to build:  Worker, Building, any other unit, Upgrades, morph, etc.
	if (IsWorker(item->abilityToTrain)) {
		if (bot.Econ().TrainWorker()) {
			buildQueue.erase(buildQueue.begin());
			std::cout << "Worker queued" << std::endl;
		}
	}
	else if (IsBuilding(item->abilityToTrain)) {
		//TODO:  We should move the item to a "building" state or hold it in a separate queue -- if it fails, re-queue it at the top, 
		//	if it succeeds, fully remove it then.
		//The construction manager will do its best to make sure this building gets constructed.
		uint64_t queueId = bot.Construction().BuildStructure(item->abilityToTrain,
			std::bind(&BuildQueueManager::OnConstructionSuccess, this, std::placeholders::_1),
			std::bind(&BuildQueueManager::OnConstructionFailed, this, std::placeholders::_1));

		std::cout << "Starting new building(" << item->GetDescription() << "), task id:  " << queueId << std::endl;

		buildQueue.erase(buildQueue.begin());
	}
	else if (IsUnit(item->abilityToTrain)) {
		//If the queue has a producer building already attached, use it directly
		if (item->fromBuilding != nullptr) {
			if (bot.Army().TrainUnitFromBuilding(item->abilityToTrain, item->fromBuilding)) {
				std::cout << item->GetDescription() << " queued" << std::endl;
				buildQueue.erase(buildQueue.begin());
			}
		}
		else {
			//We need to find a building, then train it
			if (bot.Army().TrainUnit(item->abilityToTrain)) {
				std::cout << item->GetDescription() << " queued" << std::endl;
				buildQueue.erase(buildQueue.begin());
			}
		}
	}
	else if (UpgradesManager::IsUpgrade(item->abilityToTrain)) {
		if (bot.Upgrades().PerformUpgrade(item->abilityToTrain)) {
			std::cout << item->GetDescription() << " queued" << std::endl;
			buildQueue.erase(buildQueue.begin());
		}
	}
	else if (MorphManager::IsMorph(item->abilityToTrain)) {
		if (bot.Morph().PerformMorph(item->abilityToTrain)) {
			std::cout << item->GetDescription() << " queued" << std::endl;
			buildQueue.erase(buildQueue.begin());
		}
	}
	else {
		//We must have missed one
		std::cout << "ERROR:  Unknown build queue item:  " << item->GetDescription() << ".  DEQUEUEING" << std::endl;
		buildQueue.erase(buildQueue.begin());
	}
}
