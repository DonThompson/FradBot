#include "EconManager.h"
#include "bot.h"
#include "WorkerProducerSearch.h"
using namespace sc2;

EconManager::EconManager(Bot & b)
	: ManagerBase(b)
	, refineriesInProgress(0)
	, refineriesCompleted(0)
{
	lastBalanceClock = clock();
}

void EconManager::OnStep()
{
	//Rebalance workers every few seconds.  Some odd timing issues can happen if we go every step
	const clock_t rebalanceTime = CLOCKS_PER_SEC * 2;   //2 seconds
	if (clock() - lastBalanceClock > rebalanceTime) {
		//If the economy manager has been asked to run on its own, perform these actions
		if (actAutonomously)
		{
			BalanceBuilders();

			if (NeedRefinery()) {
				BuildRefinery();
			}
		}

		//Work to do regardless of automony (inside time limiting)
		BalanceGasWorkers();

		lastBalanceClock = clock();
	}

	//Work to do regardless of automony (outside time limiting)

}

void EconManager::OnUnitIdle(const Unit* unit)
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

void EconManager::BalanceBuilders()
{
	//Make sure command centers have enough units - we might have just stolen some to bring them below threshold
	std::vector<Structure> ccs = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_COMMANDCENTER);
	for (Structure cc : ccs) {
		//Just call the idle function, it'll quit if not needed
		if (cc.getOrderCount() == 0) {
			HandleCommandCenterIdle(cc);
		}
	}
}

void EconManager::OnCommandCenterIdle(const Unit* unit)
{
	HandleCommandCenterIdle(Structure(unit));
}

void EconManager::HandleCommandCenterIdle(Structure cc)
{
	//Only build if we're acting autonomously.  Otherwise let the game strategy handle it.
	if (actAutonomously)
	{
		//Only build if we're short harvesters
		bool buildSCV = false;

		if (cc.assignedHarvesters() < cc.idealHarvesters()) {
			buildSCV = true;
		}

		//Or if we're short gas harvesters
		std::vector<Structure> refineries = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_REFINERY);
		for (Structure r : refineries) {
			if (r.assignedHarvesters() < r.idealHarvesters()) {
				buildSCV = true;
			}
		}

		if (buildSCV) {
			TrainWorker(&cc);
		}
	}
}

//Returns true if the training structure had the worker available to build and we issued the command.
//	TODO:  Still possible that it doesn't execute.
bool EconManager::TrainWorker(Structure* buildFrom/*= nullptr*/)
{
	//If the caller didn't provide one, find the optimal place to build.
	if (buildFrom == nullptr) {
		WorkerProducerSearch search(bot);
		buildFrom = search.SearchForProducer();
	}

	//Now execute the train command.
	if (buildFrom != nullptr) {
		//Handles checking for resources
		ABILITY_ID abilityID = ABILITY_ID::TRAIN_SCV;
		if (buildFrom->HasAbilityAvailable(bot, abilityID)) {
			Actions()->UnitCommand(buildFrom->building, abilityID);
			return true;
		}
	}

	return false;
}

//TODO:  Hardcoded
bool EconManager::NeedRefinery()
{
	int32_t supplyUsed = Observation()->GetFoodUsed();

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

//Counts in progress and completed
int32_t EconManager::GetRefineryCount()
{
	return refineriesCompleted + refineriesInProgress;
	// return Utils::CountOwnUnits(Observation(), UNIT_TYPEID::TERRAN_REFINERY);
}

void EconManager::OnRefinerySuccess(int64_t taskId)
{
	refineriesInProgress--;
	refineriesCompleted++;

	std::cout << "Refinery build success, task:  " << taskId << std::endl;
}

void EconManager::OnRefineryFailed(int64_t taskId)
{
	refineriesInProgress--;

	std::cout << "Refinery build failed, task:  " << taskId << std::endl;
}

void EconManager::BuildRefinery()
{
	//The build manager will take your request and do all it can to make it happen.  We get a callback on success/fail.
	uint64_t queueId = bot.Construction().BuildStructure(ABILITY_ID::BUILD_REFINERY,
		std::bind(&EconManager::OnRefinerySuccess, this, std::placeholders::_1),
		std::bind(&EconManager::OnRefineryFailed, this, std::placeholders::_1));

	refineriesInProgress++;

	std::cout << "Starting new refinery, task id:  " << queueId << std::endl;
}

const Unit* EconManager::FindNearestVespeneGeyser(const Point2D& start, const ObservationInterface* obs)
{
	Units units = obs->GetUnits(Unit::Alliance::Neutral);
	float distance = std::numeric_limits<float>::max();
	const Unit* target = nullptr;
	for (const auto& u : units) {
		if (Utils::IsVespeneGeyser(u->unit_type)) {
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
		if (Utils::IsMineralPatch(u->unit_type)) {
			//Use pathing distance, not air distance.
			//	NOTE:  Pathing using a unit appears to not work (returns 0) if the unit is in fog of war.  Explicitly use the position
			float d = bot.Query()->PathingDistance(u->pos, start);
			if (d < distance) {
				distance = d;
				target = u;
			}
		}
	}
	return target;
}

//"Always On" behavior
void EconManager::BalanceGasWorkers()
{
	//Version 1:  SIMPLE.  If we have a refinery < max, assign there.  Otherwise, assign to minerals.
	//	TODO:  Needs to pull from base location
	std::vector<Structure> refineries = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_REFINERY);
	for (Structure r : refineries) {
		if (r.IsBuildingComplete() && r.assignedHarvesters() < r.idealHarvesters()) {
			std::cout << "Moving harvester to gas refinery.  Assigned:  " << r.assignedHarvesters() << ".  Ideal:  " << r.idealHarvesters() << std::endl;
			const Unit* unit = Utils::GetRandomHarvester(Observation());
			Actions()->UnitCommand(unit, ABILITY_ID::SMART, r.building);
		}
	}
}
