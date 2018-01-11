#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"
#include "sc2utils/sc2_arg_parser.h"
#include "../src/bot.h"
#include <iostream>

/* Build Instructions
	* Messy for now, need to clean this up
	* FradBotLM should have just this file.  Delete all other source references.
	* Now drag the entirety of the source files into this project somewhere.
	* Compile.
	
*/

struct ConnectionOptions
{
	int32_t GamePort;
	int32_t StartPort;
	std::string ServerAddress;
};

void ParseArguments(int argc, char *argv[], ConnectionOptions &connect_options)
{
	sc2::ArgParser arg_parser(argv[0]);
	arg_parser.AddOptions({
		{ "-g", "--GamePort", "Port of client to connect to", false },
		{ "-o", "--StartPort", "Starting server port", false },
		{ "-l", "--LadderServer", "Ladder server address", false },
	});
	arg_parser.Parse(argc, argv);
	std::string GamePortStr;
	if (arg_parser.Get("GamePort", GamePortStr)) {
		connect_options.GamePort = atoi(GamePortStr.c_str());
	}
	std::string StartPortStr;
	if (arg_parser.Get("StartPort", StartPortStr)) {
		connect_options.StartPort = atoi(StartPortStr.c_str());
	}
	arg_parser.Get("LadderServer", connect_options.ServerAddress);
}

//*************************************************************************************************
int main(int argc, char* argv[]) {
	ConnectionOptions Options;
	ParseArguments(argc, argv, Options);

	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(argc, argv)) {
		return 1;
	}

	// Add the custom bot, it will control the players.
	Bot bot;
	coordinator.SetParticipants({
		CreateParticipant(sc2::Race::Terran, &bot),
	});

	// Start the game.

	// Step forward the game simulation.
	std::cout << "Connecting to port " << Options.GamePort << std::endl;
	coordinator.Connect(Options.GamePort);
	coordinator.SetupPorts(2, Options.StartPort, false);
	// Step forward the game simulation.
	coordinator.JoinGame();
	coordinator.SetTimeoutMS(10000);
	std::cout << " Successfully joined game" << std::endl;
	while (coordinator.Update()) {
	}

	return 0;
}