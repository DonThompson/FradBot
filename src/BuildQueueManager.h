#pragma once

#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BuildQueueItemBase.h"

class Bot;
class BuildQueueModuleItem;
class BuildQueueItem;

class BuildQueueManager : public ManagerBase
{
public:
	explicit BuildQueueManager(Bot & b);
	virtual void OnStep();

	void Push(std::shared_ptr<BuildQueueItemBase> item);

	void OnConstructionSuccess(int64_t taskId);
	void OnConstructionFailed(int64_t taskId);

private:
	std::vector<std::shared_ptr<BuildQueueItemBase>> buildQueue;

	bool IsWorker(sc2::ABILITY_ID abilityID);
	bool IsBuilding(sc2::ABILITY_ID abilityID);
	bool IsUnit(sc2::ABILITY_ID abilityID);
	bool HasResourcesFor(sc2::ABILITY_ID abilityID);

	void TryHandleModule(const std::shared_ptr<BuildQueueModuleItem> & item);
	void TryHandleGameAbility(const std::shared_ptr<BuildQueueItem> & item);
};
