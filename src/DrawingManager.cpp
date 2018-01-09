#include "DrawingManager.h"
#include "bot.h"
using namespace sc2;

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
	pt2.z *= BonusCircleDrawingHeight;	//raise it up slightly so it doesn't get stuck in nearby ground
	bot.Debug()->DebugBoxOut(pt1, pt2, color);
}

void DrawingManager::DrawTextOnMap(std::string text, Point3D pt, Color color/*= Colors::White*/)
{
	bot.Debug()->DebugTextOut(text, pt, color);
}

void DrawingManager::DrawTextAtScreenPosition(std::string text, sc2::Point2D pt, sc2::Color color/*= sc2::Colors::White*/)
{
	bot.Debug()->DebugTextOut(text, pt, color);
}

void DrawingManager::DrawNoticeText(std::string text, sc2::Color color/*= sc2::Colors::White*/)
{
	bot.Debug()->DebugTextOut(text, color);
}

void DrawingManager::DrawLine(sc2::Point3D pt1, sc2::Point3D pt2, sc2::Color color/*= sc2::Colors::White*/)
{
	bot.Debug()->DebugLineOut(pt1, pt2, color);
}
