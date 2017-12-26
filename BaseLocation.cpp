#include "BaseLocation.h"
#include "bot.h"
#include <sstream>
#include "Utils.h"
using namespace sc2;

BaseLocation::BaseLocation(uint32_t _baseLocationId, Point3D _resourceDepotLocation)
	: baseLocationId(_baseLocationId)
	, baseOwner(BaseLocation::BaseOwner::Unowned)
	, isStartingPosition(false)
	, naturalExpansionId(0)
{
	resourceDepotLocation = _resourceDepotLocation;
}

bool BaseLocation::IsUnitInBase(const Unit* unit)
{
	if (Distance3D(resourceDepotLocation, unit->pos) < baseRadius)
		return true;
	return false;
}

bool BaseLocation::IsPointInBase(Point3D pt)
{
	if (Distance3D(resourceDepotLocation, pt) < baseRadius)
		return true;
	return false;
}

bool BaseLocation::IsPointInBase(Point2D pt)
{
	if (Distance2D(resourceDepotLocation, pt) < baseRadius)
		return true;
	return false;
}

void BaseLocation::AddMineralPatch(const Unit* patch)
{
	mineralPatches.push_back(patch);
}

void BaseLocation::AddGeyser(const Unit* geyser)
{
	geysers.push_back(geyser);
}

void BaseLocation::DrawSelf(Bot & bot)
{
	Color color = Colors::Gray;
	if (baseOwner == BaseOwner::Self)
		color = Colors::Green;
	else if (baseOwner == BaseOwner::Enemy)
		color = Colors::Red;

	//Highlight where the resource center is
	bot.Draw().DrawCircle(resourceDepotLocation, 2.75f, color);	//CC Radius is 2.75
	//And set a radius around the whole base location
	bot.Draw().DrawCircle(resourceDepotLocation, baseRadius, color);

	//Flags for start and naturals
	std::string flags;
	if (isStartingPosition) {
		std::ostringstream ossFlags;
		ossFlags << "(s->n" << naturalExpansionId << ")";
		flags += ossFlags.str();
	}
	if (parentOfNaturalId > 0) {
		std::ostringstream ossFlags;
		ossFlags << "(n:" << parentOfNaturalId << ")";
		flags += ossFlags.str();
	}

	std::ostringstream oss;
	oss << "Base location " << baseLocationId << flags << std::endl;
	bot.Draw().DrawText(oss.str(), resourceDepotLocation, color);

	for (const Unit* patch : mineralPatches) {
		bot.Draw().DrawCircle(patch->pos, patch->radius, color);
	}

	for (const Unit* geyser : geysers) {
		bot.Draw().DrawCircle(geyser->pos, geyser->radius, color);
	}
}

uint32_t BaseLocation::GetBaseLocationId()
{
	return baseLocationId;
}

Point3D BaseLocation::GetResourceDepotLocation()
{
	return resourceDepotLocation;
}

void BaseLocation::SetEnemyBase()
{
	baseOwner = BaseOwner::Enemy;
}

void BaseLocation::SetMyBase()
{
	baseOwner = BaseOwner::Self;
	isStartingPosition = true;
}

void BaseLocation::SetUnownedBase()
{
	baseOwner = BaseOwner::Unowned;
}

bool BaseLocation::IsMyBase()
{
	return baseOwner == BaseOwner::Self;
}

bool BaseLocation::IsEnemyBase()
{
	return baseOwner == BaseOwner::Enemy;
}

void BaseLocation::SetIsStartingPosition()
{
	isStartingPosition = true;
}

bool BaseLocation::IsStartingPosition()
{
	return isStartingPosition;
}

void BaseLocation::SetNaturalExpansionId(uint32_t _naturalId)
{
	naturalExpansionId = _naturalId;
}

uint32_t BaseLocation::GetNaturalExpansionId()
{
	return naturalExpansionId;
}

uint32_t BaseLocation::GetParentOfNaturalId()
{
	return parentOfNaturalId;
}

void BaseLocation::SetParentOfNaturalId(uint32_t _parentId)
{
	parentOfNaturalId = _parentId;
}

bool BaseLocation::operator ==(BaseLocation rhs)
{
	//Just match on Id
	if (rhs.baseLocationId == baseLocationId)
		return true;
	return false;
}

bool BaseLocation::operator !=(BaseLocation rhs)
{
	//Just match on Id
	if (rhs.baseLocationId != baseLocationId)
		return true;
	return false;
}
