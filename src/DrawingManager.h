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
	//! Outputs text at any 3D point in the game world. Map coordinates are used.
	void DrawTextOnMap(std::string text, sc2::Point3D pt, sc2::Color color = sc2::Colors::White);
	//! Outputs text at any 2D point on the screen. Coordinate ranges are 0..1 in X and Y.
	void DrawTextAtScreenPosition(std::string text, sc2::Point2D pt, sc2::Color color = sc2::Colors::White);
	//! Outputs text at the top, left of the screen.
	void DrawNoticeText(std::string text, sc2::Color color = sc2::Colors::White);
	//! Outputs a line between two 3D points in the game world. Map coordinates are used.
	void DrawLine(sc2::Point3D pt1, sc2::Point3D pt2, sc2::Color color = sc2::Colors::White);

private:
	const float_t BonusCircleDrawingHeight = 1.05f;
};
