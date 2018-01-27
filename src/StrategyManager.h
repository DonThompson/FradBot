#pragma once
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BuildQueueEnums.h"
class Bot;

class StrategyManager : public ManagerBase
{
public:
	explicit StrategyManager(Bot & b);

	virtual void OnGameStart();
	virtual void OnStep();

private:
	void AddAbilityToBuildOrder(sc2::ABILITY_ID abilityID);
	void AddModuleToBuildOrder(MODULE_ID moduleID);

	//Various build orders
	void BO_RFE_AllRaceMatchup_BioBall();
	void BO_ExpandFirstIntoBioBall();
	void BO_SimpleBio();
};
