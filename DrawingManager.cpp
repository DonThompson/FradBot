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
void DrawingManager::DrawCircle(Point3D pt, float radius, sc2::Color color /*= Colors::White*/)
{
	pt.z *= BonusCircleDrawingHeight;	//raise it up slightly so it doesn't get stuck in nearby ground
	bot.Debug()->DebugSphereOut(pt, radius, color);
}
