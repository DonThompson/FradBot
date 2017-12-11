#include <sc2api/sc2_api.h>
#include <sc2utils/sc2_manage_process.h>        //Needed for SleepFor only
#include <iostream>
using namespace sc2;
#include "bot.h"

int main(int argc, char* argv[])
{
	//
	// Settings here
	//

	//Configurable settings.  Default values
	sc2::Race opponentRace = sc2::Random;
	sc2::Difficulty opponentDifficulty = sc2::Medium;
	//0 = Super fast AI gameplay
	//1 = Regular speed, use 'SetRealtime' feature
	//2 = Regular speed, use 'SleepFor' feature
	int32_t gameMode = 0;
	std::string mapPath = sc2::kMapBelShirVestigeLE;

	//Currently unchangeable
	sc2::Race playerRace = sc2::Terran;

	//Parse command line to change any of the settings.  If no settings, go with default
	for (int i = 0; i < argc; i++) {
		std::string sArg(argv[i]);
		//Settings here
		if (sArg == "-ot") {
			opponentRace = sc2::Terran;
		}
		else if(sArg == "-oz") {
			opponentRace = sc2::Zerg;
		}
		else if(sArg == "-op") {
			opponentRace = sc2::Protoss;
		}
		else if (sArg == "-or") {
			opponentRace = sc2::Random;
		}
		else if(sArg == "-s0") {
			gameMode = 0;
		}
		else if(sArg == "-s1") {
			gameMode = 1;
		}
		else if(sArg == "-s2") {
			gameMode = 2;
		}
		else if (sArg == "-dVE") {
			opponentDifficulty = sc2::VeryEasy;
		}
		else if (sArg == "-dE") {
			opponentDifficulty = sc2::Easy;
		}
		else if (sArg == "-dM") {
			opponentDifficulty = sc2::Medium;
		}
		else if (sArg == "-dMH") {
			opponentDifficulty = sc2::MediumHard;
		}
		else if (sArg == "-dH") {
			opponentDifficulty = sc2::Hard;
		}
		else if (sArg == "-dHVH") {
			opponentDifficulty = sc2::HardVeryHard;
		}
		else if (sArg == "-dVH") {
			opponentDifficulty = sc2::VeryHard;
		}
		else if (sArg == "-dI") {
			opponentDifficulty = sc2::CheatInsane;
		}
		//TODO: Map
	}



	//
	// Core app functionality
	//

	//seed randomization
	srand(static_cast<unsigned int>(time(NULL)));

	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);

	Bot bot;
	coordinator.SetParticipants({
		CreateParticipant(playerRace, &bot),
		CreateComputer(opponentRace, opponentDifficulty)
	});

	if (gameMode == 1)
	{
		//Slow down the damn game to a more normal speed
		//https://github.com/Blizzard/s2client-api/issues/179
		//However this also has the effect where a variable number of GameLoops may pass between calls to your OnStep depending 
		//  on how slow your logic is. If this is undesirable I suggest alkurbatov's solution.
		coordinator.SetRealtime(true);
	}

	coordinator.LaunchStarcraft();
	coordinator.StartGame(mapPath);

	while (coordinator.Update())
	{
		//keep updating

		if (gameMode == 2)
		{
			//Use this and you can control a little better than the "realtime" flag if you want to go fast
			//alkurbatov says:
			sc2::SleepFor(15);
		}
	}

	return 0;
}
