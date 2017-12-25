#include "DrawingManager.h"
#include "bot.h"

DrawingManager::DrawingManager(Bot & b)
	: ManagerBase(b)
{
}


DrawingManager::~DrawingManager()
{
}

void DrawingManager::OnStep()
{
	//Not subscribed to game events!
}

//All circles are drawn with the z position slightly above the given z position to avoid clipping
//	into nearby terrain.
void DrawingManager::DrawCircle(Point3D pt, float_t radius, sc2::Color color /*= Colors::White*/)
{
	pt.z *= BonusCircleDrawingHeight;	//raise it up slightly so it doesn't get stuck in nearby ground
	bot.Debug()->DebugSphereOut(pt, radius, color);
}

void DrawingManager::DrawBox(Point3D pt1, float_t width, float_t height, sc2::Color color /*= Colors::White*/)
{
	Point3D pt2(pt1.x + width, pt1.y + height, pt1.z);
	bot.Debug()->DebugBoxOut(pt1, pt2, color);
}

void DrawingManager::DrawText(std::string text, Point3D pt, Color color/*= Colors::White*/)
{
	bot.Debug()->DebugTextOut(text, pt, color);
}
