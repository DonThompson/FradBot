#ifndef __SUPPLY_MANAGER_H
#define __SUPPLY_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"

class Bot;

class SupplyManager : public ManagerBase
{
public:
	SupplyManager(Bot & b);
	~SupplyManager();
	virtual void OnStep();

private:
	bool SupplyDepotNeeded();
	int32_t PredictSupplyDepotsNeeded();
	int32_t CalculateSupplyCurrentlyBeingProduced();
	int32_t GetUnitSupplyActivelyProducing(UnitOrder order);
	void BuildSupplyDepot();

	uint32_t depotsInProgress;

	void SupplyManager::successTest(int64_t x);
	void SupplyManager::failTest(int64_t y);

private:
	clock_t lastBalanceClock;

};

#endif //__SUPPLY_MANAGER_H
