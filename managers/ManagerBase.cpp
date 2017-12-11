#include "ManagerBase.h"

ManagerBase::ManagerBase()
{
	bot = nullptr;
}

void ManagerBase::Initialize(Agent* b)
{
	bot = b;
}

const ObservationInterface* ManagerBase::Observation()
{
	return bot->Observation();
}

ActionInterface* ManagerBase::Actions()
{
	return bot->Actions();
}