#ifndef ROCKETLAUNCHER_H
#define ROCKETLAUNCHER_H
#include "Weapon.h"
#include "VectorManager.h"
#include "Rocket.h"
class CRocketLauncher :
	public CWeapon
{
public:
	bool Attack(int x, int y, float angle, CBaseObject* attacker);
	CRocketLauncher(CVectorManager * vm);
	virtual ~CRocketLauncher();
private:
	float projectileSpeed;
	CVectorManager * vm;
};

#endif