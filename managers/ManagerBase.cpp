#include "ManagerBase.h"
#include "../bot.h"

ManagerBase::ManagerBase(Bot & b)
	: bot(b)
{
}

const ObservationInterface* ManagerBase::Observation()
{
	return bot.Observation();
}

ActionInterface* ManagerBase::Actions()
{
	return bot.Actions();
}