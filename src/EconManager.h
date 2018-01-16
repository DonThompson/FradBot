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

//	Autonomy Enabled
//	Enabled:  Builds refineries if missing.  And everything in disabled.
//	Disabled:  Does nothing
class EconManager : public ManagerBase {
private:
	clock_t lastBalanceClock;

public:
	explicit EconManager(Bot & b);
	virtual void OnStep();

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
