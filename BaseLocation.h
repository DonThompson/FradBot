#pragma once

#include <sc2api/sc2_api.h>

class Bot;

class BaseLocation
{
public:
	BaseLocation(uint32_t _baseLocationId, sc2::Point3D _resourceDepotLocation);

	bool IsUnitInBase(const sc2::Unit* unit);
	bool IsPointInBase(sc2::Point3D pt);
	bool IsPointInBase(sc2::Point2D pt);

	void AddMineralPatch(const sc2::Unit* patch);
	void AddGeyser(const sc2::Unit* geyser);

	void DrawSelf(Bot & bot);

	uint32_t GetBaseLocationId();
	sc2::Point3D GetResourceDepotLocation();

	void SetEnemyBase();
	void SetMyBase();
	void SetUnownedBase();

	//One of the starting positions on the map?  Player or enemy
	bool IsStartingPosition();
	void SetIsStartingPosition();
	//Id of the location that is a natural expansion of this base.  Only applied to starting bases, 0 otherwise.
	uint32_t GetNaturalExpansionId();
	//Id of the location that is a natural expansion of this base.  Should only be set for starting bases.
	void SetNaturalExpansionId(uint32_t _naturalId);
	//If this base is a natural expansion, of what base?  Only applied to naturals of starting bases, 0 otherwise.
	uint32_t GetParentOfNaturalId();
	//If this base is a natural expansion, of what base?  This should only be set for naturals of starting bases.
	void SetParentOfNaturalId(uint32_t _parentId);

	bool operator ==(BaseLocation rhs);
	bool operator !=(BaseLocation rhs);

private:
	enum BaseOwner {
		Unowned = 0,		//Or unknown.  Depends when we last saw it and what's happened since.
		Self = 1,
		Enemy = 2,
	};

	uint32_t baseLocationId;
	sc2::Point3D resourceDepotLocation;	//The ideal position for a CC, Nexus, Hatchery in this base location

	std::vector<const sc2::Unit*> mineralPatches;
	std::vector<const sc2::Unit*> geysers;

	BaseOwner baseOwner;
	//One of the starting positions on the map?  Player or enemy
	bool isStartingPosition;
	//Id of the location that is a natural expansion of this base.  Only applied to starting bases, 0 otherwise.
	uint32_t naturalExpansionId;
	//If this base is a natural expansion, of what base?  Only applied to naturals of starting bases, 0 otherwise.
	uint32_t parentOfNaturalId;


	//TODO:  Temp for now.  We should maintain a radius ourselves.  Or maybe an actual complex shape?
	//	Updated from 15.0f to 16.0f for Paladino Terminal.
	const float_t baseRadius = 16.0f;
};
