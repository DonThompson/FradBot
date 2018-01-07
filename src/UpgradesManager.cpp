#include "UpgradesManager.h"
#include "bot.h"
using namespace sc2;

//TODO:  Better way to do this?  copy/pasted values out of typeenums
//TODO:  Put in upgrade manager?  Data()?
/*static */bool UpgradesManager::IsUpgrade(sc2::ABILITY_ID abilityID)
{
	switch (abilityID) {
	case ABILITY_ID::BUILD_NUKE:		//Trained as an 'upgrade' at the ghost academy
	case ABILITY_ID::RESEARCH_ADVANCEDBALLISTICS:
	case ABILITY_ID::RESEARCH_BANSHEECLOAKINGFIELD:
	case ABILITY_ID::RESEARCH_BANSHEEHYPERFLIGHTROTORS:
	case ABILITY_ID::RESEARCH_BATTLECRUISERWEAPONREFIT:
	case ABILITY_ID::RESEARCH_COMBATSHIELD:
	case ABILITY_ID::RESEARCH_CONCUSSIVESHELLS:
	case ABILITY_ID::RESEARCH_DRILLINGCLAWS:
	case ABILITY_ID::RESEARCH_HIGHCAPACITYFUELTANKS:
	case ABILITY_ID::RESEARCH_HISECAUTOTRACKING:
	case ABILITY_ID::RESEARCH_INFERNALPREIGNITER:
	case ABILITY_ID::RESEARCH_MAGFIELDLAUNCHERS:
	case ABILITY_ID::RESEARCH_NEOSTEELFRAME:
	case ABILITY_ID::RESEARCH_PERSONALCLOAKING:
	case ABILITY_ID::RESEARCH_RAVENCORVIDREACTOR:
	case ABILITY_ID::RESEARCH_RAVENRECALIBRATEDEXPLOSIVES:
	case ABILITY_ID::RESEARCH_STIMPACK:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANSTRUCTUREARMORUPGRADE:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATING:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL3:
		return true;
	}
	return false;
}

//TODO:  Refactor elsewhere?  Can this get built into UnitData?
/*static */sc2::UNIT_TYPEID UpgradesManager::GetUpgradeProducerType(sc2::ABILITY_ID abilityID)
{
	switch (abilityID) {
		//Engineering Bay
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMOR:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYARMORLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANINFANTRYWEAPONSLEVEL3:
	case ABILITY_ID::RESEARCH_HISECAUTOTRACKING:
	case ABILITY_ID::RESEARCH_NEOSTEELFRAME:
	case ABILITY_ID::RESEARCH_TERRANSTRUCTUREARMORUPGRADE:
		return UNIT_TYPEID::TERRAN_ENGINEERINGBAY;

		//Barracks tech lab
	case ABILITY_ID::RESEARCH_COMBATSHIELD:
	case ABILITY_ID::RESEARCH_CONCUSSIVESHELLS:
	case ABILITY_ID::RESEARCH_STIMPACK:
		return UNIT_TYPEID::TERRAN_BARRACKSTECHLAB;

		//Factory tech lab
	case ABILITY_ID::RESEARCH_INFERNALPREIGNITER:
	case ABILITY_ID::RESEARCH_RAPIDFIRELAUNCHERS:
	case ABILITY_ID::RESEARCH_DRILLINGCLAWS:
	case ABILITY_ID::RESEARCH_SMARTSERVOS:
		return UNIT_TYPEID::TERRAN_FACTORYTECHLAB;

		//Starport tech lab
	case ABILITY_ID::RESEARCH_HIGHCAPACITYFUELTANKS:
	case ABILITY_ID::RESEARCH_ENHANCEDMUNITIONS:
	case ABILITY_ID::RESEARCH_RAVENCORVIDREACTOR:
	case ABILITY_ID::RESEARCH_BANSHEECLOAKINGFIELD:
	case ABILITY_ID::RESEARCH_BANSHEEHYPERFLIGHTROTORS:
	case ABILITY_ID::RESEARCH_ADVANCEDBALLISTICS:
		return UNIT_TYPEID::TERRAN_STARPORTTECHLAB;

		//Armory
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANSHIPWEAPONSLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATING:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEANDSHIPPLATINGLEVEL3:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONS:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL1:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL2:
	case ABILITY_ID::RESEARCH_TERRANVEHICLEWEAPONSLEVEL3:
		return UNIT_TYPEID::TERRAN_ARMORY;

		//Ghosts
	case ABILITY_ID::RESEARCH_PERSONALCLOAKING:
	case ABILITY_ID::BUILD_NUKE:
		return UNIT_TYPEID::TERRAN_GHOSTACADEMY;

		//Battlecruisers
	case ABILITY_ID::RESEARCH_BATTLECRUISERWEAPONREFIT:
		return UNIT_TYPEID::TERRAN_FUSIONCORE;

		//Removed?  According to http://liquipedia.net/starcraft2/Tech_Lab_(Legacy_of_the_Void)
		//case ABILITY_ID::RESEARCH_MAGFIELDLAUNCHERS:
		//case ABILITY_ID::RESEARCH_RAVENRECALIBRATEDEXPLOSIVES:
		//Behemoth reactor (not defined)
	default:
		break;
	}
	return UNIT_TYPEID::INVALID;
}

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

//TODO:  Temporarily used, only covers infantry upgrades
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

//Performs an upgrade for the given ability.  Rules:
//	Find producer (can fail if we don't have one)
//	See if it's already producing something (we never queue)
//	See if it can produce this upgrade (pre-requisites may not be met)
//	Perform the action
//TODO:  No absolute guarantee of build here.  If the resources are stolen at the same time, we'll fail and not try again.
//TODO:  Should we queue upgrades?  They're lengthy and maybe we want to just get them out of the request pipeline
bool UpgradesManager::PerformUpgrade(sc2::ABILITY_ID abilityID)
{
	//What produces this upgrade?
	UnitData data = bot.Data().GetUnitData(abilityID);
	UNIT_TYPEID producerType = UpgradesManager::GetUpgradeProducerType(abilityID);

	//Find producers
	std::vector<Structure> structures = bot.Structures().GetStructuresByType(producerType);
	for (Structure s : structures) {
		//is producing?
		if (s.getOrderCount() == 0) {
			//can it do this now? (pre-reqs)
			if (s.HasAbilityAvailable(bot, abilityID)) {
				//do it!
				Actions()->UnitCommand(s.building, abilityID);
				return true;
			}
			else {
				//Can't perform this upgrade, not available.
			}
		}
		else {
			//Has existing orders, skip it
		}
	}
	return false;
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
