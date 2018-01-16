#pragma once
enum class BUILD_QUEUE_TYPE {
	GAME_ABILITY = 1,			//Use a sc2::ABILITY_ID
	GAME_MODULE = 2,			//Enable a game module.  See ModuleManager
};

//Define all modules here.  For ease of readability, please name the enumeration identical to the class name in upper case
enum class MODULE_ID {
	INVALID = 0,

	VESPENEWORKERBALANCEMODULE = 1,
	IDLEWORKERMODULE = 2,
	AUTOBUILDWORKERSMODULE = 3,

};
