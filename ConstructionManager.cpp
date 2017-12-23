#include "ConstructionManager.h"
#include "managers/EconManager.h"
#include "ConstructionPlacement.h"
#include "bot.h"

ConstructionManager::ConstructionManager(Bot & b)
	: ManagerBase(b)
	, nextBuildingId(0)
{

}

uint64_t ConstructionManager::BuildStructure(ABILITY_ID structureAbilityId, BuildQueueTaskCallbackFunction callbackSuccess /*= nullptr*/, BuildQueueTaskCallbackFunction callbackFailure /*= nullptr*/)
{
	uint64_t id = UseNextIdentifier();

	//Queue the request and return
	BuildQueueTask task(Observation()->GetGameLoop(), id, structureAbilityId, callbackSuccess, callbackFailure);
	mapBuildingQueue.insert(std::pair<uint64_t, BuildQueueTask>(id, task));
	return id;
}

//Retrieves the next available unique task identifier, then moves the counter along.
uint64_t ConstructionManager::UseNextIdentifier()
{
	uint64_t useThis = nextBuildingId;
	nextBuildingId++;
	return useThis;
}

void ConstructionManager::OnStep()
{
	//Hold any tasks that need to be shut down as canceled
	std::vector<uint64_t> tasksToRemove;

	//Process the events in the queue
	for(std::pair<uint64_t, BuildQueueTask> pair : mapBuildingQueue)
	{
		uint64_t taskId = pair.first;
		BuildQueueTask task = pair.second;

		//Has this specific task been running for too long?  Once it's been in the queue too long, we'll
		//	have to abort it as something we can't handle right now.  Caller will have to try again.
		if(task.IsTaskLongRunning(Observation()->GetGameLoop()))
		{
			if (task.GetFailureCallback() != nullptr) {
				//Call the success callback function provided
				std::invoke(task.GetFailureCallback(), taskId);
			}

			tasksToRemove.push_back(taskId);
			//Move on to the next task
			continue;
		}

		switch (task.GetConstructionTaskState())
		{
		case ConstructionTaskState::eQueued:
			HandledQueuedBuilding(task);
			break;
		case ConstructionTaskState::eFindingPosition:
			HandleFindingPosition(task);
			break;
		case ConstructionTaskState::eIssuingBuild:
			HandleIssuingBuild(task);
			break;
		case ConstructionTaskState::eConfirmOrders:
			HandleConfirmingOrders(task, tasksToRemove, taskId);
			break;
		case ConstructionTaskState::eWaitingOnBuildStart:
			HandleWaitingOnBuildStart(task);
			break;
		case ConstructionTaskState::eConstructionInProgress:
			HandleConstructionInProgress(task);
			break;
		case ConstructionTaskState::eInterrupted:
			break;
		case ConstructionTaskState::eInterrupted_FindingNewWorker:
			break;
		case ConstructionTaskState::eInterrupted_Resuming:	//Jumps back to eConstructionInProgress
			break;
		case ConstructionTaskState::eCompleted:
			HandleCompleted(task, tasksToRemove, taskId);
			break;
		}

		//Update the queue with any changes made to our task by the current step
		mapBuildingQueue[taskId] = task;
	}

	//Cleanup any removed tasks
	for (uint64_t removeMe : tasksToRemove) {
		mapBuildingQueue.erase(removeMe);
	}
}

//Handles all logic around finding the available worker to perform construction.
const Unit* ConstructionManager::FindConstructionWorker()
{
	//We ONLY pull constructors from the list of currently harvesting workers, and only
	//	those that are gathering, not returning.  Other logic throughout the 
	//	construction manager uses these assumptions about harvest state.
	return Utils::GetRandomHarvester(Observation());
}

//Pre:  Build task has been queued
//Post Success:  We found a unit that will perform the build task.
//Post Fail (repeat):  No suitable builder could be found.  Try again next step.
void ConstructionManager::HandledQueuedBuilding(BuildQueueTask &task)
{
	const Unit* builderUnit = FindConstructionWorker();
	if (builderUnit == nullptr) {
		//Can't find a worker.  Do nothing, we'll try again next step
	}
	else {
		task.AssignBuilder(builderUnit);
		//Move on to finding a position
		task.SetConstructionTaskState(ConstructionTaskState::eFindingPosition);
	}
}

//Pre:  Builder assigned
//Post Success:  We have a position in which to build.  Note:  This position may be invalid.  If so it'll be caught and re-attempted in the queue.
//Post Fail:  None at this time, always moves on to next step.
void ConstructionManager::HandleFindingPosition(BuildQueueTask &task)
{
	//SPECIAL!  If we're building a refinery, it needs a vespene structure, not a position
	if (task.GetBuildingType() == ABILITY_ID::BUILD_REFINERY) {
		const Unit* geyser = HandleFindingRefineryTarget(task.GetBuilder()->pos);
		task.SetGeyserTarget(geyser);
	}
	else {
		ConstructionPlacement p;
		Point2D buildPoint = p.GetBuildPoint(task.GetBuilder());
		task.SetBuildPoint(buildPoint);
	}
	//Issue build command
	task.SetConstructionTaskState(ConstructionTaskState::eIssuingBuild);
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
	task.SetConstructionTaskState(ConstructionTaskState::eConfirmOrders);
}

//Pre:  Builder has been issued a command
//Post Success:  Builder has accepted our orders
//Post Fail (cancel task):  Builder did not accept our orders.  Entire task is aborted.
void ConstructionManager::HandleConfirmingOrders(BuildQueueTask &task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId)
{
	//Did the builder accept our unit command?  Look for an order that isn't harvesting.
	//	Note:  This is predicated around the assumption that we only pull constructors from harvesters
	if (DoesBuilderHaveNonHarvestOrders(task.GetBuilder()))	{
		//The builder does have a non-gathering order in their queue.  We'll assume that's ours.  There is some small risk here
		//	that it's not our order.
		task.SetConstructionTaskState(ConstructionTaskState::eWaitingOnBuildStart);
	}
	else {
		//Our orders to build were not found on the builder - many failure reasons possible such as invalid location, not enough resources to spend, etc.
		//	However, to be safest, we're going to re-queue clear back to finding a builder.  Maybe that builder is stuck or has since moved on to something else.
		task.AssignBuilder(nullptr);
		//Requeue back like a new request
		task.SetConstructionTaskState(ConstructionTaskState::eQueued);
	}
}

//Pre:  Builder has accepted our orders
//Post Success:  Builder got the command and actually started the building - it's visible on the map.
void ConstructionManager::HandleWaitingOnBuildStart(BuildQueueTask &task)
{
	//Now we know the builder has the order.
	//Only way to confirm it actually started building is to search all buildings, see which are being constructed, then see
	//	which one has a position that closely matches our suggested build position.  Note that positions are NOT identical.
	std::vector<Structure> structures = bot.Structures().GetStructuresByBuildAbility(task.GetBuildingType());
	for (const Unit* buildingStarted : structures) {

		if (!IsBuildingInProgress(buildingStarted)) {
			//Building is either unstarted (might be us, we'll check again next loop), or done.  We want in progress.
			continue;
		}

		if (task.GetBuildingType() == ABILITY_ID::BUILD_REFINERY) {
			//SPECIAL!  Referineries won't match on position of target -- use the geyser position instead
			if (DoBuildingPositionsMatch(task.GetGeyserTarget()->pos, buildingStarted->pos)) {
				task.SetBuilding(buildingStarted);
				task.SetConstructionTaskState(ConstructionTaskState::eConstructionInProgress);
				return;
			}
		}
		else {
			//This building is being constructed.  Let's see where, and if it's within appropriate distance to our suggested build point, 
			//	it's very likely our building.
			if (DoBuildingPositionsMatch(task.GetBuildPoint(), buildingStarted->pos)) {
				task.SetBuilding(buildingStarted);
				task.SetConstructionTaskState(ConstructionTaskState::eConstructionInProgress);
				return;
			}
		}
	}
}

//Pre:  Building is started, we found it, and we know it's being built.
//Post Success:  The building finished, progress is complete.
//Post Fail (repeat):  The building progress is less than complete.
void ConstructionManager::HandleConstructionInProgress(BuildQueueTask &task)
{
	if (task.GetBuilding()->build_progress >= 0.999999f) {
		//Building done!
		task.SetConstructionTaskState(ConstructionTaskState::eCompleted);
	}
}

//Pre:  Building detected at 100% complete
//Post Success:  Building is completed, remove it from our queue.
void ConstructionManager::HandleCompleted(BuildQueueTask task, std::vector<uint64_t> &tasksToRemove, const uint64_t taskId)
{
	//Done.  Clean it up, no need to monitor it in our queue any longer.
	if (task.GetSuccessCallback() != nullptr) {
		//Call the success callback function provided
		std::invoke(task.GetSuccessCallback(), taskId);
	}
	tasksToRemove.push_back(taskId);
}

//Find the appropriate vespene geyser when we're trying to build a refinery
const Unit* ConstructionManager::HandleFindingRefineryTarget(Point2D builderPos)
{
	return EconManager::FindNearestVespeneGeyser(builderPos, Observation());
}

//Examine the given builder's orders and see if there are any other than harvesting.
//Future:  This could be moved to a builder class once we get there.
bool ConstructionManager::DoesBuilderHaveNonHarvestOrders(const Unit* builder)
{
	bool found = false;
	for (UnitOrder o : builder->orders) {
		if (o.ability_id != ABILITY_ID::HARVEST_GATHER && o.ability_id != ABILITY_ID::HARVEST_RETURN) {
			found = true;
		}
	}

	return found;
}

//Is the given building in progress?  Returns false if the building is unstarted (0% completion) or done (100% completion)
//Future:  This could be moved to a building class once we get there.
bool ConstructionManager::IsBuildingInProgress(const Unit* building)
{
	if (building->build_progress >= 0.9999f) {
		//Building is done, can't be what we're looking for.
		return false;
	}

	if (building->build_progress < 0.0001f) {
		//Building is unstarted.  It might be ours, or it might be something else.  Wait until it's got a little bit of progress.
		return false;
	}

	return true;
}

//There is no good way to get a building from the builder.  This function allows us to match a given position where we asked to build
//	with a building's actual position.  If they're close, we assume them to match.
bool ConstructionManager::DoBuildingPositionsMatch(Point2D pt1, Point2D pt2)
{
	const float maxBuildingDistance = 1.0f;

	float distance = Distance2D(pt1, pt2);
	if (distance < maxBuildingDistance) {
		return true;
	}

	return false;
}
