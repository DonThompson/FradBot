#include "Structure.h"

Structure::Structure(const Unit* _building)
{
	building = _building;
}

Structure::operator const Unit*()
{
	return building;
}

//TODO:  What's the right float calc here?
bool Structure::IsBuildingComplete() {
	if (building->build_progress >= 1.0f)
		return true;
	return false;
}

bool Structure::IsBuildingInProgress()
{
	if (building->build_progress >= 0.9999f) {
		//Building is done
		return false;
	}

	if (building->build_progress < 0.0001f) {
		//Building is unstarted, it is also not in progress.  It may become in progress in the next loop.
		return false;
	}

	return true;
}
