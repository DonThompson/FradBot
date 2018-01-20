#pragma once
#include "ModuleBase.h"
class Bot;

//Module that scans the map looking for buildings and assigning them to bases.  Updates base ownership as we go.
class BaseBuildingScannerModule : public ModuleBase
{
public:
	explicit BaseBuildingScannerModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnStep();
	virtual void OnUnitDestroyed(const sc2::Unit* unit);

private:
	//We only need to scan fairly infrequently
	const uint32_t stepLoopCount = 500;
};

