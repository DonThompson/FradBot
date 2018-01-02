#include "ConstructionQueueTask.h"
using namespace sc2;

//Base constructor, builds a queue task with no values.  Not recommended for use.
ConstructionQueueTask::ConstructionQueueTask()
{
	//Just use the expected constructor with some predefined values
	ConstructionQueueTask(0, 0, ABILITY_ID::INVALID, nullptr, nullptr);
}

//Common default constructor, use this one.
ConstructionQueueTask::ConstructionQueueTask(uint32_t _gameLoop, int64_t _id, ABILITY_ID _structure, ConstructionQueueTaskCallbackFunction _successFn, ConstructionQueueTaskCallbackFunction _failFn)
	: startingGameLoop(_gameLoop)
	, id(_id)
	, state(ConstructionTaskState::eQueued)
	, structureToBuild(_structure)
	, isReservingResources(true)
	, builderUnit(nullptr)
	, buildingPoint()
	, building(nullptr)
	, geyserTarget(nullptr)
	, callbackSuccess(_successFn)
	, callbackFailure(_failFn)
{
}

uint32_t ConstructionQueueTask::GetStartingGameLoop()
{
	return startingGameLoop;
}

ConstructionTaskState ConstructionQueueTask::GetConstructionTaskState()
{
	return state;
}

const Unit* ConstructionQueueTask::GetBuilder()
{
	return builderUnit;
}

Point2D ConstructionQueueTask::GetBuildPoint()
{
	return buildingPoint;
}

const Unit* ConstructionQueueTask::GetGeyserTarget()
{
	return geyserTarget;
}

ABILITY_ID ConstructionQueueTask::GetBuildingType()
{
	return structureToBuild;
}

bool ConstructionQueueTask::IsReservingResources()
{
	return isReservingResources;
}

Structure ConstructionQueueTask::GetBuilding()
{
	return building;
}

ConstructionQueueTaskCallbackFunction ConstructionQueueTask::GetSuccessCallback()
{
	return callbackSuccess;
}

ConstructionQueueTaskCallbackFunction ConstructionQueueTask::GetFailureCallback()
{
	return callbackFailure;
}

void ConstructionQueueTask::SetConstructionTaskState(ConstructionTaskState newState)
{
	state = newState;
}

void ConstructionQueueTask::AssignBuilder(const Unit* builder)
{
	builderUnit = builder;
}

void ConstructionQueueTask::SetBuildPoint(Point2D _pt)
{
	buildingPoint = _pt;
}

void ConstructionQueueTask::SetGeyserTarget(const Unit* _geyser)
{
	geyserTarget = _geyser;
}

void ConstructionQueueTask::SetBuilding(Structure _building)
{
	building = _building;
}

void ConstructionQueueTask::SetCallbackOnSuccess(ConstructionQueueTaskCallbackFunction fn)
{
	callbackSuccess = fn;
}

void ConstructionQueueTask::SetCallbackOnFailure(ConstructionQueueTaskCallbackFunction fn)
{
	callbackFailure = fn;
}

void ConstructionQueueTask::StopReservingResources()
{
	isReservingResources = false;
}

bool ConstructionQueueTask::IsTaskLongRunning(uint32_t currentGameLoop)
{
	//TODO:  Where should this logic be?  Does it make sense inside BuildQueueTask?  That's a bit of a POCO class.
	//TODO:  Here's some documentation to put ... somewhere.
	//	https://github.com/Blizzard/s2client-api/issues/164
	//	From parsing this, here's some rough ideas:  16 gameloops is used as a measurement of distance.  Presumably because this is about 1s in some speed (normal?)
	//	The blizz dev also says faster is 22.4 gameloops/second.  So we'll take a nice round value and say "20 loops per second" roughly.  If we don't get a command
	//	queued within 5 seconds, then kill it.
	const uint32_t maxGameLoopsBeforeAbort = 20 * 5;
	if (state == ConstructionTaskState::eWaitingOnBuildStart || state == ConstructionTaskState::eConstructionInProgress || state == ConstructionTaskState::eCompleted) {
		//Ignore anything that reaches the 'construction in progress' state, which will obviously not complete this fast to actually build.
		//	I also decided to keep 'waiting on build start' here.  I've seen a live case where a worker was sent a long distance, such that the orders were 
		//	relayed to the builder, he ran off, but it was so far that the max loops was hit before he started.  The construction manager reported failure
		//	to queue... but that builder still built his thing anyways.
		return false;
	}
	if (currentGameLoop - GetStartingGameLoop() > maxGameLoopsBeforeAbort ) {
		//This builder failed to get our order in a reasonable time.  Abort the whole thing and remove the task from our build queue.
		return true;
	}
	return false;
}
