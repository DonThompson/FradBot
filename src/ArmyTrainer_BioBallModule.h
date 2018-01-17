#pragma once
#include <sc2api/sc2_api.h>
#include "ModuleBase.h"
#include "ModuleNotificationRequirement.h"
#include "Structure.h"
class Bot;

//This module automatically trains an army of "bio ball" units -- Marines, Marauders, and Medivacs when enabled.  It will attempt to 
//	auto build any time a unit-producing building goes idle, and will check periodically outside of that window.
class ArmyTrainer_BioBallModule : public ModuleBase
{
private:
	//Special data POCO just for this class use
	struct CurrentBioArmyData {
	public:
		int32_t supplyUsed;
		int32_t supplyCap;
		uint32_t cntMarines;
		uint32_t cntMarauders;
		uint32_t cntMedivacs;
	};


public:
	ArmyTrainer_BioBallModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnStep();
	virtual void OnUnitIdle(const sc2::Unit* unit);

private:
	void OnBarracksIdle(Structure rax);
	void OnFactoryIdle(Structure fact);
	void OnStarportIdle(Structure port);

	const uint32_t requiredStepLoopCount = 200;

	CurrentBioArmyData GetCurrentData();
};
