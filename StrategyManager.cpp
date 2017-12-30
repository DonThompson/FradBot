#include "StrategyManager.h"
#include "bot.h"
using namespace sc2;

StrategyManager::StrategyManager(Bot & b)
	: ManagerBase(b)
{
}

void StrategyManager::OnGameStart()
{
	//Until now, our "strategy" has been to let each part of the bot operate independently to come
	//	out with a good result.
	/*
	bot.Econ().EnableAutonomy();
	bot.Supply().EnableAutonomy();
	bot.Army().EnableAutonomy();
	bot.Upgrades().EnableAutonomy();
	*/

	//New strategy leaves them all reliant on the strategy manager to coordinate!

	//Setup our initial build order
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		//Expand to our natural!
		{
			//where are we?
			//bot.BaseLocations().GetLocationById()
			//expand nonsense
			//bot.Construction().Expand(())
		}
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));

		//TODO:  Continue on...
}

void StrategyManager::OnStep()
{
	//Attempt to follow a build order.  We'll start with this one which has enough complexity to push us along
	//	http://www.sc2bm.com/builds/159#expand
	//Note numbers in the article are 1 higher ("the 13th scv")
	int32_t lastFoodSeen = 0;
	int32_t currentFood = Observation()->GetFoodUsed();

	//TODO:  Works at early game, messes up once you lose a unit?
	if (currentFood == lastFoodSeen) {
		return;
	}

}
