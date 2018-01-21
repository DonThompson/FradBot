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
		
		//Setup offense

		//If we know of an enemy base, start there.
		BaseLocation* target = nullptr;
		vector<BaseLocation*> enemyBases = GetBot().BaseLocations().EnemyBases();
		if (enemyBases.size() > 0) {
			//If there's just 1, well, attack there.
			if (enemyBases.size() == 1) {
				target = enemyBases[0];
			}
			else {
				//Find the closest one.  Take the natural out before the main, etc.
				//TODO:  Using max distance instance of pathing distance.  Always compare to our main.
				BaseLocation* myMain = GetBot().BaseLocations().Main();
				float_t maxDistance = numeric_limits<float_t>::max();
				for (BaseLocation* base : enemyBases) {
					float_t d = Distance2D(myMain->GetResourceDepotLocation(), base->GetResourceDepotLocation());
					if (d < maxDistance) {
						maxDistance = d;
						target = base;
					}
				}

				//Should be impossible to miss here.  Safety check
				if (target == nullptr) {
					std::cout << "Failed to find enemy base target" << std::endl;
				}
			}
		}
		else {
			//There are no known enemy bases.  We must have cleared them all out.  Ideally a future scouting module will clear our need
			//	for this code.

			//Check 1:  Can we find any buildings at all?  Maybe they built a supply depot in the middle of nowhere or it's in our base or something.
			Units enemyBuildings = Utils::GetEnemyUnits(GetBot().Observation(), Utils::IsStructure(GetBot().Observation()));
			if (enemyBuildings.size() > 0) {
				//Screw it, just attack here directly - it's all we know.
				platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, enemyBuildings[0]->pos));
				return;
			}

			//Check 2:  Nothing, eh?  Well he must be hiding.  At this point just start spraying platoons to random base locations
			vector<BaseLocation*> allBasesNotMine = GetBot().BaseLocations().AllBasesNotMine();
			if (allBasesNotMine.size() > 0) {
				target = allBasesNotMine[rand() % allBasesNotMine.size()];
			}
		}

		if (target != nullptr) {
			std::vector<const Unit*> buildings = target->GetKnownEnemyBuildings();
			if (buildings.size() > 0) {
				//TODO:  Maybe find closest or something?  Pathing?  For now just grab one at random
				platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, buildings[0]->pos));
			}
			else {
				//No known buildings, maybe we haven't scouted the location yet.  Just aim at the resource depot.
				platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, target->GetResourceDepotLocation()));
			}
		}
	}
}
