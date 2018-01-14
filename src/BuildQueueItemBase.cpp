#include "BuildQueueItemBase.h"
using namespace sc2;

BuildQueueItemBase::BuildQueueItemBase()
	: firstSeenGameLoop(0)
	, timeoutLoops(0)
{
	//I'm hardcoding this for now.  We could make it a property that a build order could define later.
	//Purely made up from watching "realtime" games and getting through my build order.
	timeoutLoops = 500;
}

//Returns true if the item has timed out and should be thrown away
bool BuildQueueItemBase::CheckTimeout(uint32_t currentGameLoop)
{
	//First, we use this function as initialization.  If we haven't been seen, set it.
	if (firstSeenGameLoop == 0) {
		firstSeenGameLoop = currentGameLoop;
		return false;
	}

	//After that, look for timeout based on our configured value
	if (firstSeenGameLoop + timeoutLoops < currentGameLoop) {
		return true;
	}
	return false;
}
