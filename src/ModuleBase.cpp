#include "ModuleBase.h"

ModuleBase::ModuleBase(Bot & b)
	: bot(b)
	, enabled(false)
{
}

void ModuleBase::EnableModule()
{
	enabled = true;
}

void ModuleBase::DisableModule()
{
	enabled = false;
}

bool ModuleBase::IsEnabled()
{
	return enabled;
}
