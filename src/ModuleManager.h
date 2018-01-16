#pragma once
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BuildQueueEnums.h"
class Bot;
class ModuleBase;

//In charge of managing all the modules loaded in the game.  Follows three distinct phases:
//	1)  Register modules.  See ModuleCreator.  Define your modules here and they'll be sent in.
//	2)  Enable modules.  Modules can be enabled by default or as part of a build order, etc.
//	3)  The ModuleManager will sent the required game notification events to the module.
class ModuleManager : public ManagerBase
{
public:
	ModuleManager(Bot & b);

	//Register your modules here
	void RegisterModule(MODULE_ID moduleID, const std::shared_ptr<ModuleBase> & gameModule);

	//Enable your modules here
	void EnableModule(MODULE_ID moduleID);
	void DisableModule(MODULE_ID moduleID);


	//Inherit all game events.  These should only be called by the bot framework
	virtual void OnGameStart();
	virtual void OnStep();
	virtual void OnUnitDestroyed(const sc2::Unit* unit);
	virtual void OnUnitCreated(const sc2::Unit* unit);
	virtual void OnUnitIdle(const sc2::Unit* unit);
	virtual void OnUpgradeCompleted(sc2::UpgradeID upgradeID);
	virtual void OnBuildingConstructionComplete(const sc2::Unit* unit);
	virtual void OnNydusDetected();
	virtual void OnNuclearLaunchDetected();
	virtual void OnUnitEnterVision(const sc2::Unit* unit);

private:
	//House pointers to all modules that need each type of notification
	std::vector<std::shared_ptr<ModuleBase>> gameStartNotifications;
	std::map<std::shared_ptr<ModuleBase>, uint32_t> stepLoopNotificationMap;
	std::vector<std::shared_ptr<ModuleBase>> unitDestroyNotifications;
	std::vector<std::shared_ptr<ModuleBase>> unitCreateNotifications;
	std::vector<std::shared_ptr<ModuleBase>> unitIdleNotifications;
	std::vector<std::shared_ptr<ModuleBase>> upgradeCompletedNotifications;
	std::vector<std::shared_ptr<ModuleBase>> buildingConstructedNotifications;
	std::vector<std::shared_ptr<ModuleBase>> nydusDetectedNotifications;
	std::vector<std::shared_ptr<ModuleBase>> nukeLaunchDetectedNotifications;
	std::vector<std::shared_ptr<ModuleBase>> unitEntersVisionNotifications;

	//All modules mapped to their enum
	std::map<MODULE_ID, std::shared_ptr<ModuleBase>> allModules;
};
