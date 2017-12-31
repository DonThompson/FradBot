#include "UpgradeList.h"



UpgradeList::UpgradeList()
	: hasConcussionGrenades(false)
	, hasStim(false)
	, hasCombatShields(false)
	, infAttackLevel(0)
	, infDefenseLevel(0)
{

}

bool UpgradeList::NeedUpgrades()
{
	//TODO:  Stop returning true when we are done.
	//TODO:  take supply/mins/gas to determine?
	return true;
}

void UpgradeList::CompleteWeaponUpgrade()
{
	infAttackLevel++;
}

void UpgradeList::CompleteArmorUpgrade()
{
	infDefenseLevel++;
}
