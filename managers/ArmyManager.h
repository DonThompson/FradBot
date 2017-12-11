#ifndef __ARMY_MANAGER_H
#define __ARMY_MANAGER_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "../utils/Utils.h"


class ArmyManager : public ManagerBase {
public:
	ArmyManager();
	~ArmyManager();
	virtual void OnStep();
	bool BarracksNeeded();
	bool TryBuildBarracks();
	void TryAttackInGroups();
	void LaunchAttackGroup(Units unitsToAttack);
	void OnBarracksIdle(const Unit* unit);
	void OnMarineIdle(const Unit* unit);
	virtual void OnUnitIdle(const Unit* unit);

};

#endif //__ARMY_MANAGER_H
