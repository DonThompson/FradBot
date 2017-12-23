#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "Utils.h"

class Bot;

class ArmyManager : public ManagerBase {
public:
	ArmyManager(Bot & b);
	virtual void OnStep();

	bool BarracksNeeded();
	void BuildBarracks();
	void TryAttackInGroups();
	void LaunchAttackGroup(Units unitsToAttack);
	void OnBarracksIdle(const Unit* unit);
	void OnMarineIdle(const Unit* unit);
	virtual void OnUnitIdle(const Unit* unit);
	void OnBarracksSuccess(int64_t taskId);
	void OnBarracksFailed(int64_t taskId);

private:
	clock_t lastBalanceClock;
	uint32_t raxInProgress;

};
