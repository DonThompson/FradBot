#pragma once

#include <sc2api/sc2_api.h>

class BuildQueueItem
{
public:
	BuildQueueItem(sc2::AbilityID _abilityID);
	~BuildQueueItem();

	//TODO:  Should these by private?  do we want to bother with setter/getters?
public:
	sc2::AbilityID abilityToTrain;
};

