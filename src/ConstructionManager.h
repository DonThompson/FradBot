#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "Utils.h"
#include "ConstructionQueueTask.h"
#include "BaseLocation.h"

class Bot;

/* Usage
	ConstructionManager currently will attempt to create your building, but will abort the task for any invalid build
	attempts (not enough resources, invalid position, etc.).  If this happens, it will never re-attempt.  caller must do so.

	TODO Progress:
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
	- TODO:  Resource reserve might be something we can farm to its own class.  Is there a better way to integrate it with this class?  It has a code smell - feels easily breakable.  Had to find just the right places to copy/paste the function call.  Maybe every step in the process auto updates the resource reservation system?  Then it can handle when to remove and when not.
	[DONE]- Logging/status:  Output status of the queue every few seconds.

*/

class ConstructionManager : public ManagerBase
{
public:
	explicit ConstructionManager(Bot & b);

	//Use to request that the construction manager construct a building.  Callback functions optional.  The construction manager will find a worker and a position
	//	for the building to be constructed.
	//structureAbilityId - Structure to be constructed.
	//callbackSuccess - Function to call back on success of the construction
	//callbackFailure - Function to call back on failure of the construction.
	uint64_t BuildStructure(sc2::ABILITY_ID structureAbilityId, ConstructionQueueTaskCallbackFunction callbackSuccess = nullptr, ConstructionQueueTaskCallbackFunction callbackFailure = nullptr);
	uint64_t Expand(BaseLocation expandingToLocation, ConstructionQueueTaskCallbackFunction callbackSuccess = nullptr, ConstructionQueueTaskCallbackFunction callbackFailure = nullptr);

	uint32_t GetReservedMinerals();
	uint32_t GetReservedVespene();

	//Called for each step of the game.  Not for public consumption.
	virtual void OnStep();
	virtual std::string GetDebugSummaryString();

private:
	uint64_t nextBuildingId;
	std::map<uint64_t, ConstructionQueueTask> mapBuildingQueue;
	uint64_t UseNextIdentifier();
	uint32_t reservedMinerals;
	uint32_t reservedVespene;


	//Queue management functions
	void HandledQueuedBuilding(ConstructionQueueTask &task);
	void HandleFindingPosition(ConstructionQueueTask &task);
	void HandleIssuingBuild(ConstructionQueueTask &task);
	void HandleConfirmingOrders(ConstructionQueueTask &task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId);
	void HandleWaitingOnBuildStart(ConstructionQueueTask &task);
	void HandleConstructionInProgress(ConstructionQueueTask &task);
	void HandleCompleted(ConstructionQueueTask task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId);

	const sc2::Unit* FindConstructionWorker();
	const sc2::Unit* HandleFindingRefineryTarget(sc2::Point2D builderPos);
	bool DoesBuilderHaveNonHarvestOrders(const sc2::Unit* builder);
	bool DoBuildingPositionsMatch(sc2::Point2D pt1, sc2::Point2D pt2);
	void RemoveResourceReserve(ConstructionQueueTask  &task);
};
