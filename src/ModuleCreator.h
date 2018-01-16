#pragma once
#include "BuildQueueEnums.h"
#include <memory>
class Bot;
class ModuleBase;

//Initialization class, to be used once.  This class creates all modules that can be used in the game, enables/disables per default setting, 
//	registers them with the ModuleManager and ensures they are configured for notifications.
class ModuleCreator
{
public:
	static void CreateAllGameModules(Bot & b);

private:
	static void CreateSingleModule(Bot & b, MODULE_ID moduleID, std::shared_ptr<ModuleBase> gameModule, bool enabled = false);
};
