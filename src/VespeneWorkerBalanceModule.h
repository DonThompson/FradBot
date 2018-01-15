#pragma once
#include "ModuleBase.h"
class Bot;

//Module that monitors for vespene mining to ensure that we get 3 workers on each vespene geyser.
class VespeneWorkerBalanceModule : public ModuleBase
{
public:
	explicit VespeneWorkerBalanceModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnStep();

private:
	void BalanceGasWorkers();
};
