#include "Structure.h"

//Float calculations
//	I mostly ran empirical tests and compared against what others have done for float-type safety in determining where things stand.
//	Generally, it seems safe to assume 0.0f is set if progress hasn't started yet and 1.0f is set if the building is complete.

Structure::Structure()
{
	building = nullptr;
}

Structure::Structure(const Unit* _building)
{
	building = _building;
}

bool Structure::IsBuildingComplete() {
	if (building->build_progress >= 1.0f)
		return true;
	return false;
}

bool Structure::IsBuildingInProgress()
{
	if (building->build_progress >= 1.0f) {
		//Building is done
		return false;
	}

	if (building->build_progress > 0.0f ) {
		//Building is in progress.
		return true;
	}

	//Building is unstarted, it is also not in progress.  It may become in progress in the next loop.
	return false;
}
