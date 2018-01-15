#pragma once
#include "ModuleBase.h"
#include <sc2api/sc2_api.h>
#include "Structure.h"
class Bot;

//Module that automatically builds workers when a base location does not have enough.
class AutoBuildWorkersModule : public ModuleBase
{
public:
	explicit AutoBuildWorkersModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnStep();
	virtual void OnUnitIdle(const sc2::Unit* unit);

private:
	void OnCCIdle(Structure commandCenter);
};

