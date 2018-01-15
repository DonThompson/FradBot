#pragma once
#include "ModuleBase.h"
class Bot;

//Module that monitors for idle workers.  Will put them back to work on the closest possible mineral patch, preferring
//	to do so inside the base that worker is already in.
class IdleWorkerModule : public ModuleBase
{
public:
	explicit IdleWorkerModule(Bot & b);
	virtual ModuleNotificationRequirement GetNotificationRequirements();
	virtual void OnUnitIdle(const sc2::Unit* unit);

private:
	const sc2::Unit* FindNearestMineralPatch(const sc2::Point2D& start);
	const sc2::Unit* FindNearestMineralPatch__IDEAL_NOT_USED(const sc2::Point2D& start);
	void OnSCVIdle(const sc2::Unit* unit);
};
