#pragma once

#include <sc2api/sc2_api.h>

class BuildQueueItem
{
public:
	BuildQueueItem(sc2::AbilityID _abilityID);
	~BuildQueueItem();

private:
	sc2::AbilityID abilityToTrain;
};

