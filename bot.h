#ifndef __BOT_H
#define __BOT_H

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;

#include "managers/ManagerBase.h"
#include "managers/SupplyManager.h"
#include "managers/EconManager.h"
#include "ArmyManager.h"
#include "ConstructionManager.h"
#include "StructuresManager.h"
#include "UpgradesManager.h"
#include "utils/Utils.h"


class Bot : public Agent {
private:
	SupplyManager supplyManager;
	EconManager econManager;
	ArmyManager armyManager;
	ConstructionManager constructionManager;
	StructuresManager structuresManager;
	UpgradesManager upgradesManager;

	std::vector<ManagerBase*> managers;

public:
	Bot();
	~Bot();

	//////////////////////////////////////////////
	//  Access to Manager Classes				//
	//////////////////////////////////////////////
	ConstructionManager & Construction();
	StructuresManager & Structures();
	const SupplyManager & Supply();
	const ArmyManager & Army();
	const EconManager & Econ();


	//////////////////////////////////////////////
	//Everything in ClientEvents, with comments //
	//////////////////////////////////////////////

	/////////////////////////////////////////////
	// Full game level, not passed to managers //
	/////////////////////////////////////////////
	//! Called when a game is started after a load. Fast restarting will not call this.
	virtual void OnGameFullStart();
	//! Called when a game is started or restarted.
	virtual void OnGameStart();
	//! Called when a game has ended.
	virtual void OnGameEnd();
	//! Called for various errors the library can encounter. See ClientError enum for possible errors.
	virtual void OnError(const std::vector<ClientError>& client_errors, const std::vector<std::string>& protocol_errors = {});

	////////////////////////////////////////
	// In game events, passed to managers //
	////////////////////////////////////////

	//! In non realtime games this function gets called after each step as indicated by step size.
	//! In realtime this function gets called as often as possible after request/responses are received from the game gathering observation state.
	virtual void OnStep();
	//! Called whenever one of the player's units has been destroyed.
	//!< \param unit The destroyed unit.
	virtual void OnUnitDestroyed(const Unit* unit);
	//! Called when a Unit has been created by the player.
	//!< \param unit The created unit.
	virtual void OnUnitCreated(const Unit* unit);
	//! Called when a unit becomes idle, this will only occur as an event so will only be called when the unit becomes
	//! idle and not a second time. Being idle is defined by having orders in the previous step and not currently having
	//! orders or if it did not exist in the previous step and now does, a unit being created, for instance, will call both
	//! OnUnitCreated and OnUnitIdle if it does not have a rally set.
	//!< \param unit The idle unit.
	virtual void OnUnitIdle(const Unit* unit);
	//! Called when an upgrade is finished, warp gate, ground weapons, baneling speed, etc.
	//!< \param upgrade The completed upgrade.
	virtual void OnUpgradeCompleted(UpgradeID upgradeID);
	//! Called when the unit in the previous step had a build progress less than 1.0 but is greater than or equal to 1.0 in the current step.
	//!< \param unit The constructed unit.
	virtual void OnBuildingConstructionComplete(const Unit* unit);
	//! Called when a nydus is placed.
	virtual void OnNydusDetected();
	//! Called when a nuclear launch is detected.
	virtual void OnNuclearLaunchDetected();
	//! Called when an enemy unit enters vision from out of fog of war.
	//!< \param unit The unit entering vision.
	virtual void OnUnitEnterVision(const Unit* unit);

};

#endif //__BOT_H
