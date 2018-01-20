#pragma once
#include <sc2api/sc2_api.h>
#include "ModuleBase.h"
class Platoon;
class Bot;

//Simple module to manage your military (version 1).  Just copied much of what the ArmyManager used to do.
class MilitaryManagerV1Module : public ModuleBase
{
public:
	explicit MilitaryManagerV1Module(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	void AssignPlatoonToBaseDefense(std::shared_ptr<Platoon> platoon);
	virtual void OnStep();

	//TODO:  Maybe move platoons here eventually?  Maybe a platoon module.
};
