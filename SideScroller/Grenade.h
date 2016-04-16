#ifndef GRENADE_H
#define GRENADE_H
#include "Projectile.h"
#include "vector2.h"
#include "Global.h"
#include <vector>
#include "Explosion.h"
using std::vector;
class CGrenade :
	public CProjectile
{
public:
	CGrenade(int x, int y, int textureId, int frameSizeX, int frameSizeY, int frames, int msPerFrame, int size, float xvel, float yvel, bool firedByPlayer, int timeAlive);
	CGrenade(int x, int y, int textureId, int frameSizeX, int frameSizeY, int frames, int msPerFrame, int size, float xvel, float yvel, bool firedByPlayer, int timeAlive, int radius, int impulse, int damage);
	virtual ~CGrenade();
	
	void OnCollision(bool, bool);
	

	bool UpdateLife();
private:
	//vector<CExplosion*>* explosionVector;
	int lifeInMs;

};

#endif