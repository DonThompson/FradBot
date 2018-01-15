#pragma once

#include <sc2api/sc2_api.h>
#include "BuildQueueEnums.h"

class BuildQueueItemBase
{
public:
	BuildQueueItemBase();

	bool CheckTimeout(uint32_t currentGameLoop);

	//TODO:  Should these by private?  do we want to bother with setter/getters?
public:
	BUILD_QUEUE_TYPE itemType;
	virtual std::string GetDescription() = 0;

	//Timeout behavior
	//Game loop when this build queue item was first seen
	uint32_t firstSeenGameLoop;
	//How many game loops until we should just quit this item?  Needs to be reasonably high
	//	for real things (I want to expand as my first building), but low enough that something
	//	isn't stuck forever making the bot do nothing.
	uint32_t timeoutLoops;
};
