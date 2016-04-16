#ifndef ROCKET_H
#include "Global.h"
#include "Projectile.h"
class CRocket :
	public CProjectile
{
public:
	CRocket(int x, int y, int textureId, int frameSizeX, int frameSizeY, int frames, int msPerFrame, int width, int height, float xvel, float yvel, bool firedByPlayer, int timeAlive, int radius, int impulse, int damage);
	virtual ~CRocket();
	int lifeInMs;
	void OnCollision(bool, bool);

	bool UpdateLife();

};
#define ROCKET_H
#endif // !ROCKET_H

