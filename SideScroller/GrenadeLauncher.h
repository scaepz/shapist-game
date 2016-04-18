#ifndef GRENADELAUNCHER_H
#define GRENADELAUNCHER_H

#include "Weapon.h"
#include <vector>
#include "Grenade.h"

#include "VectorManager.h"
using std::vector;
class CGrenadeLauncher :
	public CWeapon
{
public:
	bool Attack(int x, int y, float angle, CBaseObject* attacker);
	CGrenadeLauncher(CVectorManager * vm);
	virtual ~CGrenadeLauncher();
	CVectorManager* vm;
	
	float projectileSpeed;


};

#endif