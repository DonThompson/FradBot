#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"

class Bot;

//Autonomy:
//Enabled:  Builds barracks.  Trains units automatically.
//Disabled:  Sends attacks.
class ArmyManager : public ManagerBase {
public:
	ArmyManager(Bot & b);
	virtual void OnStep();

	bool BarracksNeeded();
	void BuildBarracks();
	void TryAttackInGroups();
	void LaunchAttackGroup(sc2::Units unitsToAttack);
	void OnBarracksIdle(const sc2::Unit* unit);
	virtual void OnUnitIdle(const sc2::Unit* unit);
	void OnBarracksSuccess(int64_t taskId);
	void OnBarracksFailed(int64_t taskId);

private:
	clock_t lastBalanceClock;
	uint32_t raxInProgress;

};
