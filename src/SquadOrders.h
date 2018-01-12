#pragma once
#include <sc2api\sc2_api.h>

class SquadOrders
{
public:
	static SquadOrders Empty();

public:
	SquadOrders();
	SquadOrders(sc2::Point2D targetPoint);
	bool operator ==(SquadOrders& rhs);
	bool operator !=(SquadOrders& lhs);

	//priv?
public:
	sc2::Point2D currentTargetPoint;
};
