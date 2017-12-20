#include "UpgradesManager.h"
#include "bot.h"

UpgradesManager::UpgradesManager(Bot & b)
	: ManagerBase(b)
	, lastBalanceClock(clock_t())
	, countEngBays(0)
	, countEngBaysInProgress(0)
{
}

void UpgradesManager::OnStep()
{
	//Rebalance supply every few seconds.  We really don't need to do this every step.
	//TODO:  Clock is REAL time.  Should we use game time?  In super fast AI mode this might actually be 5-6 game seconds.
	const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
	if (clock() - lastBalanceClock > rebalanceTime) {

		//Specific logic
		BuildEngBaysAsNeeded();
		TryToUpgrade();

		lastBalanceClock = clock();
	}
}

void UpgradesManager::OnUpgradeCompleted(UpgradeID upgradeID)
{
	std::cout << "Upgrade complete:  " << sc2::UpgradeIDToName(upgradeID) << std::endl;
	switch (upgradeID.operator sc2::UPGRADE_ID()) {
	case UPGRADE_ID::TERRANINFANTRYARMORSLEVEL1:
	case UPGRADE_ID::TERRANINFANTRYARMORSLEVEL2:
	case UPGRADE_ID::TERRANINFANTRYARMORSLEVEL3:
		upgrades.CompleteArmorUpgrade();
		break;
	case UPGRADE_ID::TERRANINFANTRYWEAPONSLEVEL1:
	case UPGRADE_ID::TERRANINFANTRYWEAPONSLEVEL2:
	case UPGRADE_ID::TERRANINFANTRYWEAPONSLEVEL3:
		upgrades.CompleteWeaponUpgrade();
		break;
	case UPGRADE_ID::PUNISHERGRENADES:	//Odd name, but means concussion grenades
		upgrades.hasConcussionGrenades = true;
		break;
	case UPGRADE_ID::SHIELDWALL:	//Odd name, but means combat shields
		upgrades.hasCombatShields = true;
		break;
	case UPGRADE_ID::STIMPACK:
		upgrades.hasStim = true;
		break;
	}
}

void UpgradesManager::BuildEngBaysAsNeeded()
{
	//max 1
	if (countEngBays + countEngBaysInProgress >= 1)
		return;

	const ObservationInterface* obs = Observation();
	int32_t currentSupply = obs->GetFoodUsed();
	int32_t currentMinerals = obs->GetMinerals();

	//Don't do anything before ~25 supply
	if (currentSupply < 23)
		return;

	//and 200 mins
	if (currentMinerals < 200)
		return;

	uint64_t queueId = bot.Construction().BuildStructure(ABILITY_ID::BUILD_ENGINEERINGBAY,
		std::bind(&UpgradesManager::OnEngBaySuccess, this, std::placeholders::_1),
		std::bind(&UpgradesManager::OnEngBayFailed, this, std::placeholders::_1));

	std::cout << "Starting new Eng Bay, task id:  " << queueId << std::endl;

	countEngBaysInProgress++;
}

void UpgradesManager::OnEngBaySuccess(int64_t taskId)
{
	countEngBaysInProgress--;
	countEngBays++;

	std::cout << "Eng Bay build success, task:  " << taskId << std::endl;
}

void UpgradesManager::OnEngBayFailed(int64_t taskId)
{
	countEngBaysInProgress--;

	std::cout << "Eng Bay build failed, task:  " << taskId << std::endl;
}
void UpgradesManager::TryToUpgrade()
{
	const ObservationInterface* obs = Observation();
	int32_t currentSupply = obs->GetFoodUsed();
	int32_t currentMinerals = obs->GetMinerals();
	int32_t currentGas = obs->GetVespene();

	if (!upgrades.NeedUpgrades())
		return;

	//Don't do anything before ~23 supply
	if (countEngBays == 0 && currentSupply < 23)
		return;

	//Make sure we have at least 200/200 before spending on an upgrade.
	if (currentMinerals < 200 || currentGas < 200)
		return;

	//Look for tech labs
	PerformTechLabUpgrades();

	//and ebays
	PerformEngBayUpgrades();
}

void UpgradesManager::PerformTechLabUpgrades()
{
	for (const Unit* techlab : Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_BARRACKSTECHLAB)) {
		if (techlab->orders.size() > 0)
			continue;
		else {
			//Research in order
			ABILITY_ID research = ABILITY_ID::INVALID;
			//TODO:  When we know how to use stim, it should go first
			if (!upgrades.hasCombatShields)
				research = ABILITY_ID::RESEARCH_COMBATSHIELD;
			else if (!upgrades.hasConcussionGrenades)
				research = ABILITY_ID::RESEARCH_CONCUSSIVESHELLS;
			else if (!upgrades.hasStim)
				research = ABILITY_ID::RESEARCH_STIMPACK;

			if (research != ABILITY_ID::INVALID) {
				std::cout << "Starting research at " << sc2::UnitTypeToName(techlab->unit_type) << " for " << sc2::AbilityTypeToName(research) << std::endl;
				Actions()->UnitCommand(techlab, research);
			}
		}
	}
}

void UpgradesManager::PerformEngBayUpgrades()
{
	for (const Unit* ebay : Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_ENGINEERINGBAY)) {
		//Only build 
		if (ebay->orders.size() > 0)
			continue;
		else {
			//Upgrade order... atttack1, def1, atk2, atk3, def2, def3
			ABILITY_ID research = ABILITY_ID::INVALID;

			if (upgrades.infAttackLevel == 0)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS;
			else if (upgrades.infDefenseLevel == 0)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR;
			else if (upgrades.infAttackLevel < 3)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS;
			else if (upgrades.infDefenseLevel < 3)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR;

			if (research != ABILITY_ID::INVALID) {
				std::cout << "Starting research at " << sc2::UnitTypeToName(ebay->unit_type) << " for " << sc2::AbilityTypeToName(research) << std::endl;
				Actions()->UnitCommand(ebay, research);
			}
		}
	}
}
