#pragma once

#include <sc2api/sc2_api.h>
#include "BuildQueueItemBase.h"
#include "BuildQueueEnums.h"

class BuildQueueItem : public BuildQueueItemBase
{
public:
	explicit BuildQueueItem(sc2::ABILITY_ID _abilityID);
	//Will build even if the buildings queue is not empty
	BuildQueueItem(sc2::ABILITY_ID _abilityID, const sc2::Unit* _fromBuilding);
	sc2::ABILITY_ID abilityToTrain;
	const sc2::Unit* fromBuilding;

	virtual std::string GetDescription();
};
