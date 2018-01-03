#pragma once
#include <sc2api/sc2_api.h>

class Structure;
class Bot;

//Simple search class to find a producer for a worker
class WorkerProducerSearch
{
public:
	WorkerProducerSearch(Bot & b);
	//Rules:
	//	Always start with main, then natural, then all other bases.
	//	Never queue units, build them as others finish.  Utilize our super APM!
	Structure* SearchForProducer();

private:
	Bot& bot;
	const sc2::ABILITY_ID worker = sc2::ABILITY_ID::TRAIN_SCV;		//fix later if we go non-terran

};
