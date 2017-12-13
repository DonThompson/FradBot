#ifndef __BUILDQUEUETASK_H
#define __BUILDQUEUETASK_H

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "BuildingState.h"

class BuildQueueTask
{
public:
	BuildQueueTask();
	BuildQueueTask(int64_t _id, ABILITY_ID _structure);
	~BuildQueueTask();

	BuildingState GetBuildingState();
	const Unit* GetBuilder();
	Point2D GetBuildPoint();
	ABILITY_ID GetBuildingType();
	const Unit* GetBuilding();

	void SetBuildingState(BuildingState newState);
	void AssignBuilder(const Unit* builder);
	void SetBuildPoint(Point2D _pt);
	void SetBuilding(const Unit* _building);

private:
	int64_t id;
	BuildingState state;
	ABILITY_ID structureToBuild;
	const Unit* builderUnit;
	Point2D buildingPoint;
	const Unit* building;
};

#endif //__BUILDQUEUETASK_H
