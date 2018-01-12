#pragma once
#include <sc2api/sc2_api.h>

class PlatoonOrders
{
public:
	enum ORDER_TYPE {
		GATHER = 0,
		ATTACK,
		DEFEND,
	};
public:
	PlatoonOrders(ORDER_TYPE _type, sc2::Point2D _targetPt);
	PlatoonOrders();
	void Clear();

	//Do we have any orders?
	bool hasOrders;
	//The type of order given
	ORDER_TYPE orderType;
	//The target point of these orders
	sc2::Point2D targetPoint;
};

