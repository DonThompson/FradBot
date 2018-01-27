#pragma once
#include <sc2api/sc2_api.h>
#include "ModuleBase.h"
class Bot;

//Manages building supply.  Works very simply - when a depot finishes, we build another one.  Continuous build until we hit max supply.
//	Auto disables when we hit 200 supply.
class SupplyDepotBuilderModule : public ModuleBase
{
public:
	SupplyDepotBuilderModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnBuildingConstructionComplete(const sc2::Unit* unit);
	virtual void OnUnitDestroyed(const sc2::Unit* unit);

private:
	void QueueSupplyDepot();
};
