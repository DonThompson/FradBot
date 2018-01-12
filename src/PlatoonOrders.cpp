#include "PlatoonOrders.h"

PlatoonOrders::PlatoonOrders(ORDER_TYPE _type, sc2::Point2D _targetPt)
{
	hasOrders = true;
	orderType = _type;
	targetPoint = _targetPt;
}

PlatoonOrders::PlatoonOrders()
{
	Clear();
}

//Initialize/Clear the orders to nothing
void PlatoonOrders::Clear()
{
	hasOrders = false;
	orderType = ORDER_TYPE::GATHER;
	targetPoint = sc2::Point2D(0, 0);
}
