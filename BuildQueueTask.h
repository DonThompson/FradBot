#ifndef __BUILDQUEUETASK_H
#define __BUILDQUEUETASK_H

#include <sc2api/sc2_api.h>
#include "ConstructionTaskState.h"
#include "Structure.h"

typedef std::function<void(int64_t)> BuildQueueTaskCallbackFunction;

class BuildQueueTask
{
public:
	//Initialization
	BuildQueueTask();
	BuildQueueTask(uint32_t _gameLoop, int64_t _id, sc2::ABILITY_ID _structure, BuildQueueTaskCallbackFunction _successFn, BuildQueueTaskCallbackFunction _failFn);

	//Getters
	uint32_t GetStartingGameLoop();
	ConstructionTaskState GetConstructionTaskState();
	const sc2::Unit* GetBuilder();
	sc2::Point2D GetBuildPoint();
	const sc2::Unit* GetGeyserTarget();
	sc2::ABILITY_ID GetBuildingType();
	Structure GetBuilding();
	BuildQueueTaskCallbackFunction GetSuccessCallback();
	BuildQueueTaskCallbackFunction GetFailureCallback();

	//Setters
	void SetConstructionTaskState(ConstructionTaskState newState);
	void AssignBuilder(const sc2::Unit* builder);
	void SetBuildPoint(sc2::Point2D _pt);
	void SetGeyserTarget(const sc2::Unit* _geyser);
	void SetBuilding(Structure _building);
	void SetCallbackOnSuccess(BuildQueueTaskCallbackFunction fn);
	void SetCallbackOnFailure(BuildQueueTaskCallbackFunction fn);

	//Task management
	bool IsTaskLongRunning(uint32_t currentGameLoop);

private:
	uint32_t startingGameLoop;
	int64_t id;
	ConstructionTaskState state;
	sc2::ABILITY_ID structureToBuild;
	const sc2::Unit* builderUnit;
	sc2::Point2D buildingPoint;
	//TODO:  derive class instead of if/else statements
	const sc2::Unit* geyserTarget;	//alt to buildingPoint
	Structure building;
	BuildQueueTaskCallbackFunction callbackSuccess;
	BuildQueueTaskCallbackFunction callbackFailure;
};

#endif //__BUILDQUEUETASK_H
