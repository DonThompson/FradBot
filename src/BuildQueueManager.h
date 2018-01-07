#pragma once

#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BuildQueueItem.h"

class Bot;

class BuildQueueManager : public ManagerBase
{
public:
	BuildQueueManager(Bot & b);
	virtual void OnStep();

	void Push(BuildQueueItem item);

	void OnConstructionSuccess(int64_t taskId);
	void OnConstructionFailed(int64_t taskId);

private:
	std::vector<BuildQueueItem> buildQueue;

	bool IsWorker(sc2::ABILITY_ID abilityID);
	bool IsBuilding(sc2::ABILITY_ID abilityID);
	bool IsUnit(sc2::ABILITY_ID abilityID);
	bool IsMorphOrAddOn(sc2::ABILITY_ID abilityID);
	bool HasResourcesFor(sc2::ABILITY_ID abilityID);
};
