#pragma once
#include <sc2api/sc2_api.h>
#include "ManagerBase.h"

class Bot;

class StrategyManager : public ManagerBase
{
public:
	StrategyManager(Bot & b);

	virtual void OnGameStart();
	virtual void OnStep();


	//Various build orders
	void BO_ExpandFirstIntoBioBall();
	void BO_SimpleBio();
};
