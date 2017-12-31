#pragma once

#include <sc2api/sc2_api.h>

//Represents a set of known minerals grouped together on the map.
class MineralLine
{
	//Readability inside the class
	typedef const sc2::Unit* MineralPatch;

public:
	//Initialize the class with your first mineral patch
	MineralLine(MineralPatch startingMineralPatch);
	//Are the given minerals near this mineral line?  true if so.
	bool AreTheseMineralsNear(MineralPatch mineralPatch);
	//Retrieves the current center of the mineral line.  This center will shift as minerals are added to the line.
	sc2::Point3D GetMineralCenterLocation();
	//Adds a new mineral patch to the line.  Callers should test to ensure it's near before adding.
	void AddMineralPatch(MineralPatch patch);
	//Retrieve all mineral patches in this line
	std::vector<MineralPatch> GetMineralPatches();

private:
	//Internal:  Update the center of the mineral line as patches are added.
	void UpdateMineralCenterLocation();
	//The approximate center of your mineral line based on all patches added so far.
	sc2::Point3D mineralCenterLocation;
	//Collection of mineral patches in this line.
	std::vector<MineralPatch> mineralPatches;
	//Rough guesstimate based on looking at maps.  This needs to be far enough that any 2 end mineral patches
	//	are within this distance but not far enough that nearby expansions get combined into one.
	const float_t baseRadius = 12.0f;
};
