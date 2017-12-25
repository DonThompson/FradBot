#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"
#include "BaseLocation.h"

class BaseLocationManager : public ManagerBase
{
public:
	BaseLocationManager(Bot & b);
	
	virtual void OnStep();
	virtual void OnGameStart();

	BaseLocation GetLocationById(uint32_t baseId);

private:
	std::vector<BaseLocation> baseLocations;
};
