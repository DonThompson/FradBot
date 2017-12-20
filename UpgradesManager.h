#pragma once
#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "managers/ManagerBase.h"
#include "utils/Utils.h"
#include "UpgradeList.h"

class Bot;

class UpgradesManager : public ManagerBase {
public:
	UpgradesManager(Bot & b);

	virtual void OnStep();
	virtual void OnUpgradeCompleted(UpgradeID upgradeID);

private:
	clock_t lastBalanceClock;

	void BuildEngBaysAsNeeded();
	void OnEngBaySuccess(int64_t taskId);
	void OnEngBayFailed(int64_t taskId);
	void TryToUpgrade();
	UpgradeList upgrades;
	uint32_t countEngBays;
	uint32_t countEngBaysInProgress;
	void PerformEngBayUpgrades();
	void PerformTechLabUpgrades();


};

