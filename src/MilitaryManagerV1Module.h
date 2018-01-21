#pragma once
#include <sc2api/sc2_api.h>
#include "ModuleBase.h"
class Platoon;
class Bot;
class BaseLocation;

//Simple module to manage your military (version 1).  Just copied much of what the ArmyManager used to do.
class MilitaryManagerV1Module : public ModuleBase
{
public:
	explicit MilitaryManagerV1Module(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnStep();

private:
	void AssignPlatoonToBaseDefense(std::shared_ptr<Platoon> platoon);
	BaseLocation* FindEnemyBaseToAttack();
	sc2::Point2D FindNonBaseAttackPoint();
	void AssignPlatoonToAttack(std::shared_ptr<Platoon> platoon);
	void AttackTargetBaseLocationWithPlatoon(BaseLocation * target, std::shared_ptr<Platoon> &platoon);

	//TODO:  Maybe move platoons here eventually?  Maybe a platoon module.
};
