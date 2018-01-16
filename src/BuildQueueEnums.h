#pragma once
enum class BUILD_QUEUE_TYPE {
	GAME_ABILITY = 1,			//Use a sc2::ABILITY_ID
	AUTO_ABILITY = 2,			//Enable autonomy for something
};

enum class AUTO_ABILITYID {
	INVALID = 0,

	ENABLE_AUTOBUILDWORKERS = 100,		//Enable autonomy for building workers
};

//Define all modules here.  For ease of readability, please name the enumeration identical to the class name in upper case
enum class MODULE_ID {
	INVALID = 0,

	VESPENEWORKERBALANCEMODULE = 1,
	IDLEWORKERMODULE = 2,
	AUTOBUILDWORKERSMODULE = 3,

};
