#pragma once

#include <sc2api/sc2_api.h>
using namespace sc2;
#include "ManagerBase.h"

class DrawingManager : public ManagerBase
{
public:
	DrawingManager(Bot & b);
	~DrawingManager();

	//Called for each step of the game.  Not for public consumption.
	virtual void OnStep();

	void DrawCircle(Point3D pt, float_t radius, sc2::Color color = Colors::White);
	void DrawBox(Point3D pt1, float_t width, float_t height, sc2::Color color = Colors::White);
	void DrawText(std::string text, Point3D pt, Color color = Colors::White);

private:
	const float_t BonusCircleDrawingHeight = 1.05f;
};
