#pragma once
#include "ModuleBase.h"
class Bot;

class IdleWorkerModule : public ModuleBase
{
public:
	IdleWorkerModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnUnitIdle(const sc2::Unit* unit);

private:
	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start);
	void OnSCVIdle(const sc2::Unit* unit);
};
