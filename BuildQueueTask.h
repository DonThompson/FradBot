#ifndef __BUILDQUEUETASK_H
#define __BUILDQUEUETASK_H

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "BuildingState.h"

typedef std::function<void(int64_t)> BuildQueueTaskCallbackFunction;

class BuildQueueTask
{
public:
	BuildQueueTask();
	BuildQueueTask(int64_t _id, ABILITY_ID _structure, BuildQueueTaskCallbackFunction _successFn, BuildQueueTaskCallbackFunction _failFn);
	~BuildQueueTask();

	BuildingState GetBuildingState();
	const Unit* GetBuilder();
	Point2D GetBuildPoint();
	ABILITY_ID GetBuildingType();
	const Unit* GetBuilding();
	BuildQueueTaskCallbackFunction GetSuccessCallback();
	BuildQueueTaskCallbackFunction GetFailureCallback();

	void SetBuildingState(BuildingState newState);
	void AssignBuilder(const Unit* builder);
	void SetBuildPoint(Point2D _pt);
	void SetBuilding(const Unit* _building);
	void SetCallbackOnSuccess(BuildQueueTaskCallbackFunction fn);
	void SetCallbackOnFailure(BuildQueueTaskCallbackFunction fn);

private:
	int64_t id;
	BuildingState state;
	ABILITY_ID structureToBuild;
	const Unit* builderUnit;
	Point2D buildingPoint;
	const Unit* building;
	BuildQueueTaskCallbackFunction callbackSuccess;
	BuildQueueTaskCallbackFunction callbackFailure;
};

#endif //__BUILDQUEUETASK_H
