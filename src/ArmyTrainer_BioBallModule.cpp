#include "ArmyTrainer_BioBallModule.h"
#include "BuildQueueItem.h"
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

//Use the build system.  This ensures that we don't monopolize the buildings and keep others from doing
//	useful things like flying, building addons, etc.  
//TODO:  My initial thought was that we should have the build queue enforce "build now, don't queue" when the source is provided.  But
//	I'm not sure if that's needed, and it's not in place right now.  Monitor.
void ArmyTrainer_BioBallModule::TrainUnitToBuildQueue(sc2::ABILITY_ID abilityID, Structure s)
{
	//TODO:  This isn't quite working out as I'd hoped.  A few holes...
	//	The build queue may already have items it's waiting on.  For example, an expansion.  In that case, our idle rax will just wait
	// on that to finish.  Also, it doesn't 'get' reactors at the moment, so it queues 1 marine in it... then the other sits in the queue
	//	blocking everything else.  The concept is there, but we're missing some work to make it function.  I'll keep the code, and the
	//	slightly confusing name of this function, but we're just going to build straight up.
	//TODO:  The whole point of this was so that the build queue could do addons and such.  We're pretty stuck with that at the moment.  Once
	//	you enable this module, you can't really get another addon built.
	GetBot().Actions()->UnitCommand(s.building, abilityID);
	//GetBot().BuildQueue().Push(std::make_shared<BuildQueueItem>(abilityID, s.building));
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

void ArmyTrainer_BioBallModule::OnBarracks_Reactor_Idle(Structure rax)
{
	//Always train two marines from a reactored barracks
	TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MARINE, rax);
	TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MARINE, rax);
}

void ArmyTrainer_BioBallModule::OnBarracks_TechLab_Idle(Structure rax, ArmyTrainer_BioBallModule::CurrentBioArmyData & data)
{
	//Marauders first (3 to every 5 marines)
	//Ghosts second (1 to every 5 marines)

	//do we have too many marauders?  We want a rough ratio of 3 marauders for every 5 marines
	float_t ratioMarauders = data.cntMarauders / 3.0f;
	float_t ratioMarines = data.cntMarines / 5.0f;
	if (ratioMarauders > ratioMarines) {
		//We have enough marauders compared to marines.  Let's check out ghosts instead (also confirm we have ability to build ghosts)
		float_t ghostRatio = static_cast<float_t>(data.cntMarines) / static_cast<float_t>(data.cntGhosts);
		if (ghostRatio > 5.0f && rax.HasAbilityAvailable(GetBot(), ABILITY_ID::TRAIN_GHOST)) {
			//We need more ghosts
			TrainUnitToBuildQueue(ABILITY_ID::TRAIN_GHOST, rax);
			return;
		}
		else {
			//we have enough marauders and ghosts, make a marine instead, it's all we've got left.
			TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MARINE, rax);
			return;
		}
	}
	else {
		//More marauders are needed
		TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MARAUDER, rax);
		return;
	}
}

void ArmyTrainer_BioBallModule::OnBarracksIdle(Structure rax)
{
	//See what we already have
	ArmyTrainer_BioBallModule::CurrentBioArmyData data = GetCurrentData();

	//If supply is capped, quit early
	if (data.supplyUsed >= data.supplyCap)
		return;

	//Is there an reactor?
	if (rax.HasReactor(GetBot())) {
		OnBarracks_Reactor_Idle(rax);
	}
	//Is there a tech lab?
	else if (rax.HasTechLab(GetBot())) {
		OnBarracks_TechLab_Idle(rax, data);
	}
	else {
		//If there is no addon, we can only train marines.
		TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MARINE, rax);
	}
}

void ArmyTrainer_BioBallModule::OnFactoryIdle(Structure fact)
{
	//No factory units are used in the bio ball
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
		TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MEDIVAC, port);
		if (port.HasReactor(GetBot())) {
			//Train a second
			TrainUnitToBuildQueue(ABILITY_ID::TRAIN_MEDIVAC, port);
		}
	}
}
