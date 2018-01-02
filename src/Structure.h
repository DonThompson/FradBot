#pragma once

#include <sc2api/sc2_api.h>
class Bot;

class Structure
{
public:
	Structure();
	Structure(const sc2::Unit* _building);

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

public:
	//TODO:  and make this private.  Lots of use like Actions()->UnitCommand(.... structure)
	const sc2::Unit* building;
};

