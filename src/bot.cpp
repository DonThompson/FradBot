#include "bot.h"
#include <ctime>
#include <iomanip>
#include "Timer.h"
#include <queue>
#include <sstream>
using namespace sc2;

Bot::Bot()
	: strategyManager(*this)
	, supplyManager(*this)
	, econManager(*this)
	, armyManager(*this)
	, constructionManager(*this)
	, structuresManager(*this)
	, upgradesManager(*this)
	, drawingManager(*this)
	, baseLocationManager(*this)
	, buildQueueManager(*this)
	, dataManager(*this)
	, morphManager(*this)
	, mapManager(*this)
	, last20GameLoopsTotalTimeMs(0)
{

}

Bot::~Bot()
{

}

ConstructionManager & Bot::Construction()
{
	return constructionManager;
}

StructuresManager & Bot::Structures()
{
	return structuresManager;
}

SupplyManager & Bot::Supply()
{
	return supplyManager;
}

ArmyManager & Bot::Army()
{
	return armyManager;
}

EconManager & Bot::Econ()
{
	return econManager;
}

DrawingManager & Bot::Draw()
{
	return drawingManager;
}

BaseLocationManager & Bot::BaseLocations()
{
	return baseLocationManager;
}

UpgradesManager & Bot::Upgrades()
{
	return upgradesManager;
}

BuildQueueManager & Bot::BuildQueue()
{
	return buildQueueManager;
}

DataManager & Bot::Data()
{
	return dataManager;
}

MorphManager & Bot::Morph()
{
	return morphManager;
}

MapManager & Bot::Map()
{
	return mapManager;
}

void Bot::OnGameFullStart()
{
}

void Bot::OnGameStart()
{
	std::cout << "Initializing FradBot version " << GetVersion() << std::endl;
	time_t rawTime;
	time(&rawTime);
	struct tm timeinfo;
	localtime_s(&timeinfo, &rawTime);
	std::cout << "* Game begun @ " << std::put_time(&timeinfo, "%F %T") << " local time" << std::endl;
	//TODO:  No data ever shows up.
	//std::cout << "* Map:  " << GameSettings().map_name << std::endl;

	//Order added is order they'll get notifications and steps
	managers.push_back(&strategyManager);
	managers.push_back(&econManager);
	managers.push_back(&supplyManager);
	managers.push_back(&armyManager);
	managers.push_back(&constructionManager);
	managers.push_back(&structuresManager);
	managers.push_back(&upgradesManager);
	managers.push_back(&baseLocationManager);
	managers.push_back(&buildQueueManager);
	//Intentionally not giving events to these
	//managers.push_back(&drawingManager);
	//managers.push_back(&dataManager);
	//managers.push_back(&morphManager);
	//managers.push_back(&mapManager);

	Timer t;
	//Let everyone know the game has started
	for (ManagerBase* m : managers) {
		m->OnGameStart();
	}
	std::cout << "OnGameStart finished in " << t.ElapsedMs() << " ms" << std::endl;
}

void Bot::OnGameEnd()
{
	std::cout << "Goodbye, World!" << std::endl;
}

void Bot::OnError(const std::vector<sc2::ClientError>& client_errors, const std::vector<std::string>& protocol_errors /*= {}*/)
{
}

void Bot::OnStep() {
	Timer t;
	//Pass on the step to each manager
	for (ManagerBase* m : managers) {
		m->OnStep();
	}

	//Output avg frame time
	std::ostringstream oss;
	oss << "Last 20 Frames Avg:  " << last20GameLoopsAvgTimeMs << std::endl;
	Draw().DrawTextAtScreenPosition(oss.str(), Point2D(0.8f, 0.04f));

	//Sends all batched debug commands for all managers
	Debug()->SendDebug();

	int64_t msElapsed = t.ElapsedMs();
	if (msElapsed > stepWarningThresholdMs) {
		std::cout << "WARNING:  Step exceeds warning threshold @ " << msElapsed << "ms" << std::endl;
	}

	//FUTURE:  track avg speed over time.
	last20GameLoopsTotalTimeMs += msElapsed;
	if (Observation()->GetGameLoop() % 20 == 0) {
		last20GameLoopsAvgTimeMs = static_cast<int64_t>(last20GameLoopsTotalTimeMs / 20.0); 
		last20GameLoopsTotalTimeMs = 0;
	}
}

void Bot::OnUnitDestroyed(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitDestroyed(unit);
	}
}

void Bot::OnUnitCreated(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitCreated(unit);
	}
}

void Bot::OnUnitIdle(const Unit* unit) {
	for (ManagerBase* m : managers) {
		m->OnUnitIdle(unit);
	}
}

void Bot::OnUpgradeCompleted(UpgradeID upgradeID)
{
	for (ManagerBase* m : managers) {
		m->OnUpgradeCompleted(upgradeID);
	}
}

void Bot::OnBuildingConstructionComplete(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnBuildingConstructionComplete(unit);
	}
}

void Bot::OnNydusDetected()
{
	for (ManagerBase* m : managers) {
		m->OnNydusDetected();
	}
}

void Bot::OnNuclearLaunchDetected()
{
	for (ManagerBase* m : managers) {
		m->OnNuclearLaunchDetected();
	}
}

void Bot::OnUnitEnterVision(const Unit* unit)
{
	for (ManagerBase* m : managers) {
		m->OnUnitEnterVision(unit);
	}
}

std::string Bot::GetVersion()
{
	return "0.1.0";
}

// Returns a pointer to a class deriving from sc2::Agent
void *CreateNewAgent(void)
{
	return new Bot();
}

// Returns a string identifier for the agent name
const char *GetAgentName(void)
{
	return "FradBot";
}

// Returns the agents prefered race.  should be sc2::Race cast to int.
int GetAgentRace(void)
{
	//Hardcoded for now
	return static_cast<int>(sc2::Race::Terran);
}
