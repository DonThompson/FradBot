#pragma once

#include <sc2api/sc2_api.h>
class Bot;

//TODO
//* Should "needs more workers" include gas?  Right now it's purely a mineral count.  For now we'll exclude it -- if there's no other base, we'll
//		just build at this one anyways, making enough.  If we start to move workers, we can rebuild them.


class Structure
{
public:
	Structure();
	explicit Structure(const sc2::Unit* _building);

	//Start to map out pass throughs
	int32_t assignedHarvesters();
	int32_t idealHarvesters();
	size_t getOrderCount()		{		return building->orders.size();			}
	float_t buildProgress()		{		return building->build_progress;		}
	sc2::Point2D buildingPosition2D(){		return static_cast<sc2::Point2D>(building->pos);	}
	sc2::Point3D buildingPosition3D(){		return building->pos;					}
	sc2::UnitTypeID getUnitTypeID()	{		return building->unit_type;				}


	bool IsBuildingComplete();
	//Is the given building in progress?  Returns false if the building is unstarted (0% completion) or done (100% completion)
	bool IsBuildingInProgress();
	//Can the building perform this action right now?
	//TODO:  Bleh, don't like having to load the bot here
	bool HasAbilityAvailable(Bot & bot, sc2::AbilityID abilityID);
	//Is the building currently training this ability?
	bool IsTrainingUnit(sc2::ABILITY_ID abilityID);
	//Only applies to resource worker producers
	bool NeedsMoreWorkers();
	//Gets the unit type of any addon building.  Returns INVALID if none
	sc2::UNIT_TYPEID GetAddonType(Bot & bot);
	//Simpler of the above - is there a reactor attached?
	bool HasReactor(Bot & bot);
	//Is there a tech lab attached?
	bool HasTechLab(Bot & bot);

public:
	//TODO:  and make this private.  Lots of use like Actions()->UnitCommand(.... structure)
	const sc2::Unit* building;
};
