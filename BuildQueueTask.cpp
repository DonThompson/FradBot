#include "BuildQueueTask.h"


//Base constructor, builds a queue task with no values.
BuildQueueTask::BuildQueueTask()
	: id(0)
	, state(BuildingState::eQueued)
	, structureToBuild(ABILITY_ID::INVALID)
	, builderUnit(nullptr)
	, buildingPoint()
	, building(nullptr)
{
}

//Most common constructor, 
BuildQueueTask::BuildQueueTask(int64_t _id, ABILITY_ID _structure)
	: id(_id)
	, state(BuildingState::eQueued)
	, structureToBuild(_structure)
	, builderUnit(nullptr)
	, buildingPoint()
	, building(nullptr)
{
}

BuildQueueTask::~BuildQueueTask()
{
}

BuildingState BuildQueueTask::GetBuildingState()
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

ABILITY_ID BuildQueueTask::GetBuildingType()
{
	return structureToBuild;
}

const Unit* BuildQueueTask::GetBuilding()
{
	return building;
}

void BuildQueueTask::SetBuildingState(BuildingState newState)
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

void BuildQueueTask::SetBuilding(const Unit* _building)
{
	building = _building;
}
