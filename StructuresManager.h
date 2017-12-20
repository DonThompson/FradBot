#pragma once

#include <iostream>
#include <sc2api/sc2_api.h>
using namespace sc2;
#include "managers/ManagerBase.h"
#include "utils/Utils.h"

class Bot;

class StructuresManager : public ManagerBase
{
public:
	StructuresManager(Bot & b);

	virtual void OnStep();

};

