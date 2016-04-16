#include "TrapObject.h"


CTrapObject::CTrapObject(int x, int y) : CBaseObject(x,y)
{
}


CTrapObject::~CTrapObject()
{
}

void CTrapObject::UpdateTime(int time)
{
	timeSinceDmgDealt += time;
	timeSinceAnimationStart += time;
}