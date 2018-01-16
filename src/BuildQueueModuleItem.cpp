#include "BuildQueueModuleItem.h"

BuildQueueModuleItem::BuildQueueModuleItem(MODULE_ID _moduleID)
	: BuildQueueItemBase()
	, moduleID(_moduleID)
{
	itemType = BUILD_QUEUE_TYPE::GAME_MODULE;
}

std::string BuildQueueModuleItem::GetDescription()
{
	//TODO:  Do we really need this? Seems a hassle to maintain
	return "UNKNOWN ENUMERATION";
}
