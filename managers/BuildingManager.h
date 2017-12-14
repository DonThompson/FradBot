#ifndef __BUILDING_MANAGER_H
#define __BUILDING_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"
#include "../BuildQueueTask.h"

class Bot;

/* Usage
	BuildingManager currently will attempt to create your building, but will abort the task for any invalid build
	attempts (not enough resources, invalid position, etc.).  If this happens, it will never re-attempt.  caller must do so.

	TODO:  callback event on success
	callback on fail
	never lose a request
*/

class BuildingManager : public ManagerBase
{
public:
	BuildingManager(Bot & b);
	~BuildingManager();

	int64_t BuildStructure(ABILITY_ID ability_type_for_structure);

	virtual void OnStep();

private:
	int64_t nextBuildingId;
	std::map<int64_t, BuildQueueTask> mapBuildingQueue;
	int64_t UseNextIdentifier();


	//Queue management functions
	void HandledQueuedBuilding(BuildQueueTask &task);
	void HandleFindingPosition(BuildQueueTask &task);
	void HandleIssuingBuild(BuildQueueTask &task);
	void HandleConfirmingOrders(BuildQueueTask &task, std::vector<int64_t> &tasksToRemove, const int64_t taskId);
	void HandleWaitingOnBuildStart(BuildQueueTask &task);
	void HandleConstructionInProgress(BuildQueueTask &task);
	void HandleCompleted(std::vector<int64_t> &tasksToRemove, const int64_t taskId);

};

#endif //__BUILDING_MANAGER_H
