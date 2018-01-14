#include "BuildQueueItem_Auto.h"



BuildQueueItem_Auto::BuildQueueItem_Auto(AUTO_ABILITYID _abilityID)
	: BuildQueueItemBase()
	, abilityID(_abilityID)
{
	itemType = BUILD_QUEUE_TYPE::AUTO_ABILITY;
}

std::string BuildQueueItem_Auto::GetDescription()
{
	//Needs to match the enum
	switch (abilityID) {
	case AUTO_ABILITYID::INVALID:
		return "INVALID";
	case AUTO_ABILITYID::ENABLE_AUTOBUILDWORKERS:
		return "ENABLE_AUTOBUILDWORKERS";
	default:
		//TODO:  string format and include the numeric value
		return "UNKNOWN ENUMERATION";
	}
}
