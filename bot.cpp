#include "bot.h"

Bot::Bot()
	: supplyManager(*this)
	, econManager(*this)
	, armyManager(*this)
	, constructionManager(*this)
{

}

Bot::~Bot()
{

}

ConstructionManager & Bot::Construction()
{
	return constructionManager;
}

const SupplyManager & Bot::Supply()
{
	return supplyManager;
}

const ArmyManager & Bot::Army()
{
	return armyManager;
}

const EconManager & Bot::Econ()
{
	return econManager;
}

void Bot::OnGameFullStart()
{
}

void Bot::OnGameStart()
{
	std::cout << "hello, World!" << std::endl;

	//Order added is order they'll get notifications and steps
	managers.push_back(&econManager);
	managers.push_back(&supplyManager);
	managers.push_back(&armyManager);
	managers.push_back(&constructionManager);
}

void Bot::OnGameEnd()
{
	std::cout << "Goodbye, World!" << std::endl;
}

void Bot::OnError(const std::vector<ClientError>& client_errors, const std::vector<std::string>& protocol_errors /*= {}*/)
{
}

void Bot::OnStep() {
	//Pass on the step to each manager
	for (ManagerBase* m : managers) {
		m->OnStep();
	}
}

void Bot::OnUnitDestroyed(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitDestroyed(unit);
	}
}

void Bot::OnUnitCreated(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitCreated(unit);
	}
}

void Bot::OnUnitIdle(const Unit* unit) {
	for (ManagerBase* m : managers) {
		m->OnUnitIdle(unit);
	}
}

void Bot::OnUpgradeCompleted(UpgradeID upgradeID)
{
	for (ManagerBase* m : managers) {
		m->OnUpgradeCompleted(upgradeID);
	}
}

void Bot::OnBuildingConstructionComplete(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnBuildingConstructionComplete(unit);
	}
}

void Bot::OnNydusDetected()
{
	for (ManagerBase* m : managers) {
		m->OnNydusDetected();
	}
}

void Bot::OnNuclearLaunchDetected()
{
	for (ManagerBase* m : managers) {
		m->OnNuclearLaunchDetected();
	}
}

void Bot::OnUnitEnterVision(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitEnterVision(unit);
	}
}
