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


private:
	std::vector<BaseLocation> baseLocations;

	//Base location initialization
	void InitializeBaseLocations();
	void CreateBaseLocationsFromResourceNodes(std::vector<const Unit*> nodes);
	void FindAllMineralsAndGeysers(std::vector<const Unit*> *mineralPatches, std::vector<const Unit*> *geysers);
};
