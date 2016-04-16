#include "EventTriggerZone.h"
#include "CollisionDetector.h"

CEventTriggerZone::CEventTriggerZone(CVectorManager * vm, int id) : CEventTrigger(vm, id)
{
}


CEventTriggerZone::~CEventTriggerZone()
{
}

void CEventTriggerZone::SetArea(int x, int y, int _width, int _height)
{
	pos[0] = x;
	pos[1] = y;
	width = _width;
	height = _height;
}

bool CEventTriggerZone::IsTriggered()
{
	//if player is touching zone return true else return false
	CBaseObject * obj1 = vm->GetPlayer();
	CCollisionDetector cd;
	return cd.Collides(obj1->GetX(), obj1->GetY(), obj1->GetWidth(), obj1->GetHeight(), pos[0], pos[1], width, height);

}