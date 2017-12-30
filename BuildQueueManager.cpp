#include "BuildQueueManager.h"
#include "bot.h"

BuildQueueManager::BuildQueueManager(Bot & b)
	: ManagerBase(b)
{
}

void BuildQueueManager::Push(BuildQueueItem item)
{
	buildQueue.push_back(item);
}

void BuildQueueManager::OnStep()
{
	//TODO
}
