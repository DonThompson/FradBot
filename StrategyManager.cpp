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


	//int32_t currentFood = Observation()->GetFoodUsed();
	switch (currentFood) {
	case 12:	//Build an SCV
		bot.Econ().TrainWorker();
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		break;
	case 13:	//Build an SCV
		bot.Econ().TrainWorker();
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		//build a depot - supplymgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));
		break;
	case 14:
		//build an scv - econmgr autonomous
		bot.Econ().TrainWorker();
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		break;
	case 15:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		break;
	case 16:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		//TODO:  Econ will quit here
		//Expand to our natural!
		{
			//where are we?
			//bot.BaseLocations().GetLocationById()
			//expand nonsense
			//bot.Construction().Expand(())
		}
		break;
	case 17:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		//rax 1
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));
		break;
	case 18:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		//rax 2
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));
		break;
	case 19:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		break;
	case 20:
		//build an scv - econmgr autonomous
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
		break;
	case 21:
		//first marine
		bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
		break;
	}
}
