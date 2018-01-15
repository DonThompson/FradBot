#include "ModuleNotificationRequirement.h"

//All notification requirements are disabled by default
ModuleNotificationRequirement::ModuleNotificationRequirement()
	: ModuleNotificationRequirement(false, 0, false, false, false)
{
}

//Initialize with the provided defaults
ModuleNotificationRequirement::ModuleNotificationRequirement(bool _onGameStart, uint32_t _stepLoopCount, bool _onUnitIdle, bool _onUnitCreated, bool onUnitDestroyed)
	: onGameStart(_onGameStart)
	, stepLoopCount(_stepLoopCount)
	, onUnitIdle(_onUnitIdle)
	, onUnitCreated(_onUnitCreated)
	, onUnitDestroyed(onUnitDestroyed)
{

}
