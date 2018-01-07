#pragma once
#include <sc2api\sc2_api.h>
#include "ManagerBase.h"
class Bot;

class MapManager : public ManagerBase
{
public:
	MapManager(Bot & b);
	virtual void OnStep();
	virtual void OnGameStart();
};

