#include "VespeneWorkerBalanceModule.h"

VespeneWorkerBalanceModule::VespeneWorkerBalanceModule(Bot & b)
	: ModuleBase(b)
{
}

ModuleNotificationRequirement VespeneWorkerBalanceModule::GetNotificationRequirements()
{
	//We need every 30th step or so.  Doesn't have to be all that frequent.  Rough guess.
	return ModuleNotificationRequirement(false, 30, false, false, false, false, false, false, false, false);
}
