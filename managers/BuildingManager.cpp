#include "BuildingManager.h"
#include "../utils/Utils.h"

BuildingManager::BuildingManager()
: nextBuildingId(0)
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

//TODO:  unit_type is ignored
int64_t BuildingManager::BuildStructure(const ObservationInterface* observation, ActionInterface* actions, ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type /*= UNIT_TYPEID::TERRAN_SCV*/)
{
	int64_t id = UseNextIdentifier();

	//Queue the request
	mapBuildingStates.insert(std::pair<int64_t, BuildingState>(id, BuildingState::eQueued));

	//TODO:  can we immediately return and let the caller continue on?  Move all this elsewhere.
	const Unit* unit_to_build = nullptr;
	{
		//Now find the worker
		mapBuildingStates[id] = BuildingState::eFindingWorker;

		//Get a builder to work with
		unit_to_build = Utils::GetRandomHarvester(observation);
		if (unit_to_build == nullptr) {
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
		actions->UnitCommand(unit_to_build, ability_type_for_structure, Point2D(unit_to_build->pos.x + rx * 15.0f, unit_to_build->pos.y + ry * 15.0f));

		mapBuildingStates[id] = BuildingState::eWaitingOnBuildStart;
	}

	//TODO:  return and on to next step
	return id;
	//return Utils::TryBuildStructure(observation, actions, ABILITY_ID::BUILD_SUPPLYDEPOT);
}
