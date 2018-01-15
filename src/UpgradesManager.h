#pragma once
#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "UpgradeList.h"

class Bot;

//Autonomy:
//Enabled:  Gets upgrades in a pre-defined manner automatically when resources allow.  Automatically builds an engineering bay.
//Disabled:  Does nothing
class UpgradesManager : public ManagerBase {
public:
	//Is the given ability an upgrade?
	static bool IsUpgrade(sc2::ABILITY_ID abilityID);
	//Gets the type of building that produces this upgrade
	static sc2::UNIT_TYPEID GetUpgradeProducerType(sc2::ABILITY_ID abilityID);

public:
	explicit UpgradesManager(Bot & b);

	virtual void OnStep();
	virtual void OnUpgradeCompleted(sc2::UpgradeID upgradeID);

	bool PerformUpgrade(sc2::ABILITY_ID abilityID);

private:
	clock_t lastBalanceClock;

	void BuildEngBaysAsNeeded();
	void OnEngBaySuccess(int64_t taskId);
	void OnEngBayFailed(int64_t taskId);
	void TryToUpgrade();
	//TODO:  Temporarily used, only covers infantry upgrades
	UpgradeList upgrades;
	uint32_t countEngBays;
	uint32_t countEngBaysInProgress;
	void PerformEngBayUpgrades();
	void PerformTechLabUpgrades();
};
