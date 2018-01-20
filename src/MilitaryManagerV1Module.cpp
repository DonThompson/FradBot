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

}
