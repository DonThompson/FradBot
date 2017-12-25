#include "BaseLocation.h"
#include "bot.h"
#include <sstream>
#include "Utils.h"

BaseLocation::BaseLocation(uint32_t _baseLocationId, Point3D _resourceDepotLocation)
	: baseLocationId(_baseLocationId)
	, baseOwner(BaseLocation::BaseOwner::Unowned)
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

	std::ostringstream oss;
	oss << "Base location " << baseLocationId << std::endl;
	bot.Draw().DrawText(oss.str(), resourceDepotLocation, color);
}

uint32_t BaseLocation::GetBaseLocationId()
{
	return baseLocationId;
}

Point3D BaseLocation::GetResourceDepotLocation()
{
	return resourceDepotLocation;
}

void BaseLocation::SetStartingBase(const Unit* resourceDepot)
{
	baseOwner = BaseOwner::Self;

	//Special.  Bases are initialized by looking at the map and finding room to build a command center.  However, because
	//	our starting position already has a command center, the position we find ends up being next to our existing one.
	//We'll just update with the one given here.
	resourceDepotLocation = resourceDepot->pos;
}

void BaseLocation::SetEnemyBase()
{
	baseOwner = BaseOwner::Enemy;
}

void BaseLocation::SetMyBase()
{
	baseOwner = BaseOwner::Self;
}

void BaseLocation::SetUnownedBase()
{
	baseOwner = BaseOwner::Unowned;
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