#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "Platoon.h"

class Bot;

//Autonomy:
//Enabled:  Builds barracks.  Trains units automatically.
//Disabled:  Sends attacks.
class ArmyManager : public ManagerBase {
public:
	static bool IsMilitaryUnit(const sc2::Unit* unit);

public:
	ArmyManager(Bot & b);
	virtual void OnStep();
	virtual void OnUnitCreated(const sc2::Unit* unit);
	virtual void OnUnitDestroyed(const sc2::Unit* unit);

	bool BarracksNeeded();
	void BuildBarracks();
	void TryAttackInGroups();
	void LaunchAttackGroup(sc2::Units unitsToAttack);
	void OnBarracksIdle(const sc2::Unit* unit);
	virtual void OnUnitIdle(const sc2::Unit* unit);
	void OnBarracksSuccess(int64_t taskId);
	void OnBarracksFailed(int64_t taskId);
	bool TrainUnit(sc2::ABILITY_ID abilityID);

	void AddUnitToPlatoon(const sc2::Unit* unit);

private:
	clock_t lastBalanceClock;
	uint32_t raxInProgress;

	//Unit management
	std::vector<Platoon> armyPlatoons;

};
