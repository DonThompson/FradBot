#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;

class Structure
{
public:
	Structure(const Unit* _building);

	//Start to map out pass throughs
	int32_t assignedHarvesters() {		return building->assigned_harvesters;	}
	int32_t idealHarvesters()	{		return building->ideal_harvesters;		}
	size_t getOrderCount()		{		return building->orders.size();			}
	float_t buildProgress()		{		return building->build_progress;		}
	Point2D buildingPosition()	{		return building->pos;					}
	UnitTypeID getUnitTypeID()	{		return building->unit_type;				}


	bool IsBuildingComplete();
	//Is the given building in progress?  Returns false if the building is unstarted (0% completion) or done (100% completion)
	bool IsBuildingInProgress();

public:
	//TODO:  Goal to get rid of this
	operator const Unit*();
	//TODO:  and make this private
	const Unit* building;
};

