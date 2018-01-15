#pragma once

#include <sc2api/sc2_api.h>
#include "BuildQueueItemBase.h"
#include "BuildQueueEnums.h"

class BuildQueueItem : public BuildQueueItemBase
{
public:
	BuildQueueItem(sc2::ABILITY_ID _abilityID);
	sc2::ABILITY_ID abilityToTrain;

	virtual std::string GetDescription();
};
