#include "UpgradesManager.h"
#include "bot.h"
using namespace sc2;

UpgradesManager::UpgradesManager(Bot & b)
	: ManagerBase(b)
	, lastBalanceClock(clock_t())
	, countEngBays(0)
	, countEngBaysInProgress(0)
{
}

void UpgradesManager::OnStep()
{
	//Only do these if we're autonomous
	if (actAutonomously)
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

	//Do these things regardless
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
	std::vector<Structure> techlabs = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_BARRACKSTECHLAB);
	for (Structure techlab : techlabs) {
		if (techlab.getOrderCount() > 0)
			continue;
		else {
			//What can the building actually upgrade right now?
			AvailableAbilities aa = bot.Query()->GetAbilitiesForUnit(techlab.building);

			//Shortcut - we'll fairly early on run out of upgrades to do.  Just quit when we hit that point.
			if (aa.abilities.size() == 0) {
				continue;
			}

			//Crazy c++ syntax...
			bool canUpgradeStim = std::find_if(aa.abilities.begin(), aa.abilities.end(),
				[](const AvailableAbility &aa) -> bool { return aa.ability_id == ABILITY_ID::RESEARCH_STIMPACK; }) != aa.abilities.end();
			bool canUpgradeShields = std::find_if(aa.abilities.begin(), aa.abilities.end(),
				[](const AvailableAbility &aa) -> bool { return aa.ability_id == ABILITY_ID::RESEARCH_COMBATSHIELD; }) != aa.abilities.end();
			bool canUpgradeGrenades = std::find_if(aa.abilities.begin(), aa.abilities.end(),
				[](const AvailableAbility &aa) -> bool { return aa.ability_id == ABILITY_ID::RESEARCH_CONCUSSIVESHELLS; }) != aa.abilities.end();

			//Research in order
			ABILITY_ID research = ABILITY_ID::INVALID;
			//TODO:  When we know how to use stim, it should go first
			if (!upgrades.hasCombatShields && canUpgradeShields)
				research = ABILITY_ID::RESEARCH_COMBATSHIELD;
			else if (!upgrades.hasConcussionGrenades && canUpgradeGrenades)
				research = ABILITY_ID::RESEARCH_CONCUSSIVESHELLS;
			else if (!upgrades.hasStim && canUpgradeStim)
				research = ABILITY_ID::RESEARCH_STIMPACK;

			if (research != ABILITY_ID::INVALID) {
				std::cout << "Starting research at " << sc2::UnitTypeToName(techlab.getUnitTypeID()) << " for " << sc2::AbilityTypeToName(research) << std::endl;
				Actions()->UnitCommand(techlab.building, research);
			}
		}
	}
}

void UpgradesManager::PerformEngBayUpgrades()
{
	std::vector<Structure> ebays = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_ENGINEERINGBAY);
	for (Structure ebay : ebays) {
		//Only build 
		if (ebay.getOrderCount() > 0)
			continue;
		else {
			//What can the building actually upgrade right now?
			AvailableAbilities aa = bot.Query()->GetAbilitiesForUnit(ebay.building);

			//Upgrade order... atttack1, def1, atk2, atk3, def2, def3
			ABILITY_ID research = ABILITY_ID::INVALID;

			//Crazy c++ syntax...
			bool canUpgradeWeapons = std::find_if(aa.abilities.begin(), aa.abilities.end(),
				[](const AvailableAbility &aa) -> bool { return aa.ability_id == ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS; }) != aa.abilities.end();
			bool canUpgradeArmor = std::find_if(aa.abilities.begin(), aa.abilities.end(),
				[](const AvailableAbility &aa) -> bool { return aa.ability_id == ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR; }) != aa.abilities.end();

			if (upgrades.infAttackLevel == 0 && canUpgradeWeapons)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS;
			else if (upgrades.infDefenseLevel == 0 && canUpgradeArmor)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR;
			else if (upgrades.infAttackLevel < 3 && canUpgradeWeapons)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS;
			else if (upgrades.infDefenseLevel < 3 && canUpgradeArmor)
				research = ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR;

			if (research != ABILITY_ID::INVALID) {
				std::cout << "Starting research at " << sc2::UnitTypeToName(ebay.getUnitTypeID()) << " for " << sc2::AbilityTypeToName(research) << std::endl;
				Actions()->UnitCommand(ebay.building, research);
			}
		}
	}
}
