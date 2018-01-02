#include "BuildQueueItem.h"
using namespace sc2;


BuildQueueItem::BuildQueueItem(AbilityID _abilityID)
{
	abilityToTrain = _abilityID;
}


BuildQueueItem::~BuildQueueItem()
{
}
