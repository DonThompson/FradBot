#include "BuildQueueItem.h"
using namespace sc2;

BuildQueueItem::BuildQueueItem(ABILITY_ID _abilityID)
	: BuildQueueItemBase()
	, abilityToTrain(_abilityID)
{
	itemType = BUILD_QUEUE_TYPE::GAME_ABILITY;
}

std::string BuildQueueItem::GetDescription()
{
	return sc2::AbilityTypeToName(abilityToTrain);
}
