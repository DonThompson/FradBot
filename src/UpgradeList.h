#pragma once

class UpgradeList {
public:
	bool hasConcussionGrenades;
	bool hasStim;
	bool hasCombatShields;
	int infAttackLevel;
	int infDefenseLevel;

	UpgradeList();
	bool NeedUpgrades();
	void CompleteWeaponUpgrade();
	void CompleteArmorUpgrade();

};