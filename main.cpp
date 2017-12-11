#include <sc2api/sc2_api.h>
//#include <sc2utils/sc2_manage_process.h>        //Needed for SleepFor only
#include <iostream>
using namespace sc2;


//Through tutorial 3 plus some bonus events.  Beats the medium terran AI 80% of the time.

//Main issues to work on:
//  Multi-actions on a step.  If a barracks needs built, it will often build as many as it can.  Same with refineries and any other building.



class Utils {
public:
	//Get a random peon that is currently harvesting (not returning, not building, not moving somewhere on another mission, etc).
	static const Unit* GetRandomHarvester(const ObservationInterface* observation)
	{
		Units units = GetOwnUnits(observation, UNIT_TYPEID::TERRAN_SCV);

		//Max 5 tries, if it fails, try again next step
		const int tries = 5;
		for (int i = 0; i < tries; i++)
		{
			//Get a completely random harvester
			const Unit* harvester = units[rand() % units.size()];
			if (harvester == nullptr) {
				return nullptr;
			}

			//See if this unit already has another order, we don't want to hijack him.
			for (const auto& order : harvester->orders) {
				//  We'll avoid anyone doing HARVEST_RETURN to ensure we maximize income
				if (order.ability_id == ABILITY_ID::HARVEST_GATHER) {
					return harvester;
				}
				else {
					//Got something else useful going on, skip him
					std::cout << "Skipping builder because he's busy with action " << order.ability_id << std::endl;
				}
			}
		}

		//None found, try again later
		return nullptr;
	}

	static bool Utils::TryBuildStructure(const ObservationInterface* observation, ActionInterface* actions, ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type = UNIT_TYPEID::TERRAN_SCV)
	{
		//Get a builder to work with
		const Unit* unit_to_build = Utils::GetRandomHarvester(observation);
		if (unit_to_build == nullptr) {
			return false;
		}

		//Find a random place to build
		float rx = GetRandomScalar();
		float ry = GetRandomScalar();

		//Issue the action to the unit via the command menu to build and where
		actions->UnitCommand(unit_to_build, ability_type_for_structure, Point2D(unit_to_build->pos.x + rx * 15.0f, unit_to_build->pos.y + ry * 15.0f));

		return true;
	}

	static int32_t Utils::CountOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
	{
		return (int32_t)observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID)).size();
	}

	static Units Utils::GetOwnUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID)
	{
		return observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
	}

	static Units Utils::GetIdleUnits(const ObservationInterface* observation, UNIT_TYPEID unitTypeID, UNIT_TYPEID unitTypeID2)
	{
		//TODO:  Need to find some info on Filter, this should be simpler.
		Units units1 = observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID));
		Units units2 = observation->GetUnits(Unit::Alliance::Self, IsUnit(unitTypeID2));

		Units idleUnits;
		for (const Unit* unit : units1) {
			if (unit->orders.size() == 0) {
				idleUnits.push_back(unit);
			}
		}
		for (const Unit* unit : units2) {
			if (unit->orders.size() == 0) {
				idleUnits.push_back(unit);
			}
		}

		return idleUnits;
	}
};

class ManagerBase
{
public:
	void ManagerBase::Initialize(Agent* b)
	{
		bot = b;
	}

	//Required for all managers to implement
	virtual void OnStep() = 0;

	//game actions, not required for managers to implement
	virtual void OnUnitIdle(const Unit* unit) {}
	virtual void OnUnitDestroyed(const Unit* unit) {}
	virtual void OnUnitCreated(const Unit* unit) {}
	virtual void OnUpgradeCompleted(UpgradeID upgradeID) {}
	virtual void OnBuildingConstructionComplete(const Unit* unit) {}
	virtual void OnNydusDetected() {}
	virtual void OnNuclearLaunchDetected() {}
	virtual void OnUnitEnterVision(const Unit* unit) {}

protected:
	Agent* bot;

	ManagerBase::ManagerBase()
	{
		bot = nullptr;
	}

	//Provide access to the bot agent's Observation interface
	const ObservationInterface* ManagerBase::Observation()
	{
		return bot->Observation();
	}

	ActionInterface* ManagerBase::Actions()
	{
		return bot->Actions();
	}

};

class SupplyManager : public ManagerBase
{
public:
	SupplyManager::SupplyManager()
	{
	}

	SupplyManager::~SupplyManager()
	{
	}

	virtual void SupplyManager::OnStep()
	{
		if (SupplyDepotNeeded()) {
			TryBuildSupplyDepot();
		}
	}

private:
	bool SupplyManager::SupplyDepotNeeded()
	{
		const ObservationInterface* observation = Observation();
		int32_t supplyUsed = observation->GetFoodUsed();
		int32_t currentSupplyCap = observation->GetFoodCap();

		//Shortcut - never go over 200
		if (currentSupplyCap >= 200) {
			return false;
		}

		//Predict how many we need to be building.
		int32_t numDepotNeeded = PredictSupplyDepotsNeeded();

		//How many are currently being built?
		int32_t numDepotInProgress = CountSupplyDepotsInProgress();

		if (numDepotNeeded > numDepotInProgress) {
			std::cout << "Starting new depot" << std::endl;
			return true;
		}

		return false;
	}

	int32_t SupplyManager::PredictSupplyDepotsNeeded()
	{
		const int32_t supplyDepotFood = 8;

		const ObservationInterface* observation = Observation();
		int32_t supplyUsed = observation->GetFoodUsed();
		int32_t currentSupplyCap = observation->GetFoodCap();

		//Total units currently being built.
		int32_t supplyCurrentlyBeingProduced = CalculateSupplyCurrentlyBeingProduced();

		//Assumption:  Once the current units get built, we'll shortly want to be able to build at least that much more again.  If that is going to put us
		//  at a supply disadvantage, then we'd better start a new depot.
		int32_t extraNeeded = supplyUsed + supplyCurrentlyBeingProduced - currentSupplyCap;
		if (extraNeeded <= 0) {
			//Shortcut:  If we're over 90% capacity, build a new depot, always.
			if (supplyUsed >= (int32_t)(currentSupplyCap * 0.90f))
				return 1;
			return 0;
		}

		return (int32_t)ceil((double)extraNeeded / (double)supplyDepotFood);
	}

	int32_t SupplyManager::CalculateSupplyCurrentlyBeingProduced()
	{
		const ObservationInterface* observation = Observation();
		ActionInterface* actions = Actions();

		//Loop through all units and see if they're building a unit right now, and if so what the cost of that unit is
		int32_t supplyBeingProduced = 0;

		Units units = observation->GetUnits(Unit::Alliance::Self);
		for (const auto& unit : units) {
			for (const auto& order : unit->orders) {
				supplyBeingProduced += GetUnitSupplyActivelyProducing(order);
			}
		}

		return supplyBeingProduced;
	}

	int32_t GetUnitSupplyActivelyProducing(UnitOrder order)
	{
		//NOTE:  I manually ripped these out of ABILITY_ID enum and manually entered the food cost by lookup.  Any way to calculate these?
		switch ((sc2::ABILITY_ID)order.ability_id) {
		case ABILITY_ID::TRAINWARP_ADEPT:
		case ABILITY_ID::TRAINWARP_DARKTEMPLAR:
		case ABILITY_ID::TRAINWARP_HIGHTEMPLAR:
		case ABILITY_ID::TRAINWARP_SENTRY:
		case ABILITY_ID::TRAINWARP_STALKER:
		case ABILITY_ID::TRAINWARP_ZEALOT:
		case ABILITY_ID::TRAIN_ADEPT:
		case ABILITY_ID::TRAIN_BANELING:
		case ABILITY_ID::TRAIN_BANSHEE:
		case ABILITY_ID::TRAIN_BATTLECRUISER:
		case ABILITY_ID::TRAIN_CARRIER:
		case ABILITY_ID::TRAIN_COLOSSUS:
		case ABILITY_ID::TRAIN_CORRUPTOR:
		case ABILITY_ID::TRAIN_CYCLONE:
		case ABILITY_ID::TRAIN_DARKTEMPLAR:
		case ABILITY_ID::TRAIN_DISRUPTOR:
		case ABILITY_ID::TRAIN_DRONE:
		case ABILITY_ID::TRAIN_GHOST:
		case ABILITY_ID::TRAIN_HELLBAT:
		case ABILITY_ID::TRAIN_HELLION:
		case ABILITY_ID::TRAIN_HIGHTEMPLAR:
		case ABILITY_ID::TRAIN_HYDRALISK:
		case ABILITY_ID::TRAIN_IMMORTAL:
		case ABILITY_ID::TRAIN_INFESTOR:
		case ABILITY_ID::TRAIN_LIBERATOR:
		case ABILITY_ID::TRAIN_MARAUDER:
		case ABILITY_ID::TRAIN_MARINE:
		case ABILITY_ID::TRAIN_MEDIVAC:
		case ABILITY_ID::TRAIN_MOTHERSHIPCORE:
		case ABILITY_ID::TRAIN_MUTALISK:
		case ABILITY_ID::TRAIN_OBSERVER:
		case ABILITY_ID::TRAIN_ORACLE:
			//case ABILITY_ID::TRAIN_OVERLORD:  //no supply
		case ABILITY_ID::TRAIN_PHOENIX:
		case ABILITY_ID::TRAIN_PROBE:
		case ABILITY_ID::TRAIN_QUEEN:
		case ABILITY_ID::TRAIN_RAVEN:
		case ABILITY_ID::TRAIN_REAPER:
		case ABILITY_ID::TRAIN_ROACH:
		case ABILITY_ID::TRAIN_SCV:
		case ABILITY_ID::TRAIN_SENTRY:
		case ABILITY_ID::TRAIN_SIEGETANK:
		case ABILITY_ID::TRAIN_STALKER:
		case ABILITY_ID::TRAIN_SWARMHOST:
		case ABILITY_ID::TRAIN_TEMPEST:
		case ABILITY_ID::TRAIN_THOR:
		case ABILITY_ID::TRAIN_ULTRALISK:
		case ABILITY_ID::TRAIN_VIKINGFIGHTER:
		case ABILITY_ID::TRAIN_VIPER:
		case ABILITY_ID::TRAIN_VOIDRAY:
		case ABILITY_ID::TRAIN_WARPPRISM:
			//case ABILITY_ID::TRAIN_WIDOWMINE: //No supply used
		case ABILITY_ID::TRAIN_ZEALOT:
		case ABILITY_ID::TRAIN_ZERGLING:
			//TODO:  Need to actually find the value for each unit
			return 1;
			break;
		default:
			//Any other ID is not a unit being trained
			return 0;
			break;
		}
	}

	int32_t SupplyManager::CountSupplyDepotsInProgress()
	{
		const ObservationInterface* observation = Observation();
		ActionInterface* actions = Actions();

		//Find out how many units are currently building supply depots
		int32_t countSupplyDepotBuilders = 0;

		Units units = observation->GetUnits(Unit::Alliance::Self);
		for (const auto& unit : units) {
			for (const auto& order : unit->orders) {
				if (order.ability_id == ABILITY_ID::BUILD_SUPPLYDEPOT) {
					countSupplyDepotBuilders++;
				}
			}
		}

		return countSupplyDepotBuilders;
	}

	bool SupplyManager::TryBuildSupplyDepot()
	{
		const ObservationInterface* observation = Observation();

		//try to build a depot - use a random scv
		//std::cout << "Building supply depot...  Used:  " << observation->GetFoodUsed() << ".  Cap:  " << observation->GetFoodCap() << std::endl;
		return Utils::TryBuildStructure(Observation(), Actions(), ABILITY_ID::BUILD_SUPPLYDEPOT);
	}
};

class EconManager : public ManagerBase {
private:
	clock_t lastBalanceClock;

public:
	EconManager::EconManager()
	{
		lastBalanceClock = clock();
	}

	EconManager::~EconManager()
	{
	}

	virtual void EconManager::OnStep()
	{
		if (NeedRefinery()) {
			TryBuildRefinery();
		}

		//Rebalance workers every few seconds.  Some odd timing issues can happen if we go every step
		const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
		if (clock() - lastBalanceClock > rebalanceTime) {
			BalanceBuilders();
			lastBalanceClock = clock();
		}
	}

	virtual void OnUnitIdle(const Unit* unit)
	{
		switch (unit->unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_COMMANDCENTER:     OnCommandCenterIdle(unit);      break;
		case UNIT_TYPEID::TERRAN_SCV: {
			const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
			if (mineral_target == nullptr) {
				break;
			}
			Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
			break;
		}
		default: {
			break;
		}
		}
	}

private:
	void EconManager::BalanceBuilders()
	{
		//Version 1:  SIMPLE.  If we have a refinery < max, assign there.  Otherwise, assign to minerals.
		Units refineries = Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_REFINERY);
		for (const Unit* r : refineries) {
			if (r->build_progress >= 1.0f && r->assigned_harvesters < r->ideal_harvesters) {
				std::cout << "Moving harvester to gas refinery.  Assigned:  " << r->assigned_harvesters << ".  Ideal:  " << r->ideal_harvesters << std::endl;
				const Unit* unit = Utils::GetRandomHarvester(Observation());
				Actions()->UnitCommand(unit, ABILITY_ID::SMART, r);
			}
		}

		//Make sure command centers have enough units - we might have just stolen some to bring them below threshold
		Units cc = Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_COMMANDCENTER);
		for (const Unit* u : cc) {
			//Just call the idle function, it'll quit if not needed
			if (u->orders.size() == 0) {
				OnCommandCenterIdle(u);
			}
		}
	}

	void EconManager::OnCommandCenterIdle(const Unit* unit)
	{
		//Only build if we're short harvesters
		bool buildSCV = false;

		if (unit->assigned_harvesters < unit->ideal_harvesters) {
			buildSCV = true;
		}

		//Or if we're short gas harvesters
		Units refineries = Utils::GetOwnUnits(Observation(), UNIT_TYPEID::TERRAN_REFINERY);
		for (const Unit* r : refineries) {
			if (r->assigned_harvesters < r->ideal_harvesters) {
				buildSCV = true;
			}
		}

		if (buildSCV) {
			Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
		}
	}

	//Simple hardcoded
	bool EconManager::NeedRefinery()
	{
		const ObservationInterface* observation = Observation();
		ActionInterface* actions = Actions();

		int32_t supplyUsed = observation->GetFoodUsed();

		//Don't build the first until 18 supply.
		if (supplyUsed >= 18 && GetRefineryCount() < 1) {
			return true;
		}

		//Build the second around 22
		if (supplyUsed >= 22 && GetRefineryCount() < 2) {
			return true;
		}

		return false;
	}

	//Counts building and built
	int32_t EconManager::GetRefineryCount()
	{
		return Utils::CountOwnUnits(Observation(), UNIT_TYPEID::TERRAN_REFINERY);
	}

	bool EconManager::TryBuildRefinery()
	{
		const ObservationInterface* observation = Observation();
		ActionInterface* actions = Actions();

		//Get a builder to work with
		const Unit* builder = Utils::GetRandomHarvester(observation);
		if (builder == nullptr) {
			return false;
		}

		const Unit* vespeneGeyser = FindNearestVespeneGeyser(builder->pos);
		if (vespeneGeyser == nullptr) {
			return false;
		}

		//Geyers builds are actioned by tag, not by point
		actions->UnitCommand(builder, ABILITY_ID::BUILD_REFINERY, vespeneGeyser);

		return true;
	}

	const Unit* EconManager::FindNearestVespeneGeyser(const Point2D& start)
	{
		Units units = Observation()->GetUnits(Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const Unit* target = nullptr;
		for (const auto& u : units) {
			if (u->unit_type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		return target;
	}

	const Unit* EconManager::FindNearestMineralPatch(const Point2D& start)
	{
		Units units = Observation()->GetUnits(Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const Unit* target = nullptr;
		for (const auto& u : units) {
			if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		return target;
	}

};

class ArmyManager : public ManagerBase {
public:
	ArmyManager::ArmyManager()
	{
	}

	ArmyManager::~ArmyManager()
	{
	}

	virtual void ArmyManager::OnStep()
	{
		const ObservationInterface* observation = Observation();
		ActionInterface* actions = Actions();

		if (BarracksNeeded()) {
			TryBuildBarracks();
		}

		//The whole strategy!
		TryAttackInGroups();
	}

	bool ArmyManager::BarracksNeeded()
	{
		const ObservationInterface* observation = Observation();

		int32_t countBarracks = Utils::CountOwnUnits(observation, UNIT_TYPEID::TERRAN_BARRACKS);
		int32_t currentSupply = observation->GetFoodUsed();
		int32_t currentMinerals = observation->GetMinerals();

		//Build our first barracks at 16 and build one every time we hit 600 minerals in the bank after that.  Max at 10.
		//Cap to no more than 1 per 15 food - we were getting hung up sometimes and building 9 rax at ~35 supply
		if (countBarracks == 0 && currentSupply > 16) {
			return true;
		}
		else {
			int32_t maxDesiredRax = currentSupply / 15;
			if (countBarracks < maxDesiredRax && currentMinerals >= 600) {
				return true;
			}
		}

		return false;
	}

	bool ArmyManager::TryBuildBarracks()
	{
		return Utils::TryBuildStructure(Observation(), Actions(), ABILITY_ID::BUILD_BARRACKS);
	}

	void ArmyManager::TryAttackInGroups()
	{
		const ObservationInterface* observation = Observation();

		Units idleArmy = Utils::GetIdleUnits(observation, UNIT_TYPEID::TERRAN_MARINE, UNIT_TYPEID::TERRAN_MARAUDER);
		if (idleArmy.size() > 15) {
			LaunchAttackGroup(idleArmy);
		}
	}

	void ArmyManager::LaunchAttackGroup(Units unitsToAttack)
	{
		const GameInfo& gameInfo = Observation()->GetGameInfo();
		//What is attack_attack vs attack?
		Actions()->UnitCommand(unitsToAttack, ABILITY_ID::ATTACK_ATTACK, gameInfo.enemy_start_locations.front());
	}

	void ArmyManager::OnBarracksIdle(const Unit* unit)
	{
		//0 = marine, 1 = marauder
		unsigned int thingToBuild = 0;

		//Got from examples:
		//  https://github.com/Blizzard/s2client-api/blob/master/examples/common/bot_examples.cc
		const Unit* raxAddOn = Observation()->GetUnit(unit->add_on_tag);
		if (raxAddOn == nullptr) {
			//Not upgraded, let's upgrade it 10% of the time.  Short circuit out if we do this.
			if (rand() % 10 == 0) {
				Actions()->UnitCommand(unit, ABILITY_ID::BUILD_TECHLAB);
				return;
			}

			//Only marines
			thingToBuild = 0;
		}
		else if (raxAddOn->unit_type == UNIT_TYPEID::TERRAN_BARRACKSREACTOR) {
			//Marines only
			thingToBuild = 0;
		}
		else if (raxAddOn->unit_type == UNIT_TYPEID::TERRAN_BARRACKSTECHLAB) {
			thingToBuild = rand() % 2;
		}

		switch (thingToBuild) {
		case 0:
			Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARINE);
			break;
		case 1:
			Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARAUDER);
			break;
		}
	}

	void ArmyManager::OnMarineIdle(const Unit* unit)
	{
		/* too aggressive
		const GameInfo& gameInfo = Observation()->GetGameInfo();
		//What is attack_attack vs attack?
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, gameInfo.enemy_start_locations.front());
		*/
	}

	virtual void ArmyManager::OnUnitIdle(const Unit* unit)
	{
		switch (unit->unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_BARRACKS:      OnBarracksIdle(unit);       break;
		case UNIT_TYPEID::TERRAN_MARINE:        OnMarineIdle(unit);         break;
		default:    break;
		}
	}
};

class Bot : public Agent {
private:
	SupplyManager supplyManager;
	EconManager econManager;
	ArmyManager armyManager;

	std::vector<ManagerBase*> managers;

public:
	//////////////////////////////////////////////
	//Everything in ClientEvents, with comments //
	//////////////////////////////////////////////


	/////////////////////////////////////////////
	// Full game level, not passed to managers //
	/////////////////////////////////////////////

	//! Called when a game is started after a load. Fast restarting will not call this.
	virtual void OnGameFullStart() {}

	//! Called when a game is started or restarted.
	virtual void OnGameStart() {
		std::cout << "hello, World!" << std::endl;

		supplyManager.Initialize(this);
		econManager.Initialize(this);
		armyManager.Initialize(this);

		//Order added is order they'll get notifications and steps
		managers.push_back(&econManager);
		managers.push_back(&supplyManager);
		managers.push_back(&armyManager);
	}

	//! Called when a game has ended.
	virtual void OnGameEnd() {
		std::cout << "Goodbye, World!" << std::endl;
	}

	//! Called for various errors the library can encounter. See ClientError enum for possible errors.
	virtual void OnError(const std::vector<ClientError>& /*client_errors*/, const std::vector<std::string>& /*protocol_errors*/ = {}) {}


	////////////////////////////////////////
	// In game events, passed to managers //
	////////////////////////////////////////

	//! In non realtime games this function gets called after each step as indicated by step size.
	//! In realtime this function gets called as often as possible after request/responses are received from the game gathering observation state.
	virtual void OnStep() {
		//Pass on the step to each manager
		for (ManagerBase* m : managers) {
			m->OnStep();
		}
	}

	//! Called whenever one of the player's units has been destroyed.
	//!< \param unit The destroyed unit.
	virtual void OnUnitDestroyed(const Unit* unit)
	{
		for (ManagerBase* m : managers) {
			m->OnUnitDestroyed(unit);
		}
	}

	//! Called when a Unit has been created by the player.
	//!< \param unit The created unit.
	virtual void OnUnitCreated(const Unit* unit)
	{
		for (ManagerBase* m : managers) {
			m->OnUnitCreated(unit);
		}
	}

	//! Called when a unit becomes idle, this will only occur as an event so will only be called when the unit becomes
	//! idle and not a second time. Being idle is defined by having orders in the previous step and not currently having
	//! orders or if it did not exist in the previous step and now does, a unit being created, for instance, will call both
	//! OnUnitCreated and OnUnitIdle if it does not have a rally set.
	//!< \param unit The idle unit.
	virtual void OnUnitIdle(const Unit* unit) {
		for (ManagerBase* m : managers) {
			m->OnUnitIdle(unit);
		}
	}

	//! Called when an upgrade is finished, warp gate, ground weapons, baneling speed, etc.
	//!< \param upgrade The completed upgrade.
	virtual void OnUpgradeCompleted(UpgradeID upgradeID)
	{
		for (ManagerBase* m : managers) {
			m->OnUpgradeCompleted(upgradeID);
		}
	}

	//! Called when the unit in the previous step had a build progress less than 1.0 but is greater than or equal to 1.0 in the current step.
	//!< \param unit The constructed unit.
	virtual void OnBuildingConstructionComplete(const Unit* unit)
	{
		for (ManagerBase* m : managers) {
			m->OnBuildingConstructionComplete(unit);
		}
	}

	//! Called when a nydus is placed.
	virtual void OnNydusDetected()
	{
		for (ManagerBase* m : managers) {
			m->OnNydusDetected();
		}
	}

	//! Called when a nuclear launch is detected.
	virtual void OnNuclearLaunchDetected()
	{
		for (ManagerBase* m : managers) {
			m->OnNuclearLaunchDetected();
		}
	}

	//! Called when an enemy unit enters vision from out of fog of war.
	//!< \param unit The unit entering vision.
	virtual void OnUnitEnterVision(const Unit* unit)
	{
		for (ManagerBase* m : managers) {
			m->OnUnitEnterVision(unit);
		}
	}
};


int main(int argc, char* argv[])
{
	//seed randomization
	srand(static_cast<unsigned int>(time(NULL)));


	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);

	Bot bot;
	coordinator.SetParticipants({
		CreateParticipant(Race::Terran, &bot),
		CreateComputer(Race::Terran, sc2::Medium)
	});

	//Slow down the damn game to a more normal speed
	//https://github.com/Blizzard/s2client-api/issues/179
	//However this also has the effect where a variable number of GameLoops may pass between calls to your OnStep depending 
	//  on how slow your logic is. If this is undesirable I suggest alkurbatov's solution.
	//coordinator.SetRealtime(true);

	coordinator.LaunchStarcraft();
	coordinator.StartGame(sc2::kMapBelShirVestigeLE);

	while (coordinator.Update())
	{
		//keep updating

		//Use this and you can control a little better than the "realtime" flag if you want to go fast
		//alkurbatov says:
		//sc2::SleepFor(15);
	}

	return 0;
}
