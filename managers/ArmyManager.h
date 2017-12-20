#ifndef __ARMY_MANAGER_H
#define __ARMY_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"

class Bot;

class UpgradeList {
public:
	bool hasConcussionGrenades;
	bool hasStim;
	bool hasCombatShields;
	int infAttackLevel;
	int infDefenseLevel;

	UpgradeList::UpgradeList()
		: hasConcussionGrenades(false)
		, hasStim(false)
		, hasCombatShields(false)
		, infAttackLevel(0)
		, infDefenseLevel(0)
	{

	}

	bool UpgradeList::NeedUpgrades()
	{
		//TODO:  Stop returning true when we are done.
		//TODO:  take supply/mins/gas to determine?
		return true;
	}

};

class ArmyManager : public ManagerBase {
public:
	ArmyManager(Bot & b);
	~ArmyManager();
	virtual void OnStep();
	virtual void OnUpgradeCompleted(UpgradeID upgradeID);

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

	//Upgrade hacks.  Cleanup later.
	void BuildEngBaysAsNeeded();
	void OnEngBaySuccess(int64_t taskId);
	void OnEngBayFailed(int64_t taskId);
	void TryToUpgrade();
	UpgradeList upgrades;
	uint32_t countEngBays;
	uint32_t countEngBaysInProgress;
	//end

};

#endif //__ARMY_MANAGER_H
