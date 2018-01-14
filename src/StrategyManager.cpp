#include "StrategyManager.h"
#include "bot.h"
#include "BuildQueueItem.h"
#include "BuildQueueItem_Auto.h"
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
	BO_ExpandFirstIntoBioBall();

	//BO_SimpleBio();
	

}

void StrategyManager::OnStep()
{
}

void StrategyManager::BO_ExpandFirstIntoBioBall()
{
	//Attempt to follow a build order.  We'll start with this one which has enough complexity to push us along
	//	http://www.sc2bm.com/builds/159#expand
	//Note numbers in the article are 1 higher ("the 13th scv")
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_COMMANDCENTER));	//@ 17 scv
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));	//@ 19 scv
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::MORPH_ORBITALCOMMAND));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::MORPH_ORBITALCOMMAND));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));	//@ 22 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BUNKER));
	//TODO:  SCV TO GAS- auto?  6x
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 33 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 37 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_STIMPACK));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	//TODO:  Scanner sweep auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_FACTORY));	//@ 44 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_ENGINEERINGBAY));	//@ 44 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 46 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 52 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS));
	//Econ manager will auto handle moving SCVs to gas
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));

	//Turn on automatic SCV management.  We don't need a build order for every SCV.
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem_Auto>(AUTO_ABILITYID::ENABLE_AUTOBUILDWORKERS));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_STARPORT));	//@ 60 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_FACTORY));	//@ 60 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 60 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 66 supply
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 72 supply
																			//TODO:  Lift Factory from Reactor	
																			//TODO:  Land Starport on Reactor	
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_COMMANDCENTER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MEDIVAC));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MEDIVAC));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));	//@ 78 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_FACTORY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_COMBATSHIELD));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_CONCUSSIVESHELLS));
	//Econ manager will auto handle moving SCVs to gas
	//TODO:  Scanner sweep auto?
	//END OF BUILD ORDER
}

//Simple just for testing - gets us units fast to test attacking
void StrategyManager::BO_SimpleBio()
{
	/*
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));	//@ 19 scv
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));	//@ 22 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_SCV));

	//just keep building marines
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));
	*/


}
