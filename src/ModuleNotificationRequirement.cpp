#include "ModuleNotificationRequirement.h"

//All notification requirements are disabled by default
ModuleNotificationRequirement::ModuleNotificationRequirement()
	: ModuleNotificationRequirement(false, 0, false, false, false, false, false, false, false, false)
{
}

//Initialize with the provided defaults
ModuleNotificationRequirement::ModuleNotificationRequirement(bool _onGameStart, uint32_t _stepLoopCount, bool _onUnitIdle, bool _onUnitCreated, bool _onUnitDestroyed
	, bool _onUpgradeCompleted, bool _onBuildingComplete, bool _onNydusDetected, bool _onNukeDetected, bool _onUnitEntersVision)
	: onGameStart(_onGameStart)
	, stepLoopCount(_stepLoopCount)
	, onUnitIdle(_onUnitIdle)
	, onUnitCreated(_onUnitCreated)
	, onUnitDestroyed(_onUnitDestroyed)
	, onUpgradeCompleted(_onUpgradeCompleted)
	, onBuildingComplete(_onBuildingComplete)
	, onNydusDetected(_onNydusDetected)
	, onNukeDetected(_onNukeDetected)
	, onUnitEntersVision(_onUnitEntersVision)
{
}
