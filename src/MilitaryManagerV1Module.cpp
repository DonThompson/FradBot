#include "MilitaryManagerV1Module.h"
#include "bot.h"
using namespace sc2;
using namespace std;

MilitaryManagerV1Module::MilitaryManagerV1Module(Bot & b)
	: ModuleBase(b)
{
}

ModuleNotificationRequirement MilitaryManagerV1Module::GetNotificationRequirements()
{
	return ModuleNotificationRequirement(false, 50, false, false, false, false, false, false, false, false);
}

void MilitaryManagerV1Module::AssignPlatoonToBaseDefense(shared_ptr<Platoon> platoon)
{
	//Find the choke nearest the center of the map - that's most likely where units will path in from.  Defend there.
	Point2D targetPoint = GetBot().Map().GetRegionChokeClosestToMapCenter(GetBot().BaseLocations().Natural()->GetRegionId());
	if(targetPoint == Point2D(0, 0)) {
		//If for some reason that fails, hang out near the natural.
		targetPoint = GetBot().BaseLocations().Natural()->GetResourceDepotLocation();
	}
	platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::DEFEND, targetPoint));
}

BaseLocation* MilitaryManagerV1Module::FindEnemyBaseToAttack()
{
	BaseLocation* target = nullptr;

	//Get a list of all known enemy bases.  Remember this is based on incomplete knowledge.
	vector<BaseLocation*> enemyBases = GetBot().BaseLocations().EnemyBases();
	if(enemyBases.size() == 0) {
		//No known bases, no targets
	}
	else if (enemyBases.size() == 1) {
		//If there's just 1, well, attack there.
		target = enemyBases[0];
	}
	else {
		//Find the closest one.  Take the natural out before the main, etc.
		//	It should be safe to use raw distance here over pathing distance.  We always compare distance from our main.
		BaseLocation* myMain = GetBot().BaseLocations().Main();
		float_t minDistance = numeric_limits<float_t>::max();
		for (BaseLocation* base : enemyBases) {
			float_t distance = Distance2D(myMain->GetResourceDepotLocation(), base->GetResourceDepotLocation());
			if (distance < minDistance) {
				minDistance = distance;
				target = base;
			}
		}
	}

	return target;
}

Point2D MilitaryManagerV1Module::FindNonBaseAttackPoint()
{
	Point2D targetPoint(0, 0);

	//There are no known enemy bases.  We must have cleared them all out.  Ideally a future scouting module will clear our need
	//	for this code.

	//Check 1:  Can we find any buildings at all?  Maybe they built a supply depot in the middle of nowhere or it's in our base or something.
	Units enemyBuildings = Utils::GetEnemyUnits(GetBot().Observation(), Utils::IsStructure(GetBot().Observation()));
	if (enemyBuildings.size() > 0) {
		//Screw it, just attack the first building directly - it's all we know.  Hopefully we find others near it.
		targetPoint = enemyBuildings[0]->pos;
	}
	else {
		//Check 2:  Nothing, eh?  Well he must be hiding.  At this point just start spraying platoons to random base locations
		vector<BaseLocation*> allBasesNotMine = GetBot().BaseLocations().AllBasesNotMine();
		if (allBasesNotMine.size() > 0) {
			targetPoint = allBasesNotMine[rand() % allBasesNotMine.size()]->GetResourceDepotLocation();
		}
	}
	return targetPoint;
}

void MilitaryManagerV1Module::AssignPlatoonToAttack(shared_ptr<Platoon> platoon)
{
	BaseLocation* target = nullptr;
	//If we know of an enemy base, start there.
	target = FindEnemyBaseToAttack();
	//Did we get one?
	if(target == nullptr) {
		Point2D targetPoint = FindNonBaseAttackPoint();
		if (targetPoint != Point2D(0, 0)) {
			//We don't have a base target, so target a point and quit out.
			platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, targetPoint));
			return;
		}
	}

	//We have a target base - what should we target in that base?
	if (target != nullptr) {
		AttackTargetBaseLocationWithPlatoon(target, platoon);
	}
}

void MilitaryManagerV1Module::AttackTargetBaseLocationWithPlatoon(BaseLocation * target, std::shared_ptr<Platoon> &platoon)
{
	std::vector<const Unit*> buildings = target->GetKnownEnemyBuildings();
	if (buildings.size() > 0) {
		//Ideally we should be targeting the building closest to us - that ensures we get new orders as needed and the squads are less
		//	likely to run off after something else.
		//TODO:  For now we're just picking the first building for simplicity.
		platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, buildings[0]->pos));
	}
	else {
		//No known buildings, maybe we haven't scouted the location yet.  Just aim at the resource depot.
		platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, target->GetResourceDepotLocation()));
	}
}

void MilitaryManagerV1Module::OnStep()
{
	for (shared_ptr<Platoon> platoon : GetBot().Army().armyPlatoons) {
		//Don't change orders at this point, let them keep working
		if (platoon->HasOrders())
			continue;

		//Make sure we have at least 12 units to get started
		//TODO:  Random made up number.  Improve.
		if (platoon->GetTotalPlatoonUnitCount() < 12) {
			//Setup defense
			AssignPlatoonToBaseDefense(platoon);
			continue;
		}
		
		//Platoon is large enough, let's send them on the attack
		AssignPlatoonToAttack(platoon);
	}
}
