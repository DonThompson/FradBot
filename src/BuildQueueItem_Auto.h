#pragma once
#include "BuildQueueItemBase.h"

class BuildQueueItem_Auto : public BuildQueueItemBase
{
public:
	BuildQueueItem_Auto(AUTO_ABILITYID _abilityID);
	AUTO_ABILITYID abilityID;
	virtual std::string GetDescription();
};

