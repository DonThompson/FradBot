#ifndef __ECON_MANAGER_H
#define __ECON_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
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
	virtual void OnUnitIdle(const Unit* unit);

	void OnRefinerySuccess(int64_t taskId);
	void OnRefineryFailed(int64_t taskId);

	static const Unit* FindNearestVespeneGeyser(const Point2D& start, const ObservationInterface* obs);

private:
	void BalanceBuilders();
	void OnCommandCenterIdle(const Unit* unit);
	bool NeedRefinery();
	int32_t GetRefineryCount();
	void BuildRefinery();
	
	const Unit* FindNearestMineralPatch(const Point2D& start);
	void HandleCommandCenterIdle(Structure cc);

	uint32_t refineriesInProgress;
	uint32_t refineriesCompleted;

};

#endif //__ECON_MANAGER_H
