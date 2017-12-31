#ifndef __SUPPLY_MANAGER_H
#define __SUPPLY_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"

class Bot;

//Autonomy:
//Enabled:  Attempts to predict supply needs and build depots in time to stay ahead.
//Disabled:  Does absolutely nothing.
class SupplyManager : public ManagerBase
{
public:
	SupplyManager(Bot & b);
	~SupplyManager();
	virtual void OnStep();

private:
	bool SupplyDepotNeeded();
	uint32_t PredictSupplyDepotsNeeded();
	uint32_t CalculateSupplyCurrentlyBeingProduced();
	uint32_t GetUnitSupplyActivelyProducing(sc2::UnitOrder order);
	void BuildSupplyDepot();

	uint32_t depotsInProgress;

	void SupplyManager::successTest(int64_t x);
	void SupplyManager::failTest(int64_t y);

private:
	clock_t lastBalanceClock;

};

#endif //__SUPPLY_MANAGER_H
