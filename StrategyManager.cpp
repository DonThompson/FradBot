#include "StrategyManager.h"
#include "bot.h"

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

	int32_t currentFood = Observation()->GetFoodUsed();
	switch (currentFood) {
	case 13:	//Build an SCV
		bot.Econ().TrainWorker();
		break;
	case 14:
		//build an scv - econmgr autonomous
		//build a depot - supplymgr autonomous
		break;
	case 15:
		//build an scv - econmgr autonomous
		break;
	case 16:
		//build an scv - econmgr autonomous
		break;
	case 17:
		//TODO:  Econ will quit here
		//Expand to our natural!
	{
		//where are we?
		//bot.BaseLocations().GetLocationById()
		//expand nonsense
		//bot.Construction().Expand(())
	}
		
		break;

	}
}
