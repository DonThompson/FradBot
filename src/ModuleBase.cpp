#include "ModuleBase.h"

ModuleBase::ModuleBase()
	: enabled(false)
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
