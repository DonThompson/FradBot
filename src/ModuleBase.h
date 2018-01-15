#pragma once
#include "ModuleNotificationRequirement.h"
class Bot;

//Base class for any game module.  Modules are designed to do a single task in response to some game stimulus.
class ModuleBase
{
public:
	explicit ModuleBase(Bot & b);

	//Turn on the module.  Will not be executed until enabled.
	void EnableModule();
	//Turn off the module.  No longer will receive any game updates.
	void DisableModule();
	//Is the module enabled?
	bool IsEnabled();

	//All derived classes must implement.  Defines which game notifications the module wishes to receive.
	virtual ModuleNotificationRequirement GetNotificationRequirements() = 0;

	//Implement as needed, should match your notification requirements.
	//	See bot.h for descriptions.
	virtual void OnGameStart() { }
	virtual void OnStep() { }
	virtual void OnUnitDestroyed(const sc2::Unit* unit) { }
	virtual void OnUnitCreated(const sc2::Unit* unit) { }
	virtual void OnUnitIdle(const sc2::Unit* unit) { }
	virtual void OnUpgradeCompleted(sc2::UpgradeID upgradeID) { }
	virtual void OnBuildingConstructionComplete(const sc2::Unit* unit) { }
	virtual void OnNydusDetected() { }
	virtual void OnNuclearLaunchDetected() { }
	virtual void OnUnitEnterVision(const sc2::Unit* unit) { }

protected:
	//Necessary reference to the bot.
	Bot & bot;
	//Module enabled status
	bool enabled;
};
