#include "ModuleCreator.h"
#include "bot.h"
#include "ModuleBase.h"

//Add ridiculous number of includes here
#include "VespeneWorkerBalanceModule.h"
#include "IdleWorkerModule.h"
#include "AutoBuildWorkersModule.h"
#include "ArmyTrainer_BioBallModule.h"
using namespace std;


//EVIL MACROS!  BUT IT SURE MAKES IT EASIER TO READ

//CreateSingleModule - enabled
#define CSM_E(moduleID, moduleClassName) CreateSingleModule(bot, moduleID, make_shared<moduleClassName>(bot), true);
//CreateSingleModule - disabled
#define CSM(moduleID, moduleClassName) CreateSingleModule(bot, moduleID, make_shared<moduleClassName>(bot));


/*static*/void ModuleCreator::CreateAllGameModules(Bot & bot)
{
	//The following modules are always on, enabled from the beginning.
	CSM_E(MODULE_ID::VESPENEWORKERBALANCEMODULE, VespeneWorkerBalanceModule);
	CSM_E(MODULE_ID::IDLEWORKERMODULE, IdleWorkerModule);

	//All other modules need to be enabled when ready
	CSM(MODULE_ID::AUTOBUILDWORKERSMODULE, AutoBuildWorkersModule);
	CSM(MODULE_ID::ARMYTRAINER_BIOBALLMODULE, ArmyTrainer_BioBallModule);
}

/*static*/void ModuleCreator::CreateSingleModule(Bot & bot, MODULE_ID moduleID, shared_ptr<ModuleBase> gameModule, bool enabled/*= false*/)
{
	//Enable the module by default if it should be
	if (enabled) {
		gameModule->EnableModule();
	}

	//Now register it with the bot
	bot.Modules().RegisterModule(moduleID, gameModule);
}
