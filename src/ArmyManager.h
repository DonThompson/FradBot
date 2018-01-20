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
	explicit ArmyManager(Bot & b);
	virtual void OnStep();
	virtual void OnUnitCreated(const sc2::Unit* unit);
	virtual void OnUnitDestroyed(const sc2::Unit* unit);
	virtual std::string GetDebugSummaryString();

	bool BarracksNeeded();
	void BuildBarracks();
	void TryAttackInGroups();
	void LaunchAttackGroup(sc2::Units unitsToAttack);
	void OnBarracksIdle(const sc2::Unit* unit);
	virtual void OnUnitIdle(const sc2::Unit* unit);
	void OnBarracksSuccess(int64_t taskId);
	void OnBarracksFailed(int64_t taskId);
	bool TrainUnit(sc2::ABILITY_ID abilityID);
	bool TrainUnitFromBuilding(sc2::ABILITY_ID abilityID, const sc2::Unit* building);
	void AddUnitToPlatoon(const sc2::Unit* unit);
	size_t GetTotalArmyUnitCount();

public:
	//Unit management
	//TODO:  Made public when I ripped out the attack/defend logic.  Not ideal.
	std::vector<std::shared_ptr<Platoon>> armyPlatoons;

private:
	clock_t lastBalanceClock;
	uint32_t raxInProgress;

	//TODO:  Should the strategy manage this?
	//TODO:  enum?
	uint32_t currentStrategy;

};
