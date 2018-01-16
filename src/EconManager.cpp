#include "EconManager.h"
#include "bot.h"
#include "WorkerProducerSearch.h"
#include "VespeneWorkerBalanceModule.h"
#include "IdleWorkerModule.h"
#include "AutoBuildWorkersModule.h"
using namespace sc2;
using namespace std;

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
			if (NeedRefinery()) {
				BuildRefinery();
			}
		}

		//Work to do regardless of automony (inside time limiting)

		lastBalanceClock = clock();
	}

	//Work to do regardless of automony (outside time limiting)

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
