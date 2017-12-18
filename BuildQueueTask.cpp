#include "BuildQueueTask.h"


//Base constructor, builds a queue task with no values.  Not recommended for use.
BuildQueueTask::BuildQueueTask()
{
	//Just use the expected constructor with some predefined values
	BuildQueueTask(0, 0, ABILITY_ID::INVALID, nullptr, nullptr);
}

//Common default constructor, use this one.
BuildQueueTask::BuildQueueTask(uint32_t _gameLoop, int64_t _id, ABILITY_ID _structure, BuildQueueTaskCallbackFunction _successFn, BuildQueueTaskCallbackFunction _failFn)
	: startingGameLoop(_gameLoop)
	, id(_id)
	, state(ConstructionTaskState::eQueued)
	, structureToBuild(_structure)
	, builderUnit(nullptr)
	, buildingPoint()
	, geyserTarget(nullptr)
	, building(nullptr)
	, callbackSuccess(_successFn)
	, callbackFailure(_failFn)
{
}

uint32_t BuildQueueTask::GetStartingGameLoop()
{
	return startingGameLoop;
}

ConstructionTaskState BuildQueueTask::GetConstructionTaskState()
{
	return state;
}

const Unit* BuildQueueTask::GetBuilder()
{
	return builderUnit;
}

Point2D BuildQueueTask::GetBuildPoint()
{
	return buildingPoint;
}

const Unit* BuildQueueTask::GetGeyserTarget()
{
	return geyserTarget;
}

ABILITY_ID BuildQueueTask::GetBuildingType()
{
	return structureToBuild;
}

const Unit* BuildQueueTask::GetBuilding()
{
	return building;
}

BuildQueueTaskCallbackFunction BuildQueueTask::GetSuccessCallback()
{
	return callbackSuccess;
}

BuildQueueTaskCallbackFunction BuildQueueTask::GetFailureCallback()
{
	return callbackFailure;
}

void BuildQueueTask::SetConstructionTaskState(ConstructionTaskState newState)
{
	state = newState;
}

void BuildQueueTask::AssignBuilder(const Unit* builder)
{
	builderUnit = builder;
}

void BuildQueueTask::SetBuildPoint(Point2D _pt)
{
	buildingPoint = _pt;
}

void BuildQueueTask::SetGeyserTarget(const Unit* _geyser)
{
	geyserTarget = _geyser;
}

void BuildQueueTask::SetBuilding(const Unit* _building)
{
	building = _building;
}

void BuildQueueTask::SetCallbackOnSuccess(BuildQueueTaskCallbackFunction fn)
{
	callbackSuccess = fn;
}

void BuildQueueTask::SetCallbackOnFailure(BuildQueueTaskCallbackFunction fn)
{
	callbackFailure = fn;
}

bool BuildQueueTask::IsTaskLongRunning(uint32_t currentGameLoop)
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
