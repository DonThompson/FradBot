#ifndef __ECON_MANAGER_H
#define __ECON_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"

class EconManager : public ManagerBase {
private:
	clock_t lastBalanceClock;

public:
	EconManager();
	~EconManager();
	virtual void OnStep();
	virtual void OnUnitIdle(const Unit* unit);

private:
	void BalanceBuilders();
	void OnCommandCenterIdle(const Unit* unit);
	bool NeedRefinery();
	int32_t GetRefineryCount();
	bool TryBuildRefinery();
	const Unit* FindNearestVespeneGeyser(const Point2D& start);
	const Unit* FindNearestMineralPatch(const Point2D& start);

};

#endif //__ECON_MANAGER_H
