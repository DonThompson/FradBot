#include "StrategyManager.h"
#include "bot.h"
#include "BuildQueueItem.h"
#include "BuildQueueModuleItem.h"
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
	//BO_SimpleBio();
	//BO_ExpandFirstIntoBioBall();
	BO_RFE_AllRaceMatchup_BioBall();
	

}

void StrategyManager::OnStep()
{
	//Not needed at this point
}

void StrategyManager::AddAbilityToBuildOrder(sc2::ABILITY_ID abilityID)
{
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(abilityID));
}

void StrategyManager::AddModuleToBuildOrder(MODULE_ID moduleID)
{
	bot.BuildQueue().Push(std::make_shared<BuildQueueModuleItem>(moduleID));
}

//Built based on the information from this article:
//	https://terrancraft.com/2017/10/06/one-build-for-three-match-ups/
void StrategyManager::BO_RFE_AllRaceMatchup_BioBall()
{
	//TODO:  I've had to roughly guess by inserting scv training to hit the build points.  This is trial and error and not great.  I'd be better off
	//	enabling auto-scvs and just letting the build order run.  Maybe things shouldn't time out until we reach a certain point in the BO?

	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//13th
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//14th
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_SUPPLYDEPOT);
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//15th
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_REFINERY);
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//16th
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_BARRACKS);
	//TODO:  Some delays or guarantees would be handy to make sure these build in order
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//17th
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//18th
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_SCV);	//19th
	AddAbilityToBuildOrder(ABILITY_ID::TRAIN_REAPER);	//20th
	AddAbilityToBuildOrder(ABILITY_ID::MORPH_ORBITALCOMMAND);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_COMMANDCENTER);	//expand @ 100% orbital/400 mins
	//auto build workers from here out
	AddModuleToBuildOrder(MODULE_ID::AUTOBUILDWORKERSMODULE);
	//auto army
	AddModuleToBuildOrder(MODULE_ID::ARMYTRAINER_BIOBALLMODULE);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_FACTORY);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_REACTOR_BARRACKS);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_SUPPLYDEPOT);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_REFINERY);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_STARPORT);
	AddAbilityToBuildOrder(ABILITY_ID::BUILD_SUPPLYDEPOT);


	//END PHASE 1.

	//After the core of our build order, we should enable timeouts for safety.  Avoids the entire manager getting stuck if it can never build.
	//TODO:  This code is just for queue.  Do we need a module to enable timeout?  Bleh.
	//bot.BuildQueue().EnableTimeout();

	//TODO:  PHASE 2+
}

//Deprecated.  This was v2 build order.  Works really well if we have time to macro up.  Fails hard in tournament because everyone is cheesy.  Need to slow down the expansion in v3.
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

	//Turn on automatic SCV management.  We don't need a build order for every SCV from here out.
	bot.BuildQueue().Push(std::make_shared<BuildQueueModuleItem>(MODULE_ID::AUTOBUILDWORKERSMODULE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 37 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_STIMPACK));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	//TODO:  Scanner sweep auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_FACTORY));	//@ 44 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_ENGINEERINGBAY));	//@ 44 supply
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 46 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 52 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS));
	//Econ manager will auto handle moving SCVs to gas
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_GHOSTACADEMY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARINE));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::TRAIN_MARAUDER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_STARPORT));	//@ 60 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_FACTORY));	//@ 60 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 60 supply

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_TECHLAB_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));

	//Turn on automatic army building - just keep making our bio ball.  Don't do this until we have the addons we want
	bot.BuildQueue().Push(std::make_shared<BuildQueueModuleItem>(MODULE_ID::ARMYTRAINER_BIOBALLMODULE));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR));

	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 66 supply
	//TODO:  Mules should probably always be auto?
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_SUPPLYDEPOT));	//@ 72 supply
																			//TODO:  Lift Factory from Reactor	
																			//TODO:  Land Starport on Reactor	
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_COMMANDCENTER));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REFINERY));	//@ 78 supply
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_FACTORY));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_COMBATSHIELD));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_CONCUSSIVESHELLS));
	//Econ manager will auto handle moving SCVs to gas
	//TODO:  Scanner sweep auto?
	//END OF BUILD ORDER

	//reactor that starport and add 2 more rax with reactors
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS));

	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_STARPORT));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_ARMORY));		//for upgrades
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::BUILD_REACTOR_BARRACKS));
	bot.BuildQueue().Push(std::make_shared<BuildQueueItem>(ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR));

}

//v1 build order.  Simple just for testing - gets us units fast to test attacking
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
