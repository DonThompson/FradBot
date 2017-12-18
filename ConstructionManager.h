#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "managers/ManagerBase.h"
#include "utils/Utils.h"
#include "BuildQueueTask.h"

class Bot;

/* Usage
	ConstructionManager currently will attempt to create your building, but will abort the task for any invalid build
	attempts (not enough resources, invalid position, etc.).  If this happens, it will never re-attempt.  caller must do so.

	TODO:  callback event on success
	callback on fail
	never lose a request
	Logging/status:  Output status of the queue every few seconds.

	Progress:
	[DONE] - callback functions (done.  tested as null)
	[DONE] - test callbacks non-null
	[DONE] - fix the step to go backwards instead of aborting.  remove this failure callback.  Note:  Current code will probably then be queued to build hundreds of depots.
	[DONE] - put in some kind of counter - can't bounce around forever, it needs to eventually time out
	[DONE]- callback on that cancel
	[DONE]- test that case somehow
	[DONE]- go fix econ (refineries), supply (depots), and army (rax) managers to not spam so many commands and to implement callbacks
	- logging work
	- document, document, document.
	[DONE]- refactor and cleanup
	[DONE]- rename to construction manager?  ongoing confusion between "constructing buildings" manager and manager of "existing buildings"  construction/structure instead?  bot.Construction() and bot.Structures().  I am liking these.
	- TODO:  maybe put a cap on how many items in the queue get processed?  the game really starts to slow down at least at 100, and at 300 it's barely moving.  safety check.
*/

class ConstructionManager : public ManagerBase
{
public:
	ConstructionManager(Bot & b);

	//Use to request that the construction manager construct a building.  Callback functions optional.  The construction manager will find a worker and a position
	//	for the building to be constructed.
	//structureAbilityId - Structure to be constructed.
	//callbackSuccess - Function to call back on success of the construction
	//callbackFailure - Function to call back on failure of the construction.
	uint64_t BuildStructure(ABILITY_ID structureAbilityId, BuildQueueTaskCallbackFunction callbackSuccess = nullptr, BuildQueueTaskCallbackFunction callbackFailure = nullptr);

	//Called for each step of the game.  Not for public consumption.
	virtual void OnStep();

private:
	uint64_t nextBuildingId;
	std::map<uint64_t, BuildQueueTask> mapBuildingQueue;
	uint64_t UseNextIdentifier();


	//Queue management functions
	void HandledQueuedBuilding(BuildQueueTask &task);
	void HandleFindingPosition(BuildQueueTask &task);
	void HandleIssuingBuild(BuildQueueTask &task);
	void HandleConfirmingOrders(BuildQueueTask &task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId);
	void HandleWaitingOnBuildStart(BuildQueueTask &task);
	void HandleConstructionInProgress(BuildQueueTask &task);
	void HandleCompleted(BuildQueueTask task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId);

	const Unit* FindConstructionWorker();
	const Unit* HandleFindingRefineryTarget(Point2D builderPos);
	bool DoesBuilderHaveNonHarvestOrders(const Unit* builder);
	bool IsBuildingInProgress(const Unit* building);
	bool DoBuildingPositionsMatch(Point2D pt1, Point2D pt2);
	
};
