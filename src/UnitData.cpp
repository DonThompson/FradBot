#include "UnitData.h"

UnitData::UnitData()
{

}

UnitData::UnitData(sc2::UnitTypeData sourceData)
{
	//Copy!
	unit_type_id = sourceData.unit_type_id;
	name = sourceData.name;
	available = sourceData.available;
	cargo_size = sourceData.cargo_size;
	mineral_cost = sourceData.mineral_cost;
	vespene_cost = sourceData.vespene_cost;
	food_required = sourceData.food_required;
	food_provided = sourceData.food_provided;
	ability_id = sourceData.ability_id;
	race = sourceData.race;
	build_time = sourceData.build_time;
	tech_requirement = sourceData.tech_requirement;
	require_attached = sourceData.require_attached;
}
