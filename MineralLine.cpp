#include "MineralLine.h"
using namespace sc2;

MineralLine::MineralLine(MineralPatch startingMineralPatch)
{
	//Simply add it to our list as the first.
	AddMineralPatch(startingMineralPatch);
}

bool MineralLine::AreTheseMineralsNear(MineralPatch mineralPatch)
{
	//Use our defined base radius to determine suitability
	if (Distance3D(mineralCenterLocation, mineralPatch->pos) < baseRadius) {
		return true;
	}
	return false;
}

Point3D MineralLine::GetMineralCenterLocation()
{
	return mineralCenterLocation;
}

void MineralLine::AddMineralPatch(MineralPatch patch)
{
	mineralPatches.push_back(patch);
	UpdateMineralCenterLocation();
}

//Updates the location of the center of the mineral line by evaluating all known 
//	mineral patches at this base.
void MineralLine::UpdateMineralCenterLocation()
{
	float_t totalX = 0.0f;
	float_t totalY = 0.0f;
	float_t lastZ = 0.0f;
	for (MineralPatch patch : mineralPatches) {
		totalX += patch->pos.x;
		totalY += patch->pos.y;
		//We'll just use the last Z we happen to see.
		lastZ = patch->pos.z;
	}

	mineralCenterLocation = Point3D(
		totalX / mineralPatches.size(),
		totalY / mineralPatches.size(),
		lastZ
	);
}

std::vector<MineralLine::MineralPatch> MineralLine::GetMineralPatches()
{
	return mineralPatches;
}
