#include "Item.h"


CItem::CItem(int x, int y, int textureId, int size) :CPhysicsObject(x,y,textureId,size)
{
	randSeed = SDL_GetTicks();
	renderOrder = 2;
	ableToPassThroughEntities = true;
}


CItem::~CItem()
{
}
int CItem::GetFrame()
{
	return 0;
}
void CItem::OnCollision(bool stopHor, bool stopVer)
{
	if (stopHor)
	{
		velocity.SetX(velocity[0] * -0.2);

	}
	if (stopVer)
	{
		velocity.SetX(velocity[0] * -0.2);
		velocity.SetY(velocity[1] * -0.2);
		if (abs(velocity[1]) < 0.004)
		{
			velocity.SetY(0);
		}
		if (abs(velocity[0]) < 0.004)
		{
			velocity.SetX(0);
		}
	}
}