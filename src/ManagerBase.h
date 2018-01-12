#ifndef __MANAGER_BASE_H
#define __MANAGER_BASE_H

#include <sc2api/sc2_api.h>

class Bot;

class ManagerBase
{
public:
	//Base constructor
	ManagerBase(Bot & bot);

	//Required for all managers to implement, handles each step of the game.
	virtual void OnStep() = 0;

	//Individual game actions, not required for managers to implement.  Use as needed.
	virtual void OnGameStart() {}
	virtual void OnUnitIdle(const sc2::Unit* unit) {}
	virtual void OnUnitDestroyed(const sc2::Unit* unit) {}
	virtual void OnUnitCreated(const sc2::Unit* unit) {}
	virtual void OnUpgradeCompleted(sc2::UpgradeID upgradeID) {}
	virtual void OnBuildingConstructionComplete(const sc2::Unit* unit) {}
	virtual void OnNydusDetected() {}
	virtual void OnNuclearLaunchDetected() {}
	virtual void OnUnitEnterVision(const sc2::Unit* unit) {}

	//Enables the economy manager to act autonomously.  Each manager will treat this differently.
	void EnableAutonomy();
	//Disables the economy manager to act autonomously.  Each manager will treat this differently.
	void DisableAutonomy();
	//Gets a summary string to output on the screen when debugging
	virtual std::string GetDebugSummaryString() { return ""; }

protected:
	//Our working bot
	Bot& bot;

	//See each manager for implementation
	bool actAutonomously;

	//Provide access to the bot agent's Observation interface
	const sc2::ObservationInterface* ManagerBase::Observation();
	//Provide access to the bot agent's Action interface
	sc2::ActionInterface* ManagerBase::Actions();

};


#endif //__MANAGER_BASE_H
