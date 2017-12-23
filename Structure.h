#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;

class Structure
{
public:
	Structure(const Unit* _building);
	~Structure();

	//Start to map out pass throughs
	int32_t assignedHarvesters() {		return building->assigned_harvesters;	}
	int32_t idealHarvesters() {			return building->ideal_harvesters;		}

public:
	//TODO:  Goal to get rid of this
	operator const Unit*();
	//TODO:  and make this private
	const Unit* building;
};

