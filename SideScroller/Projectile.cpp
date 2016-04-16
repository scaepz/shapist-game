#include "Projectile.h"


CProjectile::CProjectile(int x, int y, int textureId, int size, bool _firedByPlayer) : CPhysicsObject(x, y, textureId, size)
{
	weight = 1;
	renderOrder = 3;
	explosionData[explosive] = 1;
	explosionData[radius] = 128;
	explosionData[impulse] = 0.3;
	firedByPlayer = _firedByPlayer;

}

CProjectile::CProjectile(int x, int y, int textureId, int width, int height, bool _firedByPlayer, bool _subjectToGravity) : CPhysicsObject(x, y, textureId, width, height, _subjectToGravity)
{
	weight = 1;
	renderOrder = 3;
	explosionData[explosive] = 1;
	explosionData[radius] = 128;
	explosionData[impulse] = 0.3;
	firedByPlayer = _firedByPlayer;

}
CProjectile::~CProjectile()
{
}
int CProjectile::GetFrame()
{
	if (timeSinceAnimationStart / animationData[msPerFrameData] < animationData[frameCountData])
	{
		return timeSinceAnimationStart / animationData[msPerFrameData];
	}
	else
	{
		ResetTime();
		return GetFrame();
	}
}