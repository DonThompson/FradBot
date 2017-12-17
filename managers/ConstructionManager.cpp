#include "ConstructionManager.h"
#include "../utils/Utils.h"
#include "EconManager.h"

ConstructionManager::ConstructionManager(Bot & b)
	: ManagerBase(b)
	, nextBuildingId(0)
{

}

ConstructionManager::~ConstructionManager()
{

}

int64_t ConstructionManager::BuildStructure(ABILITY_ID structureAbilityId, BuildQueueTaskCallbackFunction callbackSuccess /*= nullptr*/, BuildQueueTaskCallbackFunction callbackFailure /*= nullptr*/)
{
	int64_t id = UseNextIdentifier();

	//Queue the request
	BuildQueueTask task(Observation()->GetGameLoop(), id, structureAbilityId, callbackSuccess, callbackFailure);
	mapBuildingQueue.insert(std::pair<int64_t, BuildQueueTask>(id, task));
	return id;
}

int64_t ConstructionManager::UseNextIdentifier()
{
	int64_t useThis = nextBuildingId;
	nextBuildingId++;
	return useThis;
}

void ConstructionManager::OnStep()
{
	//Process the events in the queue
	std::vector<int64_t> tasksToRemove;
	for(std::pair<int64_t, BuildQueueTask> pair : mapBuildingQueue) {
		int64_t taskId = pair.first;
		BuildQueueTask task = pair.second;
		switch (task.GetBuildingState())
		{
		case BuildingState::eQueued:
			HandledQueuedBuilding(task);
			break;
		case BuildingState::eFindingPosition:
			HandleFindingPosition(task);
			break;
		case BuildingState::eIssuingBuild:
			HandleIssuingBuild(task);
			break;
		case BuildingState::eConfirmOrders:
			HandleConfirmingOrders(task, tasksToRemove, taskId);
			break;
		case BuildingState::eWaitingOnBuildStart:
			HandleWaitingOnBuildStart(task);
			break;
		case BuildingState::eConstructionInProgress:
			HandleConstructionInProgress(task);
			break;
		case BuildingState::eInterrupted:
			//TODO:  Not sure how we get into this state.  Look for builder deaths?
			break;
		case BuildingState::eInterrupted_FindingNewWorker:
			break;
		case BuildingState::eInterrupted_Resuming:	//Jumps back to eConstructionInProgress
			break;
		case BuildingState::eCompleted:
			HandleCompleted(task, tasksToRemove, taskId);
			break;
		}

		//Update the queue with any changes made to our task by the current step
		mapBuildingQueue[taskId] = task;
	}

	//Cleanup any removed tasks
	for (int64_t removeMe : tasksToRemove) {
		mapBuildingQueue.erase(removeMe);
	}
}

//Pre:  Build task has been queued
//Post Success:  We found a unit that will perform the build task.
//Post Fail (repeat):  No suitable builder could be found.  Try again next step.
void ConstructionManager::HandledQueuedBuilding(BuildQueueTask &task)
{
	const Unit* builderUnit = Utils::GetRandomHarvester(Observation());
	if (builderUnit == nullptr) {
		//Can't find a worker.  Do nothing, we'll try again next step
	}
	else {
		task.AssignBuilder(builderUnit);
		//Move on to finding a position
		task.SetBuildingState(BuildingState::eFindingPosition);
	}
}

//Pre:  Builder assigned
//Post Success:  We have a position in which to build.  TODO:  This position may be invalid.
//Post Fail:  None at this time, always moves on to next step.
void ConstructionManager::HandleFindingPosition(BuildQueueTask &task)
{
	//SPECIAL!  If we're building a refinery, it needs a vespene structure, not a position
	if (task.GetBuildingType() == ABILITY_ID::BUILD_REFINERY) {
		const Unit* geyser = HandleFindingRefineryTarget(task.GetBuilder()->pos);
		task.SetGeyserTarget(geyser);
		task.SetBuildingState(BuildingState::eIssuingBuild);
	}
	else {

		//Find a random place to build
		//TODO:  Random is a poor way to place buildings
		float rx = sc2::GetRandomScalar();
		float ry = sc2::GetRandomScalar();
		const Unit* myBuilder = task.GetBuilder();
		if (myBuilder != nullptr) {
			Point2D buildPoint(myBuilder->pos.x + rx * 15.0f, myBuilder->pos.y + ry * 15.0f);
			task.SetBuildPoint(buildPoint);
			//Issue build command
			task.SetBuildingState(BuildingState::eIssuingBuild);
		}
		else {
			//TODO:  What if we lose our builder?  Would this even be null, or just some invalid pointer?
		}
	}
}

//Pre:  Builder assigned and position known.
//Post Success:  Unit command issued to perform a build.  This may or may not succeed, we do not know what happens.
//Post Fail:  None at this time, always moves on to next step.
void ConstructionManager::HandleIssuingBuild(BuildQueueTask &task)
{
	//Geysers require special handling
	if (task.GetBuildingType() == ABILITY_ID::BUILD_REFINERY) {
		Actions()->UnitCommand(task.GetBuilder(), task.GetBuildingType(), task.GetGeyserTarget());
	}
	else {
		//Issue the action to the unit via the command menu to build and where
		Actions()->UnitCommand(task.GetBuilder(), task.GetBuildingType(), task.GetBuildPoint());
	}

	//wait on build to start
	task.SetBuildingState(BuildingState::eConfirmOrders);
}

//Pre:  Builder has been issued a command
//Post Success:  Builder has accepted our orders
//Post Fail (cancel task):  Builder did not accept our orders.  Entire task is aborted.
void ConstructionManager::HandleConfirmingOrders(BuildQueueTask &task, std::vector<int64_t> &tasksToRemove, const int64_t taskId)
{
	//Did the builder accept our unit command?  Look for an order that isn't harvesting.
	const Unit* builder = task.GetBuilder();
	bool bFound = false;
	for (UnitOrder o : builder->orders) {
		if (o.ability_id != ABILITY_ID::HARVEST_GATHER && o.ability_id != ABILITY_ID::HARVEST_RETURN) {
			//TODO:  Breakpoint here a bit and see if there are any other cases we're missing?
			bFound = true;
		}
	}
	if (!bFound) {
		//Our orders to build were not found on the builder - many failure reasons possible such as invalid location, not enough resources to spend, etc.
		//	If this task has been in our queue too long, we'll abort it.

		//TODO:  Where should this logic be?  Does it make sense inside BuildQueueTask?  That's a bit of a POCO class.
		//TODO:  Here's some documentation to put ... somewhere.
		//	https://github.com/Blizzard/s2client-api/issues/164
		//	From parsing this, here's some rough ideas:  16 gameloops is used as a measurement of distance.  Presumably because this is about 1s in some speed (normal?)
		//	The blizz dev also says faster is 22.4 gameloops/second.  So we'll take a nice round value and say "20 loops per second" roughly.  If we don't get a command
		//	queued within 5 seconds, then kill it.
		const uint32_t maxGameLoopsBeforeAbort = 20 * 5;
		if (Observation()->GetGameLoop() - task.GetStartingGameLoop() > maxGameLoopsBeforeAbort) {
			//This builder failed to get our order in a reasonable time.  Abort the whole thing and remove the task from our build queue.
			if (task.GetFailureCallback() != nullptr) {
				//Call the success callback function provided
				std::invoke(task.GetFailureCallback(), taskId);
			}

			tasksToRemove.push_back(taskId);
		}
		else {
			//NEW STATE:  Our orders to build were not found on the builder - many failure reasons possible such as invalid location, not enough resources to spend, etc.
			//	However, to be safest, we're going to re-queue clear back to finding a builder.  Maybe that builder is stuck or has since moved on to something else.
			task.AssignBuilder(nullptr);
			//Requeue back like a new request
			task.SetBuildingState(BuildingState::eQueued);
		}
	}
	else {
		//Otherwise, the builder does have a non-gathering order in their queue.  We'll assume that's ours.
		//	TODO:  Some small risk of them being kidnapped elsewhere in between or the order not being ours.
		task.SetBuildingState(BuildingState::eWaitingOnBuildStart);
	}
}

//Pre:  Builder has accepted our orders
//Post Success:  Builder got the command and actually started the building - it's visible on the map.
//TODO:  Could we / should we go back a step?  That might ensure we're completing tasks rather than skipping them.
void ConstructionManager::HandleWaitingOnBuildStart(BuildQueueTask &task)
{
	//Now we know the builder has the order.
	//Only way to confirm it actually started building is to search all buildings, see which are being constructed, then see
	//	which one has a position that closely matches our suggested build position.  Note that positions are NOT identical.

	//TODO:  This is looking for supply depots.  Need to fix for other types of buildings.  How is this working at all?
	for (const Unit* buildingStarted : Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_SUPPLYDEPOT)) {
		if (buildingStarted->build_progress >= 0.9999f) {
			//Building is done, can't be what we're looking for.
			continue;
		}

		if (buildingStarted->build_progress < 0.0001f) {
			//Building is unstarted.  It might be ours, or it might be something else.  Wait until it's got a little bit of progress.
			continue;
		}

		//This building is being constructed.  Let's see where, and if it's within appropriate distance to our suggested build point, 
		//	it's very likely our building.
		//TODO:  Be nice to confirm this in some other fashion or confirm the distances.  Is it possible for this distance to be less
		//	then 1.0f and not be the same?  How can we test this?
		const float maxBuildingDistance = 1.0f;

		float distance = Distance2D(task.GetBuildPoint(), buildingStarted->pos);
		if (distance < maxBuildingDistance) {
			task.SetBuilding(buildingStarted);
			task.SetBuildingState(BuildingState::eConstructionInProgress);
		}
	}
}

//Pre:  Building is started, we found it, and we know it's being built.
//Post Success:  The building finished, progress is complete.
//Post Fail (repeat):  The building progress is less than complete.
//TODO:  Detection for interrupted state.  Lose worker?  How to test this?
void ConstructionManager::HandleConstructionInProgress(BuildQueueTask &task)
{
	if (task.GetBuilding()->build_progress >= 0.999999f) {
		//Building done!
		task.SetBuildingState(BuildingState::eCompleted);
	}
}

//Pre:  Building detected at 100% complete
//Post Success:  Building is completed, remove it from our queue.
void ConstructionManager::HandleCompleted(BuildQueueTask task, std::vector<int64_t> &tasksToRemove, const int64_t taskId)
{
	//Done.  Clean it up, no need to monitor it in our queue any longer.
	if (task.GetSuccessCallback() != nullptr) {
		//Call the success callback function provided
		std::invoke(task.GetSuccessCallback(), taskId);
	}
	tasksToRemove.push_back(taskId);
}

const Unit* ConstructionManager::HandleFindingRefineryTarget(Point2D builderPos)
{
	return EconManager::FindNearestVespeneGeyser(builderPos, Observation());
}