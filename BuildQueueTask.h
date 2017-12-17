#ifndef __BUILDQUEUETASK_H
#define __BUILDQUEUETASK_H

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ConstructionTaskState.h"

typedef std::function<void(int64_t)> BuildQueueTaskCallbackFunction;

class BuildQueueTask
{
public:
	//Initialization
	BuildQueueTask();
	BuildQueueTask(uint32_t _gameLoop, int64_t _id, ABILITY_ID _structure, BuildQueueTaskCallbackFunction _successFn, BuildQueueTaskCallbackFunction _failFn);

	//Getters
	uint32_t GetStartingGameLoop();
	ConstructionTaskState GetConstructionTaskState();
	const Unit* GetBuilder();
	Point2D GetBuildPoint();
	const Unit* GetGeyserTarget();
	ABILITY_ID GetBuildingType();
	const Unit* GetBuilding();
	BuildQueueTaskCallbackFunction GetSuccessCallback();
	BuildQueueTaskCallbackFunction GetFailureCallback();

	//Setters
	void SetConstructionTaskState(ConstructionTaskState newState);
	void AssignBuilder(const Unit* builder);
	void SetBuildPoint(Point2D _pt);
	void SetGeyserTarget(const Unit* _geyser);
	void SetBuilding(const Unit* _building);
	void SetCallbackOnSuccess(BuildQueueTaskCallbackFunction fn);
	void SetCallbackOnFailure(BuildQueueTaskCallbackFunction fn);

	//Task management
	bool IsTaskLongRunning(uint32_t currentGameLoop);

private:
	uint32_t startingGameLoop;
	int64_t id;
	ConstructionTaskState state;
	ABILITY_ID structureToBuild;
	const Unit* builderUnit;
	Point2D buildingPoint;
	//TODO:  derive class instead of if/else statements
	const Unit* geyserTarget;	//alt to buildingPoint
	const Unit* building;
	BuildQueueTaskCallbackFunction callbackSuccess;
	BuildQueueTaskCallbackFunction callbackFailure;
};

#endif //__BUILDQUEUETASK_H
