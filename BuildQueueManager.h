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

private:
	std::vector<BuildQueueItem> buildQueue;
};
