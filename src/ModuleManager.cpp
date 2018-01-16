#include "ModuleManager.h"
#include "bot.h"
#include "ModuleBase.h"
#include "ModuleCreator.h"
using namespace std;
using namespace sc2;

ModuleManager::ModuleManager(Bot & b)
	: ManagerBase(b)
{
	ModuleCreator::CreateAllGameModules(b);
}

//Call to initialize any module and register it to receive notifications.  Note that modules still need to be
//	enabled before they will get notifications.
void ModuleManager::RegisterModule(MODULE_ID moduleID, const shared_ptr<ModuleBase> & gameModule)
{
	//Save a map of this module against its enumeration for future lookup
	allModules.insert(pair<MODULE_ID, std::shared_ptr<ModuleBase>>(moduleID, gameModule));

	//Setup any notifications for this module
	ModuleNotificationRequirement reqs = gameModule->GetNotificationRequirements();
	if (reqs.onGameStart) {
		gameStartNotifications.push_back(gameModule);
	}
	uint32_t stepLoopCount = reqs.stepLoopCount;
	if (stepLoopCount > 0) {
		stepLoopNotificationMap.insert(pair<const shared_ptr<ModuleBase>, uint32_t>(gameModule, stepLoopCount));
	}
	if (reqs.onUnitDestroyed) {
		unitDestroyNotifications.push_back(gameModule);
	}
	if (reqs.onUnitCreated) {
		unitCreateNotifications.push_back(gameModule);
	}
	if (reqs.onUnitIdle) {
		unitIdleNotifications.push_back(gameModule);
	}
	if (reqs.onUpgradeCompleted) {
		upgradeCompletedNotifications.push_back(gameModule);
	}
	if (reqs.onBuildingComplete) {
		buildingConstructedNotifications.push_back(gameModule);
	}
	if (reqs.onNydusDetected) {
		nydusDetectedNotifications.push_back(gameModule);
	}
	if (reqs.onNukeDetected) {
		nukeLaunchDetectedNotifications.push_back(gameModule);
	}
	if (reqs.onUnitEntersVision) {
		unitEntersVisionNotifications.push_back(gameModule);
	}
}

//Finds and enables the module with the given MODULE_ID
void ModuleManager::EnableModule(MODULE_ID moduleID)
{
	shared_ptr<ModuleBase> gameModule = allModules[moduleID];
	if (gameModule) {
		gameModule->EnableModule();
	}
}

//Finds and disables the module with the given MODULE_ID
void ModuleManager::DisableModule(MODULE_ID moduleID)
{
	shared_ptr<ModuleBase> gameModule = allModules[moduleID];
	if (gameModule) {
		gameModule->DisableModule();
	}
}


//////////////////////////
//	Notification Events //
//////////////////////////

void ModuleManager::OnGameStart()
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : gameStartNotifications) {
		if (m->IsEnabled()) {
			m->OnGameStart();
		}
	}
}

void ModuleManager::OnStep()
{
	//Send notifications out to each module that has been enabled
	for (std::pair<std::shared_ptr<ModuleBase>, uint32_t> p : stepLoopNotificationMap) {
		std::shared_ptr<ModuleBase> m = p.first;
		if (m->IsEnabled()) {
			if (bot.Observation()->GetGameLoop() % p.second == 0) {
				m->OnStep();
			}
		}
	}
}

void ModuleManager::OnUnitDestroyed(const sc2::Unit* unit)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : unitDestroyNotifications) {
		if (m->IsEnabled()) {
			m->OnUnitDestroyed(unit);
		}
	}
}

void ModuleManager::OnUnitCreated(const sc2::Unit* unit)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : unitCreateNotifications) {
		if (m->IsEnabled()) {
			m->OnUnitCreated(unit);
		}
	}
}

void ModuleManager::OnUnitIdle(const sc2::Unit* unit)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : unitIdleNotifications) {
		if (m->IsEnabled()) {
			m->OnUnitIdle(unit);
		}
	}
}

void ModuleManager::OnUpgradeCompleted(sc2::UpgradeID upgradeID)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : upgradeCompletedNotifications) {
		if (m->IsEnabled()) {
			m->OnUpgradeCompleted(upgradeID);
		}
	}
}

void ModuleManager::OnBuildingConstructionComplete(const sc2::Unit* unit)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : buildingConstructedNotifications) {
		if (m->IsEnabled()) {
			m->OnBuildingConstructionComplete(unit);
		}
	}
}

void ModuleManager::OnNydusDetected()
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : nydusDetectedNotifications) {
		if (m->IsEnabled()) {
			m->OnNydusDetected();
		}
	}
}

void ModuleManager::OnNuclearLaunchDetected()
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : nukeLaunchDetectedNotifications) {
		if (m->IsEnabled()) {
			m->OnNuclearLaunchDetected();
		}
	}
}

void ModuleManager::OnUnitEnterVision(const sc2::Unit* unit)
{
	//Send notifications out to each module that has been enabled
	for (std::shared_ptr<ModuleBase> m : unitEntersVisionNotifications) {
		if (m->IsEnabled()) {
			m->OnUnitEnterVision(unit);
		}
	}
}
