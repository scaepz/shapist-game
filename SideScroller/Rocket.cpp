#include "Rocket.h"


CRocket::CRocket(int x, int y, int textureId, int _frameSizeX, int _frameSizeY, int frames, int msPerFrame, int width, int height, float xvel, float yvel, bool firedByPlayer, int _timeAlive, int _radius, int _impulse, int _damage) : CProjectile(x,y,textureId, width, height, firedByPlayer, false)
{
	smallObject = true;
	lifeInMs = 10000;
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


CRocket::~CRocket()
{
}

void CRocket::OnCollision(bool a , bool b)
{
	if (b || a)
	{
		lifeInMs = 0;
	}
}


bool CRocket::UpdateLife()
{
	//bandaid fix for rocket sometimes stopping and hovering in air
	if (abs(velocity[0]) < 0.1f && abs(velocity[1] < 0.1f))
	{
		lifeInMs = 0;
	}
	AddTime(g_time);
	return (lifeInMs > 0);
}