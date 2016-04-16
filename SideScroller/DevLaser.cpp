#include "DevLaser.h"
#include "Global.h"

CDevLaser::CDevLaser(int x, int y, int _textureId, int width, int height, float degrees) : CBaseObject(x,y,_textureId, width, height)
{
	frameSizeX = 8;
	frameSizeY = 8;
	lifeInMs = 100;
	renderOrder = 3;
	rotation = degrees;
}


CDevLaser::~CDevLaser()
{
}

void CDevLaser::SetRotation(float _rotation)
{
	rotation = _rotation;
}
int CDevLaser::GetFrame()
{
	timeSinceAnimationStart += g_time;
	if (timeSinceAnimationStart/100 < 6)
	{
		return timeSinceAnimationStart / 100;
	}
	else
	{
		ResetTime();
		return 0;
	}
}

bool CDevLaser::UpdateLife()
{
	lifeInMs -= g_time;
	if (lifeInMs <= 0)
	{
		return false;
	}
	else return true;
}