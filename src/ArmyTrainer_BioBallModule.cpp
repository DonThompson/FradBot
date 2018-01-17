#include "ArmyTrainer_BioBallModule.h"
#include "bot.h"
using namespace sc2;

ArmyTrainer_BioBallModule::ArmyTrainer_BioBallModule(Bot & b)
	: ModuleBase(b)
{
}

//We want idle notifications and a refresher every requiredStepLoopCount frames just in case.
ModuleNotificationRequirement ArmyTrainer_BioBallModule::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, requiredStepLoopCount, true, false, false, false, false, false, false, false);
}

//Only called as often as defined above in requiredStepLoopCount
void ArmyTrainer_BioBallModule::OnStep()
{
	std::vector<Structure> allRax = GetBot().Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_BARRACKS);
	std::vector<Structure> allFact = GetBot().Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_FACTORY);
	std::vector<Structure> allPorts = GetBot().Structures().GetStructuresByType(UNIT_TYPEID::TERRAN_STARPORT);

	//Any that has no orders needs to get an opportunity to build
	for (Structure rax : allRax) {
		if (rax.getOrderCount() == 0) {
			OnBarracksIdle(rax);
		}
	}

	for (Structure fact : allFact) {
		if (fact.getOrderCount() == 0) {
			OnFactoryIdle(fact);
		}
	}

	for (Structure port : allPorts) {
		if (port.getOrderCount() == 0) {
			OnStarportIdle(port);
		}
	}
}

void ArmyTrainer_BioBallModule::OnUnitIdle(const sc2::Unit* unit)
{
	switch (unit->unit_type.ToType()) {
	case UNIT_TYPEID::TERRAN_BARRACKS:
		OnBarracksIdle(Structure(unit));
		break;
	case UNIT_TYPEID::TERRAN_FACTORY:
		OnFactoryIdle(Structure(unit));
		break;
	case UNIT_TYPEID::TERRAN_STARPORT:
		OnStarportIdle(Structure(unit));
		break;
	default:
		//Only worried about the above
		break;
	}
}

ArmyTrainer_BioBallModule::CurrentBioArmyData ArmyTrainer_BioBallModule::GetCurrentData()
{
	ArmyTrainer_BioBallModule::CurrentBioArmyData data;
	data.supplyUsed = GetBot().Observation()->GetFoodUsed();
	data.supplyCap = GetBot().Observation()->GetFoodCap();
	data.cntMarines = Utils::CountOwnUnits(GetBot().Observation(), UNIT_TYPEID::TERRAN_MARINE);
	data.cntMarauders = Utils::CountOwnUnits(GetBot().Observation(), UNIT_TYPEID::TERRAN_MARAUDER);
	data.cntMedivacs = Utils::CountOwnUnits(GetBot().Observation(), UNIT_TYPEID::TERRAN_MEDIVAC);
	data.cntGhosts = Utils::CountOwnUnits(GetBot().Observation(), UNIT_TYPEID::TERRAN_GHOST);
	return data;
}

//TODO:  Ugly code, needs cleaned up
void ArmyTrainer_BioBallModule::OnBarracksIdle(Structure rax)
{
	//See what we already have
	ArmyTrainer_BioBallModule::CurrentBioArmyData data = GetCurrentData();

	//If supply is capped, quit early
	if (data.supplyUsed >= data.supplyCap)
		return;

	//Is there an reactor?
	if (rax.HasReactor(GetBot())) {
		//Always train two marines from a reactored barracks
		GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_MARINE);
		GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_MARINE);
		return;
	}

	//Is there a tech lab?
	if (rax.HasTechLab(GetBot())) {
		//Marauders first (3 to every 5 marines)
		//Ghosts second (1 to every 5 marines)

		//do we have too many marauders?  We want a rough ratio of 3 marauders for every 5 marines
		float_t ratioMarauders = data.cntMarauders / 3.0f;
		float_t ratioMarines = data.cntMarines / 5.0f;
		if (ratioMarauders > ratioMarines) {
			//Check to see if we should build ghosts (and if we can build ghosts)
			float_t ghostRatio = static_cast<float_t>(data.cntMarines) / static_cast<float_t>(data.cntGhosts);
			if (ghostRatio > 5.0f && rax.HasAbilityAvailable(GetBot(), ABILITY_ID::TRAIN_GHOST)) {
				//We need more ghosts
				GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_GHOST);
				return;
			}
			else {
				//we have enough marauders and ghosts, make a marine instead
				GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_MARINE);
				return;
			}
		}
		else {
			//More marauders
			GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_MARAUDER);
			return;
		}
	}

	//If neither, we can only train marines
	GetBot().Actions()->UnitCommand(rax.building, ABILITY_ID::TRAIN_MARINE);
}

void ArmyTrainer_BioBallModule::OnFactoryIdle(Structure fact)
{

}

void ArmyTrainer_BioBallModule::OnStarportIdle(Structure port)
{
	//See what we already have
	ArmyTrainer_BioBallModule::CurrentBioArmyData data = GetCurrentData();

	//If supply is capped, quit early
	if (data.supplyUsed >= data.supplyCap)
		return;

	//1 medivac for every 5 marines
	float_t ratio = static_cast<float_t>(data.cntMarines) / static_cast<float_t>(data.cntMedivacs);
	if (ratio > 5.0f) {
		//Build a new medivac
		GetBot().Actions()->UnitCommand(port.building, ABILITY_ID::TRAIN_MEDIVAC);
		if (port.HasReactor(GetBot())) {
			//Train a second
			GetBot().Actions()->UnitCommand(port.building, ABILITY_ID::TRAIN_MEDIVAC);
		}
	}
}
