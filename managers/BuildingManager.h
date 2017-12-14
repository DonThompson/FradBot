#pragma once

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
	Logging/status:  Output status of the queue every few seconds.

	Progress:
	- callback functions (done.  tested as null)
	- test callbacks non-null
	- fix the step to go backwards instead of aborting.  remove this failure callback.
	- put in some kind of counter - can't bounce around forever, it needs to eventually time out
	- callback on that cancel
	- test that case somehow
	- go fix econ (refineries), supply (depots), and army (rax) managers to not spam so many commands and to implement callbacks
	- logging work
	- document, document, document.
	- refactor and cleanup
	- this should put buildmanager in a good place going forward for a while
*/

class BuildingManager : public ManagerBase
{
public:
	BuildingManager(Bot & b);
	~BuildingManager();

	int64_t BuildStructure(ABILITY_ID ability_type_for_structure, BuildQueueTaskCallbackFunction callbackSuccess = nullptr, BuildQueueTaskCallbackFunction callbackFailure = nullptr);

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
	void HandleCompleted(BuildQueueTask task, std::vector<int64_t> &tasksToRemove, const int64_t taskId);

};
