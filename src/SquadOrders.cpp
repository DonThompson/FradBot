#include "SquadOrders.h"
using namespace sc2;

/*static */SquadOrders SquadOrders::Empty()
{
	return SquadOrders();
}

SquadOrders::SquadOrders()
	: currentTargetPoint(Point2D(0, 0))
{

}

SquadOrders::SquadOrders(sc2::Point2D targetPoint)
	: currentTargetPoint(targetPoint)
{
}

//Are two sets of squad orders the same?
bool SquadOrders::operator ==(SquadOrders& rhs)
{
	//For now, just look at the target point
	return currentTargetPoint == rhs.currentTargetPoint;
}

bool SquadOrders::operator !=(SquadOrders& rhs)
{
	//For now, just look at the target point
	return currentTargetPoint != rhs.currentTargetPoint;
}
