#include "AutoBuildWorkersModule.h"
#include "Utils.h"
#include "Structure.h"
#include "bot.h"
using namespace sc2;



AutoBuildWorkersModule::AutoBuildWorkersModule(Bot & b)
	: ModuleBase(b)
{
}

//We want to be notified for idle command centers and will build workers as needed there
//We will additionally be notified every 200 game loops for the same purpose - just in case some workers are lost, run off to do something else, etc.
ModuleNotificationRequirement AutoBuildWorkersModule::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, 200, true, false, false, false, false, false, false, false);
}

void AutoBuildWorkersModule::OnStep()
{
	//Make sure to check all types of morphed command centers
	std::vector<Structure> ccs = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_COMMANDCENTER);
	std::vector<Structure> orbitals = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_ORBITALCOMMAND);
	std::vector<Structure> forts = bot.Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_PLANETARYFORTRESS);

	//No faster to bring them all into one array then iterate, so just do it the hard way
	for (Structure cc : ccs) {
		if (cc.getOrderCount() == 0) {
			OnCCIdle(cc);
		}
	}

	for (Structure cc : orbitals) {
		if (cc.getOrderCount() == 0) {
			OnCCIdle(cc);
		}
	}

	for (Structure cc : forts) {
		if (cc.getOrderCount() == 0) {
			OnCCIdle(cc);
		}
	}
}

void AutoBuildWorkersModule::OnUnitIdle(const sc2::Unit* unit)
{
	//Check for any type of morphed command center
	switch (unit->unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_COMMANDCENTER:
		case UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
		case UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
			OnCCIdle(Structure(unit));			break;
		default:																			break;	//Not interested in any other units
	}
}

//Ignore vespene.  The VespeneWorkerBalanceModule will take them as needed, pulling us back down, and allowing this module
//	to train more workers as needed.
void AutoBuildWorkersModule::OnCCIdle(Structure commandCenter)
{
	//Only build if we're short harvesters
	bool buildSCV = false;

	if (commandCenter.assignedHarvesters() < commandCenter.idealHarvesters()) {
		buildSCV = true;
	}

	if (buildSCV) {
		bot.Econ().TrainWorker(&commandCenter);
	}
}
