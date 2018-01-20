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

void MilitaryManagerV1Module::OnStep()
{
	for (shared_ptr<Platoon> platoon : GetBot().Army().armyPlatoons) {
		//Don't change orders at this point, let them keep working
		if (platoon->HasOrders())
			continue;

		//Make sure we have at least 12 units to get started
		//TODO:  Random made up number.  Improve.
		if (platoon->GetTotalPlatoonUnitCount() < 12)
			continue;

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
			//No known bases, we'll have to guess.  Pick a random base location
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


/* Army manager v2 HISTORY REFERENCE TODO DELETE
	//V2:  Attack @ ~12 units in each platoon
	for (shared_ptr<Platoon> platoon : GetBot().Army().armyPlatoons) {
		//Attack if we're big enough
		if (platoon->GetTotalPlatoonUnitCount() >= 12 && !platoon->HasOrders()) {
			//TODO:  Position.  Picking the enemy start for now
			Point2D targetPoint = GetBot().Observation()->GetGameInfo().enemy_start_locations.front();
			platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::ATTACK, targetPoint));
		}
		//defend otherwise
		else if (platoon->GetTotalPlatoonUnitCount() >= 1 && !platoon->HasOrders()) {
			//TODO:  Position.  Picking the highest natural choke for now
			Point2D targetPoint;
			std::vector<Point2D> chokes = GetBot().Map().GetRegionChokePoints(GetBot().BaseLocations().Natural()->GetRegionId());
			if (chokes.size() > 0) {
				targetPoint = chokes[chokes.size() - 1];
			}
			else {
				//TODO
				std::cout << "WARNING:  No choke points available to set defense target" << std::endl;
			}
			platoon->SetOrders(PlatoonOrders(PlatoonOrders::ORDER_TYPE::DEFEND, targetPoint));
		}
	}
*/
}
