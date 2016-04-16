#include "Explosion.h"
#include "Global.h"

CExplosion::CExplosion(int _size, float _impulse, int _damage, int x, int y, int _type) : CBaseObject(x,y,_type,_size)
{
	
	impulse = _impulse;
	damage = _damage;
	frameSizeX = 256;
	frameSizeY = 256;
	renderOrder = 4;

	radius = _size;
	impulseCentre[0] = x + (radius / 2);
	impulseCentre[1] = y + (radius / 2);
	textureId = 501;
	animationData[blow][frameCountData] = 17;
	animationData[blow][msPerFrameData] = 50;
	animationData[blow][startFrameData] = 0;
	ChangeAnimation(blow);

}


CExplosion::~CExplosion()
{
}

int CExplosion::GetFrame()
{
	
	if (timeSinceAnimationStart / animationData[currentAnim][1] < animationData[currentAnim][frameCountData])
	{
		return animationData[currentAnim][startFrameData] + timeSinceAnimationStart / animationData[currentAnim][msPerFrameData];
	}
	else
	{
		ResetTime();
		return GetFrame();
	}
}