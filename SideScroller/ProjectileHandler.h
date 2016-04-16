#ifndef PROJECTILEHANDLER_H
#define PROJECTILEHANDLER_H
#include <vector>
#include "PhysicsObject.h"
#include "SDL.h"
#include "Explosion.h"
#include "Projectile.h"
#include "VectorManager.h"


using std::vector;
class ProjectileHandler
{
public:
	ProjectileHandler(CVectorManager* vm);
	virtual ~ProjectileHandler();
	void HandleProjectiles();
	
	
private:
	void DoImpulse(CExplosion* & explosion);
	void HandleExplosions();
	CVectorManager* vm;
	vector<CPhysicsObject*>* objectVector;
	vector<CProjectile*>* projectileVector;
	vector<CBaseObject*>* baseVector;
	vector<CExplosion*>* explosionVector;
	
};

#endif