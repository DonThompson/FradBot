#ifndef __ECON_MANAGER_H
#define __ECON_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "Structure.h"

class Bot;
class ModuleBase;
class VespeneWorkerBalanceModule;
class IdleWorkerModule;
class AutoBuildWorkersModule;

//Behaviors
//	Always
//		* VespeneWorkerBalanceModule
//		* Idle workers moved to nearest minerals 

//	Autonomy Enabled



//	Enabled:  Builds workers if we have not saturated the minerals & gas fully.  Builds refineries if missing.  Shifts workers between the two.  And everything in disabled.
//	Disabled:  Handles idle workers and puts them back to mining.
class EconManager : public ManagerBase {
private:
	clock_t lastBalanceClock;

public:
	explicit EconManager(Bot & b);
	virtual void OnStep();

	void EnableAutoBuildWorkersModule();

	void OnRefinerySuccess(int64_t taskId);
	void OnRefineryFailed(int64_t taskId);

	static const sc2::Unit* FindNearestVespeneGeyser(const sc2::Point2D& start, const sc2::ObservationInterface* obs);

	//Train a worker.  Optionally provide a source to build from.  If not provided, one will be found from the most suitable base.
	bool TrainWorker(Structure* buildFrom = nullptr);

private:
	bool NeedRefinery();
	int32_t GetRefineryCount();
	void BuildRefinery();

	uint32_t refineriesInProgress;
	uint32_t refineriesCompleted;
};

#endif //__ECON_MANAGER_H
