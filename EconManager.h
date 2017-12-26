#ifndef __ECON_MANAGER_H
#define __ECON_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "Structure.h"

class Bot;

class EconManager : public ManagerBase {
private:
	clock_t lastBalanceClock;

public:
	EconManager(Bot & b);
	virtual void OnStep();
	virtual void OnUnitIdle(const sc2::Unit* unit);

	void OnRefinerySuccess(int64_t taskId);
	void OnRefineryFailed(int64_t taskId);

	static const sc2::Unit* FindNearestVespeneGeyser(const sc2::Point2D& start, const sc2::ObservationInterface* obs);

	//Enables the economy manager to act autonomously.  In this mode, it will attempt to smartly optimize workers, harvesting
	//	activities, vespene, etc.
	void EnableAutonomy();
	//Disables the economy manager to act autonomously.  In this mode, it will not build anything, but will still manage
	//	idle workers and attempt to optimize.
	void DisableAutonomy();

private:
	bool actAutonomously;
	void BalanceBuilders();
	void OnCommandCenterIdle(const sc2::Unit* unit);
	bool NeedRefinery();
	int32_t GetRefineryCount();
	void BuildRefinery();
	
	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start);
	void HandleCommandCenterIdle(Structure cc);

	uint32_t refineriesInProgress;
	uint32_t refineriesCompleted;

};

#endif //__ECON_MANAGER_H
