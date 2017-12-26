#pragma once

#include <sc2api/sc2_api.h>
#include "ManagerBase.h"
#include "BaseLocation.h"

class BaseLocationManager : public ManagerBase
{
public:
	BaseLocationManager(Bot & b);
	
	virtual void OnStep();
	virtual void OnGameStart();

	BaseLocation* GetLocationById(uint32_t baseId);
	BaseLocation* GetLocationByPosition(sc2::Point2D pt);

private:
	std::vector<BaseLocation> baseLocations;

	void InitializeKnownEnemyBase();
	void InitializeNaturalExpansions();
};
