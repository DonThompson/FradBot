#include "BuildingManager.h"
#include "../utils/Utils.h"

BuildingManager::BuildingManager(Bot & b)
	: ManagerBase(b)
	, nextBuildingId(0)
{

}

BuildingManager::~BuildingManager()
{

}

int64_t BuildingManager::UseNextIdentifier()
{
	int64_t useThis = nextBuildingId;
	nextBuildingId++;
	return useThis;
}

void BuildingManager::OnStep()
{
	//TODO
}

int64_t BuildingManager::BuildStructure(ABILITY_ID structureAbilityId)
{
	int64_t id = UseNextIdentifier();

	//Queue the request
	mapBuildingStates.insert(std::pair<int64_t, BuildingState>(id, BuildingState::eQueued));

	//TODO:  can we immediately return and let the caller continue on?  Move all this elsewhere.
	const Unit* builderUnit = nullptr;
	{
		//Now find the worker
		mapBuildingStates[id] = BuildingState::eFindingWorker;

		//Get a builder to work with
		builderUnit = Utils::GetRandomHarvester(Observation());
		if (builderUnit == nullptr) {
			//Can't find a worker.  Return the id to the caller, the request is in our queue and we'll try again shortly.
			return id;
		}
	}

	//TODO:  return and on to next step
	{
		//Now find the position
		mapBuildingStates[id] = BuildingState::eFindingPosition;

		//Find a random place to build
		float rx = sc2::GetRandomScalar();
		float ry = sc2::GetRandomScalar();

		//TODO:  Separate states, but fairly inclusive of each other
		mapBuildingStates[id] = BuildingState::eIssuingBuild;

		//Issue the action to the unit via the command menu to build and where
		Actions()->UnitCommand(builderUnit, structureAbilityId, Point2D(builderUnit->pos.x + rx * 15.0f, builderUnit->pos.y + ry * 15.0f));

		mapBuildingStates[id] = BuildingState::eWaitingOnBuildStart;
	}

	//TODO:  return and on to next step
	return id;
	//return Utils::TryBuildStructure(observation, actions, ABILITY_ID::BUILD_SUPPLYDEPOT);
}
