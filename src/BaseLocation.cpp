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
	, resourceDepot(nullptr)
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
	//Use raw distance, not pathing distance.  We'll eventually be removing this methodology.
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

	/*draw all tiles in each base region - crazy slow
	for (TilePosition tile : bot.Map().GetRegionTiles(regionId)) {
		Point2D pt = tile.first;
		Point3D usablePt(pt.x, pt.y, resourceDepotLocation.z);
		//std::shared_ptr<Tile> t = tile.second;
		bot.Draw().DrawBox(usablePt, 1, 1, color);
	}
	*/

	/*draw all edges - NOT IMPLEMENTED IN OVERSEER
	for (RegionEdge edge : bot.Map().GetRegionEdges(regionId)) {
		//EdgeType t = edge.getEdgeType();
		//edge.getRegions();
		std::vector<TilePosition> positions = edge.getPoints();
		for (TilePosition tilePosition : positions) {
			Point2D pt = tilePosition.first;
			Point3D usablePt(pt.x, pt.y, resourceDepotLocation.z);
			//std::shared_ptr<Tile> t = tile.second;
			bot.Draw().DrawBox(usablePt, 1, 1, color);
		}
	}
	*/

	//Draw all choke points of this base
	for (Point2D pt : chokePoints) {
		Point3D ptUsable(pt.x, pt.y, resourceDepotLocation.z);
		bot.Draw().DrawCircle(ptUsable, 1.0f, color);
	}

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
	oss << "Base location " << baseLocationId << flags << " Region " << regionId << std::endl;
	bot.Draw().DrawTextOnMap(oss.str(), resourceDepotLocation, color);

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

Structure & BaseLocation::GetResourceDepot()
{
	return resourceDepot;
}

void BaseLocation::SetResourceDepot(Structure _depot)
{
	resourceDepot = _depot;
}

void BaseLocation::SetEnemyBase()
{
	baseOwner = BaseOwner::Enemy;
}

void BaseLocation::SetMyStartingBase()
{
	baseOwner = BaseOwner::Self;
	isStartingPosition = true;
}

void BaseLocation::ClaimBaseForPlayer()
{
	baseOwner = BaseOwner::Self;
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

size_t BaseLocation::GetRegionId()
{
	return regionId;
}

std::vector<Point2D> BaseLocation::GetChokePoints()
{
	return chokePoints;
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

void BaseLocation::SetRegionId(size_t _regionId)
{
	regionId = _regionId;
}

void BaseLocation::SetChokePoints(std::vector<Point2D> _points)
{
	chokePoints = _points;
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
