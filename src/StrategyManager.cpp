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
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_COMMANDCENTER));	//@ 17 scv
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));	//@ 19 scv
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::MORPH_ORBITALCOMMAND));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::MORPH_ORBITALCOMMAND));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BARRACKS));	//@ 22 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_BUNKER));
	//TODO:  SCV TO GAS- auto?  6x
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 33 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 37 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::RESEARCH_STIMPACK));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	//TODO:  Scanner sweep auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_FACTORY));	//@ 44 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_ENGINEERINGBAY));	//@ 44 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 46 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 52 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS));
	//TODO:  SCV TO GAS- auto?  3x
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_STARPORT));	//@ 60 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REACTOR_FACTORY));	//@ 60 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 60 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 66 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 72 supply
	//TODO:  Lift Factory from Reactor	
	//TODO:  Land Starport on Reactor	
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_COMMANDCENTER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MEDIVAC));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MEDIVAC));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REFINERY));	//@ 78 supply
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::BUILD_REACTOR_FACTORY));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::RESEARCH_COMBATSHIELD));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(BuildQueueItem(ABILITY_ID::RESEARCH_CONCUSSIVESHELLS));
	//TODO:  SCV TO GAS- auto?  3x
	//TODO:  Scanner sweep auto?

	//END OF BUILD ORDER

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
