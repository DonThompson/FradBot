#pragma once

#include <sc2api/sc2_api.h>
#include "ManagerBase.h"

class DrawingManager : public ManagerBase
{
public:
	DrawingManager(Bot & b);
	~DrawingManager();

	//Called for each step of the game.  Not for public consumption.
	virtual void OnStep();

	void DrawCircle(sc2::Point3D pt, float_t radius, sc2::Color color = sc2::Colors::White);
	void DrawBox(sc2::Point3D pt1, float_t width, float_t height, sc2::Color color = sc2::Colors::White);
	void DrawText(std::string text, sc2::Point3D pt, sc2::Color color = sc2::Colors::White);

private:
	const float_t BonusCircleDrawingHeight = 1.05f;
};
