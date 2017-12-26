#ifndef __ECON_MANAGER_H
#define __ECON_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "Structure.h"

class Bot;

//Autonomy:
//	Enabled:  Builds workers if we have not saturated the minerals & gas fully.  Builds refineries if missing.  Shifts workers between the two.  And everything in disabled.
//	Disabled:  Handlers idle workers and puts them back to mining.
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

private:
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
