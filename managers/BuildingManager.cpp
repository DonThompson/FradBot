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
	//Process the events in the queue
	//for(std::pair<int64_t, BuildQueueTask> pair : mapBuildingQueue) {
	for(auto it = mapBuildingQueue.cbegin(); it != mapBuildingQueue.cend();) {
		BuildQueueTask task = it->second;
		bool bUpdateTask = true;
		switch (task.GetBuildingState())
		{
		case BuildingState::eQueued:
			//TODO:  Delete, unnecessary
			//Move to finding a worker
			task.SetBuildingState(BuildingState::eFindingWorker);
			break;
		case BuildingState::eFindingWorker:
		{
			const Unit* builderUnit = Utils::GetRandomHarvester(Observation());
			if (builderUnit == nullptr) {
				//Can't find a worker.  Do nothing, we'll try again next step
			}
			else {
				task.AssignBuilder(builderUnit);
			}
			//Move on to finding a position
			task.SetBuildingState(BuildingState::eFindingPosition);
		}
			break;
		case BuildingState::eFindingPosition:
		{
			//Find a random place to build
			float rx = sc2::GetRandomScalar();
			float ry = sc2::GetRandomScalar();
			const Unit* myBuilder = task.GetBuilder();
			if (myBuilder != nullptr) {
				Point2D buildPoint(myBuilder->pos.x + rx * 15.0f, myBuilder->pos.y + ry * 15.0f);
				task.SetBuildPoint(buildPoint);
				//Issue build command
				task.SetBuildingState(BuildingState::eIssuingBuild);
			}
		}
			break;
		case BuildingState::eIssuingBuild:
			//Issue the action to the unit via the command menu to build and where
			Actions()->UnitCommand(task.GetBuilder(), task.GetBuildingType(), task.GetBuildPoint());

			//wait on build to start
			task.SetBuildingState(BuildingState::eWaitingOnBuildStart);
			break;
		case BuildingState::eWaitingOnBuildStart:
		{
			//TODO - what's this lazy worker doing?  Not entirely sure how to get this.
			const Unit* builder = task.GetBuilder();
			//tried build_progress (always 1.0000).  tried orders (don't change after he starts).

			//Only way to confirm the guy actually started building is to find a building linked to him?
			
			for (const Unit* buildingStarted : Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_SUPPLYDEPOT)) {
				if (buildingStarted->build_progress >= 0.9999f) {
					//done
					continue;
				}

				if (buildingStarted->build_progress < 0.0001f) {
					//not under construction
					continue;
				}

				/********************************
				*			TODO				*
				*********************************
				We issued a command to a worker to go build at a point.  It will take him some time to get there and begin
				actually working.  How can we tell that he's started?
				In digging through the CCBot code, he appears to do it by comparing positions.  He has a bit more detail on his
				Unit & building class data, and seems to have a reasonable way to figure this out.
				My data doesn't seem to work as well.  Here's an example:
					task.GetBuildPoint()	{x=131.581100 y=25.1799183 }	sc2::Point2D
					buildingStarted->pos	{x=109.000000 y=21.0000000 z=11.9882813 }	const sc2::Point3D
				It is the same building - so why are they so far apart?

				For lack of anything better at the moment, I've just guessed a distance of 10 is probably the same building.  I'm not
				sure if that's right or not.
				Need to find a better answer to all this.

				THIRD? TODO:  I think this is confused by issues with supply.  It's spawning hundreds of requests to build a depot -- most of which fail and get abandoned.
				All the size values I was comparing actually weren't the right connection.  Figure that out and come back.
				*/
				const float maxBuildingDistance = 1.0f;

				float distance = Distance2D(task.GetBuildPoint(), buildingStarted->pos);
				if (distance < maxBuildingDistance) {
					task.SetBuilding(buildingStarted);
					task.SetBuildingState(BuildingState::eConstructionInProgress);
				}
			}
		}
			break;
		case BuildingState::eConstructionInProgress:
			if (task.GetBuilding()->build_progress >= 0.999999f) {
				//Building done!
				task.SetBuildingState(BuildingState::eCompleted);
			}
			break;
		case BuildingState::eInterrupted:
			//TODO:  Not sure how we get into this state.  Look for builder deaths?
			break;
		case BuildingState::eInterrupted_FindingNewWorker:
			break;
		case BuildingState::eInterrupted_Resuming:	//Jumps back to eConstructionInProgress
			break;
		case BuildingState::eCompleted:
			//Done.  Clean it up, no need to monitor it in our queue any longer.

			//TODO:  Crash every time on map iteration after removal.
			//mapBuildingQueue.erase(it);
			bUpdateTask = false;
			break;
		}

		if (bUpdateTask) {
			mapBuildingQueue[it->first] = task;
		}

		it++;
	}
}

int64_t BuildingManager::BuildStructure(ABILITY_ID structureAbilityId)
{
	int64_t id = UseNextIdentifier();

	//Queue the request
	BuildQueueTask task(id, structureAbilityId);
	mapBuildingQueue.insert(std::pair<int64_t, BuildQueueTask>(id, task));
	return id;
}
