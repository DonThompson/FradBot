#include "WorkerProducerSearch.h"
#include <sc2api/sc2_api.h>
#include "bot.h"
using namespace sc2;

WorkerProducerSearch::WorkerProducerSearch(Bot & b)
	: bot(b)
{
}

Structure* WorkerProducerSearch::SearchForProducer()
{
	Structure* buildFrom = nullptr;

	Structure &mainDepot = bot.BaseLocations().Main()->GetResourceDepot();

	//Use an ordered set of bases
	std::vector<Structure*> structures;
	structures.push_back(&mainDepot);
	if (bot.BaseLocations().Natural() != nullptr) {
		structures.push_back(&(bot.BaseLocations().Natural()->GetResourceDepot()));
	}
	for (BaseLocation* base : bot.BaseLocations().OtherBases()) {
		structures.push_back(&(base->GetResourceDepot()));
	}

	//Find the first that needs workers and that is not training one currently
	for (Structure* s : structures) {
		if (s->NeedsMoreWorkers() && !s->IsTrainingUnit(worker)) {
			buildFrom = s;
			break;
		}
	}

	//If we didn't find one, give the main one more try -- we no longer care if he's full.
	if (buildFrom == nullptr && !mainDepot.IsTrainingUnit(worker)) {
		//Found nothing!  Default back to our main base
		buildFrom = &mainDepot;
	}
	return buildFrom;
}
