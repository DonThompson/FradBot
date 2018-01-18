#include "BuildQueueItem.h"
using namespace sc2;

BuildQueueItem::BuildQueueItem(ABILITY_ID _abilityID)
	: BuildQueueItem(_abilityID, nullptr)
{
	//Call the other constructor
}

BuildQueueItem::BuildQueueItem(sc2::ABILITY_ID _abilityID, const sc2::Unit* _fromBuilding)
	: BuildQueueItemBase()
	, abilityToTrain(_abilityID)
	, fromBuilding(_fromBuilding)
{
	itemType = BUILD_QUEUE_TYPE::GAME_ABILITY;
}

std::string BuildQueueItem::GetDescription()
{
	return sc2::AbilityTypeToName(abilityToTrain);
}
