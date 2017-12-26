#include "StrategyManager.h"
#include "bot.h"

StrategyManager::StrategyManager(Bot & b)
	: ManagerBase(b)
{
}

void StrategyManager::OnGameStart()
{
	//for now
	bot.Econ().EnableAutonomy();
	bot.Supply().EnableAutonomy();
	bot.Army().EnableAutonomy();
	bot.Upgrades().EnableAutonomy();
}

void StrategyManager::OnStep()
{
	//Attempt to follow a build order.  We'll start with this one which has enough complexity to push us along
	//	http://www.sc2bm.com/builds/159#expand

	int32_t currentFood = Observation()->GetFoodUsed();
	switch (currentFood) {
	case 13:
		//build an scv - econmgr autonous
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
