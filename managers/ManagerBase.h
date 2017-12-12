#ifndef __MANAGER_BASE_H
#define __MANAGER_BASE_H

#include <sc2api/sc2_api.h>
using namespace sc2;

class Bot;

class ManagerBase
{
public:
	//Base constructor
	ManagerBase(Bot & bot);

	//Required for all managers to implement, handles each step of the game.
	virtual void OnStep() = 0;

	//Individual game actions, not required for managers to implement.  Use as needed.
	virtual void OnUnitIdle(const Unit* unit) {}
	virtual void OnUnitDestroyed(const Unit* unit) {}
	virtual void OnUnitCreated(const Unit* unit) {}
	virtual void OnUpgradeCompleted(UpgradeID upgradeID) {}
	virtual void OnBuildingConstructionComplete(const Unit* unit) {}
	virtual void OnNydusDetected() {}
	virtual void OnNuclearLaunchDetected() {}
	virtual void OnUnitEnterVision(const Unit* unit) {}

protected:
	//Our working bot
	Bot& bot;

	//Provide access to the bot agent's Observation interface
	const ObservationInterface* ManagerBase::Observation();
	//Provide access to the bot agent's Action interface
	ActionInterface* ManagerBase::Actions();

};


#endif //__MANAGER_BASE_H
