#include "Structure.h"

Structure::Structure(const Unit* _building)
{
	building = _building;
}

Structure::~Structure()
{
}

Structure::operator const Unit*()
{
	return building;
}
