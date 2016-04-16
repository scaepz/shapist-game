#include "Grenade.h"


CGrenade::CGrenade(int x, int y, int textureId, int _frameSizeX, int _frameSizeY, int frames, int msPerFrame, int size, float xvel, float yvel, bool firedByPlayer, int timeAlive) : CProjectile(x, y, textureId, size, firedByPlayer)
{
	smallObject = true;
	lifeInMs = timeAlive;
	renderOrder = 2;
	velocity.SetX(xvel);
	velocity.SetY(yvel);
	frameSizeX = 200;
	frameSizeY = 200;

	explosionData[explosive] = 0; //bool

	animationData[frameCountData] = frames;
	animationData[msPerFrameData] = msPerFrame;


}
CGrenade::CGrenade(int x, int y, int textureId, int _frameSizeX, int _frameSizeY, int frames, int msPerFrame, int size, float xvel, float yvel, bool firedByPlayer, int _timeAlive, int _radius, int _impulse, int _damage) : CProjectile(x, y, textureId, size, firedByPlayer)
{
	smallObject = true;
	lifeInMs = _timeAlive;
	renderOrder = 2;
	velocity.SetX(xvel);
	velocity.SetY(yvel);
	explosionData[explosive] = 1; //bool
	explosionData[radius] = _radius;
	explosionData[impulse] = _impulse;
	explosionData[damage] = _damage;

	animationData[frameCountData] = frames;
	animationData[msPerFrameData] = msPerFrame;

	frameSizeX = _frameSizeX;
	frameSizeY = _frameSizeY;
}


bool CGrenade::UpdateLife()
{
	AddTime(g_time);
	lifeInMs -= g_time;
	if (lifeInMs <= 0)
	{
		return false;
	}
	else return true;
}

void CGrenade::OnCollision(bool stopHor, bool stopVer)
{
	if (stopHor)
	{
		velocity.SetX(velocity[0] * -0.5);

	}
	if (stopVer)
	{
		velocity.SetY(velocity[1] * -0.5);

	}
}

CGrenade::~CGrenade()
{
}



