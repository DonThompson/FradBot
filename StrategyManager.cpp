#include "StrategyManager.h"

StrategyManager::StrategyManager(Bot & b)
	: ManagerBase(b)
{
}

void StrategyManager::OnStep()
{
	//Attempt to follow abuild order.  We'll start with this one which has enough complexity to push us along
}
