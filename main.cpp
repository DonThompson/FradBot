#include <sc2api/sc2_api.h>
//#include <sc2utils/sc2_manage_process.h>        //Needed for SleepFor only
#include <iostream>
using namespace sc2;
#include "bot.h"

int main(int argc, char* argv[])
{
	//seed randomization
	srand(static_cast<unsigned int>(time(NULL)));


	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);

	Bot bot;
	coordinator.SetParticipants({
		CreateParticipant(Race::Terran, &bot),
		CreateComputer(Race::Terran, sc2::Medium)
	});

	//Slow down the damn game to a more normal speed
	//https://github.com/Blizzard/s2client-api/issues/179
	//However this also has the effect where a variable number of GameLoops may pass between calls to your OnStep depending 
	//  on how slow your logic is. If this is undesirable I suggest alkurbatov's solution.
	//coordinator.SetRealtime(true);

	coordinator.LaunchStarcraft();
	coordinator.StartGame(sc2::kMapBelShirVestigeLE);

	while (coordinator.Update())
	{
		//keep updating

		//Use this and you can control a little better than the "realtime" flag if you want to go fast
		//alkurbatov says:
		//sc2::SleepFor(15);
	}

	return 0;
}
