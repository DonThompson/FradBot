#pragma once
#include <sc2api/sc2_api.h>

//Describes a modules notification requirements.  Defines which notifications will be sent to the module.
class ModuleNotificationRequirement
{
public:
	ModuleNotificationRequirement();
	ModuleNotificationRequirement(bool _onGameStart, uint32_t _stepLoopCount, bool _onUnitIdle, bool _onUnitCreated, bool _onUnitDestroyed
		, bool _onUpgradeCompleted, bool _onBuildingComplete, bool _onNydusDetected, bool _onNukeDetected, bool _onUnitEntersVision);

public:
	//Which notifications do you wish this module to receive?
	bool onGameStart;			//Notify at game start
	uint32_t stepLoopCount;		//Every x GetGameLoop().  0 = None, 1 = Every, 2+ = GetGameLoop() % stepLoopCount
	bool onUnitIdle;			//Notify when any unit becomes idle
	bool onUnitCreated;			//Notify when any unit is created
	bool onUnitDestroyed;		//Notify when any unit is destroyed
	bool onUpgradeCompleted;	//Notify when any upgrade completes
	bool onBuildingComplete;	//Notify when any building construction completes
	bool onNydusDetected;		//Notify when a nydus is detected
	bool onNukeDetected;		//Notify when a nuke is detected
	bool onUnitEntersVision;	//Notify when an enemy unit enters vision
};
