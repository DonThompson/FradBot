#pragma once
#include <sc2api/sc2_api.h>

//Local (faster) copy of sc2::UnitTypeData with data that doesn't change
//	Simply copied everything out of sc2::UnitTypeData and hid (toward the bottom)
//	those things that we don't need now or that look to change.
class UnitData
{
public:
	UnitData();
	UnitData(sc2::UnitTypeData sourceData);

	//! Stable ID. This ID will not change between patches.
	sc2::UnitTypeID unit_type_id;
	//! Unit type name, corresponds to the game's catalog.
	std::string name;
	//! If true, the unit is available to the current mods/map.
	bool available;
	//! Number of cargo slots they occupy in a transport.
	uint32_t cargo_size;
	//! Cost in minerals to build this unit type.
	int mineral_cost;
	//! Cost in vespene to build this unit type.
	int vespene_cost;
	//! How much food the unit requires.
	float food_required;
	//! How much food the unit provides.
	float food_provided;
	//! Which ability id creates the unit.
	sc2::AbilityID ability_id;
	//! The race the unit belongs to.
	sc2::Race race;
	//! How long the unit takes to build.
	float build_time;
	//! Structure required to build this unit. (Or any with the same tech_alias)
	sc2::UnitTypeID tech_requirement;
	//! Whether tech_requirement is an add-on.
	bool require_attached;


	//Manual additions
	sc2::UnitTypeID producingBuilding;


	//Hide stuff we aren't (yet) using here in case we want it in the future
private:
	//! Whether the unit can have minerals (mineral patches).
	//bool has_minerals;
	//! Whether the unit can have vespene (vespene geysers).
	//bool has_vespene;
	//! Units that are morphed variants of the same unit.
	//sc2::UnitTypeID unit_alias;
	//! Units this is equivalent to in terms of satisfying tech requirements.
	//std::vector<sc2::UnitTypeID> tech_alias;



	//These things look to change, and as such we should go directly to the GetUnitData() to lookup
private:
	//! Unit attributes, may change based on upgrades.
	//std::vector<sc2::Attribute> attributes;
	//! Movement speed of unit type.
	//float movement_speed;
	//! Armor of unit type.
	//float armor;
	//! Weapons on this unit type.
	//std::vector<sc2::Weapon> weapons;
	//! Range the unit reveals vision.
	//float sight_range;

};

