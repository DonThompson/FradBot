#pragma once
#include "ModuleBase.h"
class Bot;

class VespeneWorkerBalanceModule : public ModuleBase
{
public:
	VespeneWorkerBalanceModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();


};

