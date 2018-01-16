#pragma once
#include "BuildQueueItemBase.h"

//TODO:  This was an "AutoAbility", but got turned into the Module system at some point.  Could use renaming.
class BuildQueueModuleItem : public BuildQueueItemBase
{
public:
	explicit BuildQueueModuleItem(MODULE_ID _moduleID);
	MODULE_ID moduleID;
	virtual std::string GetDescription();
};
