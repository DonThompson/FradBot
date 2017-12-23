#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;

class Structure
{
public:
	Structure(const Unit* _building);
	~Structure();

	//TODO:  Goal to get rid of this
	operator const Unit*();

	const Unit* building;
};

