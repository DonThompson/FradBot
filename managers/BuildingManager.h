#ifndef __BUILDING_MANAGER_H
#define __BUILDING_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"

class Bot;

//States of a build
enum BuildingState  {
	eQueued = 0,
	eFindingWorker,
	eFindingPosition,
	eIssuingBuild,
	eWaitingOnBuildStart,
	eConstructionInProgress,
	eInterrupted,
	eInterrupted_FindingNewWorker,
	eInterrupted_Resuming,	//Jumps back to eConstructionInProgress
	eCompleted
};


class BuildingManager : public ManagerBase
{
public:
	BuildingManager(Bot & b);
	~BuildingManager();

	int64_t BuildStructure(ABILITY_ID ability_type_for_structure);

	virtual void OnStep();

private:
	int64_t nextBuildingId;
	std::map<int64_t, BuildingState> mapBuildingStates;
	int64_t UseNextIdentifier();

};

#endif //__BUILDING_MANAGER_H
