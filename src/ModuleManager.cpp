#include "ModuleManager.h"
#include "bot.h"
#include "ModuleBase.h"
using namespace std;
using namespace sc2;

ModuleManager::ModuleManager(Bot & b)
	: ManagerBase(b)
{
}

//Call to initialize any module and register it to receive notifications.  Note that modules still need to be
//	enabled before they will get notifications.
void ModuleManager::RegisterModule(const shared_ptr<ModuleBase> gameModule)
{
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
}

void ModuleManager::OnUnitCreated(const sc2::Unit* unit)
{
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
}

void ModuleManager::OnBuildingConstructionComplete(const sc2::Unit* unit)
{
}

void ModuleManager::OnNydusDetected()
{
}

void ModuleManager::OnNuclearLaunchDetected()
{
}

void ModuleManager::OnUnitEnterVision(const sc2::Unit* unit)
{
}
